#include "zuartrecv.h"
#include <QImage>
#include <QDebug>
ZUARTRecv::ZUARTRecv(QObject *parent) : QObject(parent)
{
    this->m_uart=nullptr;
    this->m_rxBuffer=nullptr;
}
ZUARTRecv::~ZUARTRecv()
{
    delete this->m_uart;
    delete [] this->m_rxBuffer;
}
bool ZUARTRecv::ZDoInit()
{
    //buffer to hold received data, Visible Light JPEG image or Infrared Image.
    //buffer size: 5MBytes.
    this->m_rxBuffer=new quint8[RX_BUFFER_SIZE];
    if(nullptr==this->m_rxBuffer)
    {
        emit this->ZSignalLog(tr("Failed to allocate 5MBytes memory!"));
        return false;
    }
    //////////////////////////////////////////////////////////////////////////////////
    this->m_uart=new QSerialPort;
    if(nullptr==this->m_uart)
    {
        return false;
    }
    this->m_uart->setPortName("COM23");
    this->m_uart->setBaudRate(4000000); //4Mbps.
    this->m_uart->setDataBits(QSerialPort::Data8);
    this->m_uart->setStopBits(QSerialPort::OneStop);
    this->m_uart->setParity(QSerialPort::NoParity);
    this->m_uart->setFlowControl(QSerialPort::NoFlowControl);
    connect(this->m_uart,SIGNAL(readyRead()),this,SLOT(ZSlotDataReady()));
    ////////////////////////////////////////////////////////////////////////
    this->m_imgIR=QImage(256,192,QImage::Format_RGB888);
    this->m_imgTemperature=QImage(256,192,QImage::Format_RGB888);
    //192 rows * 256 columns.
    this->m_arrayTemp.resize(192);
    for(qint32 i=0;i<this->m_arrayTemp.size();i++)
    {
        this->m_arrayTemp[i].resize(256);
    }
    return true;
}
bool ZUARTRecv::ZIsOpened()
{
    if(nullptr==this->m_uart)
    {
        return false;
    }
    return this->m_uart->isOpen();
}
bool ZUARTRecv::ZOpenUART(QString uartName)
{
    this->m_uart->setPortName(uartName);
    if(!this->m_uart->open(QIODevice::ReadWrite))
    {
        emit this->ZSignalLog(this->m_uart->errorString());
        return false;
    }
    emit this->ZSignalLog(uartName+",4Mbps/8N1");

    //reset counters.
    this->m_rxBufLen=0;
    this->m_rxBufLenReplicated=0;
    this->m_iRxBytes=0;
    this->m_iRxIRFrames=0;
    this->m_iRxVLFrames=0;
    this->m_iRxErrFrames=0;
    return true;
}
void ZUARTRecv::ZCloseUART()
{
    if(nullptr==this->m_uart)
    {
        return;
    }
    this->m_uart->close();
    this->m_rxBufLen=0;
    this->m_rxBufLenReplicated=0;
    this->m_iRxIRFrames=0;
    this->m_iRxVLFrames=0;
    this->m_iRxErrFrames=0;
    this->m_iRxBytes=0;
}
void ZUARTRecv::ZSlotDataReady()
{
    //check if we have adequate space to hold all available bytes.
    qint32 iSpaceRemaing=RX_BUFFER_SIZE-this->m_rxBufLen;
    qint32 iRdMax=this->m_uart->bytesAvailable();
    qDebug()<<"Remain:"<<iSpaceRemaing<<", byteAvailable:"<<iRdMax<<",BufferLen:"<<this->m_rxBufLen;
    if(iSpaceRemaing>=iRdMax)
    {
        qint32 iRdBytes=this->m_uart->read((char*)(this->m_rxBuffer+this->m_rxBufLen),iRdMax);
        if(iRdBytes<0)
        {
            emit this->ZSignalLog(this->m_uart->errorString());
        }else{
            this->m_rxBufLen+=iRdBytes;
            emit this->ZSignalRxBytes(this->m_rxBufLen);
            this->m_rxBufLenReplicated=this->m_rxBufLen;
        }
    }else{
        emit this->ZSignalLog("Rx Buffer overflow, reset.");
        this->m_rxBufLen=0;
        emit this->ZSignalRxBytes(this->m_rxBufLen);
        return;
    }

    //Minimum data length check.
    //Normally, for OV5640, resolution is 2596*1944. data size is greater than 1MBytes.
    //For Infrared Image Sensor, resolution is 256*192.
    //One Single Line is 16 sync header bytes+256*2Bytes(Pixel)+256*2Bytes(Temperature)=1040Bytes
    //Total bytes is 192Lines*1040=199680
    if(this->m_rxBufLen<199680)
    {
        return; //no adequate data, needs more data, maybe process next time.
    }

    //The first 20-bytes are sync frame header bytes.
    //searching Sync Header.
    this->m_frameOffset=0;
    char iFrameType=0x0; //0x0:Invaild Frame, 0x1:OV5640 Visible Light Image, 0x2:Infrared Image, 0x3: Ultraviolet Image.
    while(m_frameOffset<(this->m_rxBufLen-20))
    {
        uchar *p=(uchar*)(this->m_rxBuffer+m_frameOffset);
        if(p[4]==0x01 && p[5]==0x48 && p[6]==0x90 && p[7]==0x82 && p[8]==0x03 && p[9]==0x0e && p[10]==0x76)
        {
            iFrameType=0x1; //0x1:Infrared Image.
            break;
        }else if(p[4]==0x56 && p[5]==0x40 && p[9]==0x0e && p[10]==0x76)
        {
            iFrameType=0x2; //0x2:OV5640 Visible Light Image.
            break;
        }else{
            this->m_frameOffset++;
        }
    }
    qDebug()<<"FrameStartOffset="<<this->m_frameOffset;
    //check searching result.
    quint8 *pHead=(quint8*)(this->m_rxBuffer+this->m_frameOffset);
    quint8 *pData=(quint8*)(this->m_rxBuffer+this->m_frameOffset+20);
    this->m_frameLength=pHead[0]<<24|pHead[1]<<16|pHead[2]<<8|pHead[3]<<0;
    QByteArray baSensorID((const char*)(pHead+4),5);
    QByteArray baHyperRAMID((const char*)(pHead+9),2);
    QByteArray baReserved((const char*)(pHead+11),9);
    emit this->ZSignalLog(QString::asprintf("Frame Length: %02X%02x%02x%02X(%d), ",pHead[0],pHead[1],pHead[2],pHead[3],this->m_frameLength) + ///<
                          QString::asprintf("Image Sensor ID: %02X%02X%02X%02X%02X, ",///<
                                            (quint8)baSensorID[0],(quint8)baSensorID[1],///<
                                            (quint8)baSensorID[2],(quint8)baSensorID[3],///<
                                            (quint8)baSensorID[4]) + ///<
                          QString::asprintf("HyperRAM ID: %02X%02X.",(quint8)baHyperRAMID[0],(quint8)baHyperRAMID[1]));
    switch(iFrameType)
    {
    case 0x0: //0x0:Invaild Frame.
        emit this->ZSignalLog("No frame head bytes found, reset.");
        break;
    case 0x1: //0x1:Infrared Image.
        if((this->m_rxBufLen-this->m_frameOffset)<this->m_frameLength)
        {
            emit this->ZSignalLog("Infrared Frame Found, but more data needed.");
            return;
        }
        //Sync Head Bytes is 20 bytes. CRC32 is 4 bytes.
        if(this->ZParseSingleFrame_IR(pData,this->m_rxBufLen-this->m_frameOffset-20-4))
        {
            //render successfully.
            emit this->ZSignalRxNewFrames(1+this->m_iRxIRFrames++,this->m_iRxVLFrames,this->m_iRxErrFrames);
        }else{
            //render failed.
            emit this->ZSignalRxNewFrames(this->m_iRxIRFrames,this->m_iRxVLFrames,1+this->m_iRxErrFrames++);
        }
        break;
    case 0x2: //0x2:OV5640 Visible Light Image.
        if((this->m_rxBufLen-this->m_frameOffset)<this->m_frameLength)
        {
            emit this->ZSignalLog("OV5640 Frame Found, but more data needed.");
            return;
        }
        //Sync Head Bytes is 20 bytes. CRC32 is 4 bytes.
        if(this->ZParseSingleFrame_OV5640(pData,this->m_rxBufLen-this->m_frameOffset-20-4))
        {
            //render successfully.
            emit this->ZSignalRxNewFrames(this->m_iRxIRFrames,1+this->m_iRxVLFrames++,this->m_iRxErrFrames);
        }else{
            //render failed.
            emit this->ZSignalRxNewFrames(this->m_iRxIRFrames,this->m_iRxVLFrames,1+this->m_iRxErrFrames++);
        }
        break;
    default:
        emit this->ZSignalLog("No frame head bytes found, reset.");
        break;
    }

    //after processing, reset Rx Buffer.
    this->m_rxBufLenReplicated=this->m_rxBufLen;
    this->m_rxBufLen=0;
    return;
}
QColor ZUARTRecv::ZMapTemperature2Color(quint16 tTemp)
{
    //How to map 16-bits temperature to RGB is a question here!!!
    //Find a 16-bits color map online and map 16-bits temperature to RGB.
    //  quint8 tR=(tTemp&0xFF);
    //  quint8 tG=0;
    //  quint8 tB=0;
    //  return QColor(tR,tG,tB);

    qint32 diffTemp=this->m_maxTemp-this->m_minTemp+1;
    //here we split diff into 6 gaps.
    qint32 iGapStep=(diffTemp/(6-1));
    //calculate which Gap does tTemp belongs.
    float fRatio=(tTemp-this->m_minTemp)/(diffTemp*1.0f);
    qint32 iGapNo=(qint32)(fRatio*(6-1));
    qDebug("%d,%d,%d,fRatio=%.2f,iGapNo=%d\n",tTemp,this->m_minTemp,diffTemp,fRatio,iGapNo);
    emit this->ZSignalLog(QString("Max:%1,Min:%2,Diff:%3,GapStep(/6):%4, Temp(%5) -> GapNo(%6).")///<
                              .arg(this->m_maxTemp).arg(this->m_minTemp).arg(diffTemp).arg(iGapStep).arg(tTemp-this->m_minTemp).arg(iGapNo));
    /////////////////////////////////////////////////////////////////////////////////////////
    qint32 iGapMax=(iGapNo+1)*iGapStep+this->m_minTemp;
    qint32 iGapMin=(iGapNo)*iGapStep+this->m_minTemp;
    float fPartialOffset=(tTemp-iGapMin)/((iGapMax-iGapMin)*1.0f);
    qDebug("[%d -(%d)- %d], PartialOffset:%.2f\n",iGapMin,tTemp,iGapMax,fPartialOffset);
    ////////////////////////////////////////////////////////////////////////////////////////
    if(tTemp<this->m_minTemp)
    {
        return QColor(0,0,0);
    }
    if(tTemp>this->m_maxTemp)
    {
        return QColor(255,255,255);
    }
    switch(iGapNo)
    {
    case 0:
        return QColor(0,int(fPartialOffset*255),255);
    case 1:
        return QColor(0,255,int((1.0f-fPartialOffset)*255));
    case 2:
        return QColor(int(fPartialOffset*255),255,0);
    case 3:
        return QColor(255,int((1.0f-fPartialOffset)*255),0);
    case 4:
        return QColor(255,0,int(fPartialOffset*255));
    case 5:
        return QColor(255,0,int((1.0f-fPartialOffset)*255));
    default:
        return QColor(255,255,255);
    }
}
void ZUARTRecv::ZSlotFetchIRImageData(qint32 iRow, qint32 iCol)
{

}
void ZUARTRecv::ZSlotFetchTempImageData(qint32 iRow, qint32 iCol)
{
    if(iRow>=0 && iRow<=192-1 && iCol>=0 && iCol<=256-1)
    {
        quint16 tTemp=this->m_arrayTemp.at(iRow).at(iCol);
        emit this->ZSignalLog(QString("Temperature[%1,%2]=%3.").arg(iRow).arg(iCol).arg(tTemp));
    }
}
bool ZUARTRecv::ZParseSingleFrame_IR(quint8 *data, quint32 length)
{
    //check length.
    //1040-Bytes*192-Lines.
    if(length!=(1040*192))
    {
        emit this->ZSignalLog(tr("Error, Render Infrared Image, but data length does not match!"));
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    //1st 16 bytes are FF0000B6, FF0000AB, FF00009D, FF000080.
    //equals to        FFFFFFFF, 00000001, FF00009D, FF000080.
    bool bLineHead=false;
    if( (data[0]==0xFF) && (data[1]==0x00) && (data[2]==0x00) && (data[3]==0xB6) && ///<
        (data[4]==0xFF) && (data[5]==0x00) && (data[6]==0x00) && (data[7]==0xAB) && ///<
        (data[8]==0xFF) && (data[9]==0x00) && (data[10]==0x00) && (data[11]==0x9D) && ///<
        (data[12]==0xFF) && (data[13]==0x00) && (data[14]==0x00) && (data[15]==0x80)) ///<
    {
        bLineHead=true;
    }
    if(!bLineHead)
    {
        emit this->ZSignalLog(QString::asprintf("Error, Infrared Frame Head Does Not Match," ///<
                                 "Expected:FF0000B6-FF0000AB-FF00009D-FF000080,"///<
                                "Actually:%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X",///<
                                                (quint32)data[0],(quint32)data[1],(quint32)data[2],(quint32)data[3], ///<
                                                (quint32)data[4],(quint32)data[5],(quint32)data[6],(quint32)data[7], ///<
                                                (quint32)data[8],(quint32)data[9],(quint32)data[10],(quint32)data[11], ///<
                                                (quint32)data[12],(quint32)data[13],(quint32)data[14],(quint32)data[15]));
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    this->m_maxTemp=0; this->m_minTemp=0xFFFF;
    quint32 lineOffset=0;
    for(qint32 iRowNo=0; iRowNo<192; iRowNo++)  //192 Lines.
    {
        emit this->ZSignalRenderProgress(iRowNo);
        QString lineHead=QString::asprintf("%02x %02x %02x %02x", ///<
                                             (quint8)data[lineOffset+0], (quint8)data[lineOffset+1], ///<
                                             (quint8)data[lineOffset+2], (quint8)data[lineOffset+3]);
        emit this->ZSignalHexData(lineHead);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        lineHead=QString::asprintf("%02x %02x %02x %02x", ///<
                                     (quint8)data[lineOffset+4], (quint8)data[lineOffset+5], ///<
                                     (quint8)data[lineOffset+6], (quint8)data[lineOffset+7]);
        emit this->ZSignalHexData(lineHead);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        lineHead=QString::asprintf("%02x %02x %02x %02x", ///<
                                     (quint8)data[lineOffset+8], (quint8)data[lineOffset+9], ///<
                                     (quint8)data[lineOffset+10], (quint8)data[lineOffset+11]);
        emit this->ZSignalHexData(lineHead);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        lineHead=QString::asprintf("%02x %02x %02x %02x", ///<
                                     (quint8)data[lineOffset+12], (quint8)data[lineOffset+13], ///<
                                     (quint8)data[lineOffset+14], (quint8)data[lineOffset+15]);
        emit this->ZSignalHexData(lineHead);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bypass 16 bytes of each line.
        qint32 uPixelOffset=16; //1st 16 bytes are FFFFFFFF,00000001,FF00009D,FF000080.
        qint32 iColNo=0;
        //256*2Bytes are pixel data, 512bytes/4=128.
        for(qint32 i=0; i<128; i++)
        {
            quint8 uCb=data[lineOffset+uPixelOffset+0];
            quint8 uY1=data[lineOffset+uPixelOffset+1];
            quint8 uCr=data[lineOffset+uPixelOffset+2];
            quint8 uY2=data[lineOffset+uPixelOffset+3];
            uPixelOffset+=4;

            //qDebug("CbYCrY: %02x %02x %02x %02x\n", uCb,uY1,uCr,uY2);

            //YUV to RGB.
            uint8_t R1=uY1+1.371*(uCr-128);
            uint8_t G1=uY1-0.698*(uCr-128)-0.336*(uCb-128);
            uint8_t B1=uY1+1.732*(uCb-128);

            uint8_t R2=uY2+1.371*(uCr-128);
            uint8_t G2=uY2-0.698*(uCr-128)-0.336*(uCb-128);
            uint8_t B2=uY2+1.732*(uCb-128);

            this->m_imgIR.setPixelColor(iColNo+0,iRowNo,QColor(R1,G1,B1));
            this->m_imgIR.setPixelColor(iColNo+1,iRowNo,QColor(R2,G2,B2));
            iColNo+=2; //We set two pixels one time.
        }
        //256*2Bytes are temperature data, 512Bytes/4=128.
        iColNo=0;
        for(qint32 j=0;j<128; j++)
        {
            quint8 uCb=data[lineOffset+uPixelOffset+0];
            quint8 uY1=data[lineOffset+uPixelOffset+1];
            quint8 uCr=data[lineOffset+uPixelOffset+2];
            quint8 uY2=data[lineOffset+uPixelOffset+3];
            uPixelOffset+=4;

            quint16 tTemp1=(quint16)uY1<<8|uCb;
            quint16 tTemp2=(quint16)uY2<<8|uCr;
            qDebug("Temperature: %02x %02x %02x %02x, %04x/%04x, %d/%d\n", uCb,uY1,uCr,uY2,tTemp1,tTemp2,tTemp1,tTemp2);
            //save temperature data into two-dimension array for future temperature chart display.
            this->m_arrayTemp[iRowNo][iColNo+0]=tTemp1;
            this->m_arrayTemp[iRowNo][iColNo+1]=tTemp2;

            //find out the maximum value and minimum value.
            this->m_maxTemp=(tTemp1>this->m_maxTemp)?(tTemp1):(this->m_maxTemp);
            this->m_minTemp=(tTemp1<this->m_minTemp)?(tTemp1):(this->m_minTemp);
            ///////////////////////////////////////////////////////////////////
            this->m_maxTemp=(tTemp2>this->m_maxTemp)?(tTemp2):(this->m_maxTemp);
            this->m_minTemp=(tTemp2<this->m_minTemp)?(tTemp2):(this->m_minTemp);
            emit this->ZSignalMaxMinDiffTempChanged(this->m_maxTemp,this->m_minTemp,this->m_maxTemp-this->m_minTemp);
            //////////////////////////////////////////////////////////////////////
            iColNo+=2;
        }
        //start to process next line.
        if((lineOffset+1040)>length)
        {
            break;
        }
        else{
            lineOffset+=1040; //process next line.
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    emit this->ZSignalLog(QString("Rendering Temperature Image ..."));
    qint32 iRow=0,iCol=0;
    for(QVector<QVector<quint16>>::iterator pRow=this->m_arrayTemp.begin(); pRow!=this->m_arrayTemp.end();pRow++)
    {
        for(QVector<quint16>::iterator pCol=pRow->begin();pCol!=pRow->end();pCol++)
        {
            QColor tColor=this->ZMapTemperature2Color(*pCol);
            this->m_imgTemperature.setPixelColor(iCol,iRow,tColor);
            iCol++; //next column.
        }
        iRow++; iCol=0; //next row.
        emit this->ZSignalRenderProgress(iRow);
    }
    //process one frame completely, reset.
    emit this->ZSignalNewImage(this->m_imgIR, this->m_imgTemperature);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    emit this->ZSignalLog(QString("Received IR Frame: MaxTemp=%1, MinTemp=%2, Diff=%3.") ///<
                              .arg(this->m_maxTemp).arg(this->m_minTemp).arg(this->m_maxTemp-this->m_minTemp));
    return true;
}
bool ZUARTRecv::ZParseSingleFrame_OV5640(quint8 *data, quint32 length)
{
    //check JPEG head fixed bytes.
    if(data[0]!=0xFF || data[1]!=0xD8)
    {
        emit this->ZSignalLog(tr("Invalid JPEG Head, Expected:FF D8, Actually:")+QString::asprintf("%02X %02X",data[0],data[1]));
        return false;
    }
    //locate JPEG tail fixed bytes.
    quint32 iTailOffset=0;
    for(quint32 i=length;i>=2;i--)
    {
        if(0xFF==data[i-2] && 0xD9==data[i-1])
        {
            iTailOffset=i;
            break;
        }
    }
    if(0==iTailOffset)
    {
        emit this->ZSignalLog(tr("Failed to locate JPEG Tail FF D9."));
        return false;
    }
    if(!this->m_imgOV5640.loadFromData((const uchar*)data,length-(length-iTailOffset),"JPG"))
    {
        emit this->ZSignalLog(tr("Failed to load jpeg from data."));
        return false;
    }
    emit this->ZSignalNewJPEG(this->m_imgOV5640);
    return true;
}
quint8* ZUARTRecv::ZGetBufferAddress()
{
    return this->m_rxBuffer;
}
quint32 ZUARTRecv::ZGetBufferLen()
{
    return this->m_rxBufLenReplicated;
}
quint32 ZUARTRecv::ZTxData(const quint8 *cmd, quint32 length)
{
    return this->m_uart->write((const char*)cmd,length);
}
