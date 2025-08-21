#include "zuartrecv.h"
#include <QImage>
#include <QDebug>
ZUARTRecv::ZUARTRecv(QObject *parent) : QObject(parent)
{
    this->m_uart=nullptr;
}
ZUARTRecv::~ZUARTRecv()
{
    delete this->m_uart;
}
bool ZUARTRecv::ZDoInit()
{
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
    emit this->ZSignalLog(this->m_uart->portName()+",4Mbps/8N1");

    //buffer to hold received data, Visible Light JPEG image or Infrared Image.
    //buffer size: 5MBytes.

    //Infrared Image Resolution: 256*192
    //One single line: 16 bytes sync header+256*2B(Pixel)+256*2B(Temperature)=1040Bytes.
    //One Complete Frame: 1040Bytes*192Lines=199680.
    //Extend to 200K to hold some random data to make it more flexibility.
    this->m_baBuffer.resize(5*1024*1024); //5MBytes.
    this->m_bufferLen=0;

    //reset counters.
    this->m_iRxBytes=0;
    this->m_iRxFrames=0;
    return true;
}
void ZUARTRecv::ZCloseUART()
{
    if(nullptr==this->m_uart)
    {
        return;
    }
    this->m_uart->close();
    this->m_baBuffer.resize(0);
    this->m_bufferLen=0;
    this->m_iRxFrames=0;
    this->m_iRxBytes=0;
}
void ZUARTRecv::ZSlotDataReady()
{
    //check if we have adequate space to hold all available bytes.
    qint32 iSpaceRemaing=this->m_baBuffer.size()-this->m_bufferLen;
    qint32 iRdMax=this->m_uart->bytesAvailable();
    if(iSpaceRemaing>=iRdMax)
    {
        qint32 iRdBytes=this->m_uart->read(this->m_baBuffer.data()+this->m_bufferLen,iRdMax);
        if(iRdBytes<0)
        {
            emit this->ZSignalLog(this->m_uart->errorString());
        }else{
            this->m_bufferLen+=iRdBytes;
            emit this->ZSignalRxBytes(this->m_bufferLen);
        }
    }else{
        emit this->ZSignalLog("Rx Buffer overflow, reset.");
        this->m_bufferLen=0;
        emit this->ZSignalRxBytes(this->m_bufferLen);
        return;
    }

    //Minimum data length check.
    //Normally, for OV5640, resolution is 2596*1944. data size is greater than 1MBytes.
    //For Infrared Image Sensor, resolution is 256*192.
    //One Single Line is 16 sync header bytes+256*2Bytes(Pixel)+256*2Bytes(Temperature)=1040Bytes
    //Total bytes is 192Lines*1040=199680
    if(this->m_bufferLen<199680)
    {
        return; //no adequate data, needs more data, maybe process next time.
    }

    //The first 20-bytes are sync frame header bytes.
    //searching Sync Header.
    qint32 iFrameStartOffset=0;
    char iFrameType=0x0; //0x0:Invaild Frame, 0x1:OV5640 Visible Light Image, 0x2:Infrared Image, 0x3: Ultraviolet Image.
    while(iFrameStartOffset<(this->m_bufferLen-20))
    {
        char *p=this->m_baBuffer.data()+iFrameStartOffset;
        if(p[4]==0x01 && p[5]==0x47 && p[6]==0x33 && p[7]==0x83 && p[8]==0x15 && p[9]==0x0e && p[10]==0x76)
        {
            iFrameType=0x1; //0x1:OV5640 Visible Light Image.
            break;
        }else if(p[4]==0x56 && p[5]==0x40 && p[9]==0x0e && p[10]==0x76)
        {
            iFrameType=0x2; //0x2:Infrared Image.
            break;
        }else{
            iFrameStartOffset++;
        }
    }
    //check searching result.
    char *pHead=this->m_baBuffer.data()+iFrameStartOffset;
    char *pData=this->m_baBuffer.data()+iFrameStartOffset+20;
    quint32 frameLength=pHead[0]<<24|pHead[1]<<16|pHead[2]<<8|pHead[3]<<0;
    QByteArray baSensorID(pHead+4,5);
    QByteArray baHyperRAMID(pHead+9,2);
    QByteArray baReserved(pHead+11,9);
    emit this->ZSignalLog(tr("Image Sensor ID: ")+QString::asprintf("%02X %02X %02X %02X %02X",///<
                                                                      baSensorID.at(0),baSensorID[1],baSensorID[2],baSensorID[3],baSensorID[4]));
    emit this->ZSignalLog(tr("Hyper RAM ID: ")+QString::asprintf("%02X %02X",baHyperRAMID[0],baHyperRAMID[1]));
    switch(iFrameType)
    {
    case 0x0: //0x0:Invaild Frame.
        emit this->ZSignalLog("No frame head bytes found, reset.");
        break;
    case 0x1: //0x1:OV5640 Visible Light Image.
        if((this->m_bufferLen-iFrameStartOffset)<frameLength)
        {
            emit this->ZSignalLog("OV5640 Frame Found, but more data needed.");
            return;
        }
        //Sync Head Bytes is 20 bytes. CRC32 is 4 bytes.
        this->ZParseSingleFrame_OV5640(pData,this->m_bufferLen-iFrameStartOffset-20-4);
    case 0x2: //0x2:Infrared Image.
        if((this->m_bufferLen-iFrameStartOffset)<frameLength)
        {
            emit this->ZSignalLog("Infrared Frame Found, but more data needed.");
            return;
        }
        //Sync Head Bytes is 20 bytes. CRC32 is 4 bytes.
        this->ZParseSingleFrame_IR(pData,this->m_bufferLen-iFrameStartOffset-20-4);
    default:
        emit this->ZSignalLog("No frame head bytes found, reset.");
        break;
    }

    //after processing, reset Rx Buffer.
    this->m_bufferLen=0;
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
void ZUARTRecv::ZParseSingleFrame_IR(char *data, int length)
{
    //check length.
    //1040-Bytes*192-Lines.
    if(length!=(1040*192))
    {
        emit this->ZSignalLog(tr("Error, Render Infrared Image, but data length does not match!"));
        return;
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
        emit this->ZSignalLog(tr("Error, Frame Head Does Not Match!"));
        return;
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
    this->m_iRxFrames++;
    emit this->ZSignalLog(QString("Received one frame done:%1, MaxTemp=%2, MinTemp=%3, Diff=%4.") ///<
                              .arg(this->m_iRxFrames).arg(this->m_maxTemp).arg(this->m_minTemp).arg(this->m_maxTemp-this->m_minTemp));
    emit this->ZSignalRxFrames(this->m_iRxFrames);
    emit this->ZSignalLog("Reset Buffer.");
}
void ZUARTRecv::ZParseSingleFrame_OV5640(char *data, int length)
{
    //check JPEG head & tail fixed bytes.
    if(data[0]!=0xFF || data[1]!=0xD8 || data[length-2]!=0xFF || data[length-1]!=0xD9)
    {
        emit this->ZSignalLog(tr("Invalid JPEG Head/Tail, Expected:FF D8 FF D9, Actually:")+QString::asprintf("%02X %02X %02X %02X",data[0],data[1],data[length-2],data[length-1]));
        return;
    }
    if(this->m_imgOV5640.loadFromData((const uchar*)data,length,"JPG"))
    {
        emit this->ZSignalNewJPEG(this->m_imgOV5640);
    }else{
        emit this->ZSignalLog(tr("Failed to load jpeg from data."));
    }
}
