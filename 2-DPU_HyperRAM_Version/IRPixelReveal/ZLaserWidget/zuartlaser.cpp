#include "zuartlaser.h"

ZUARTLaser::ZUARTLaser(QObject *parent):QObject(parent)
{
    this->m_uart=nullptr;
    this->m_rxBuffer=nullptr;
    this->m_rxBufLen=0;
}

ZUARTLaser::~ZUARTLaser()
{
    delete [] this->m_rxBuffer;
    delete this->m_uart;
}
bool ZUARTLaser::ZDoInit()
{
    this->m_rxBuffer=new quint8[LASER_RX_BUFFER_SIZE];
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
    this->m_uart->setPortName("ttyS0");
    this->m_uart->setBaudRate(9600); //bps.
    this->m_uart->setDataBits(QSerialPort::Data8);
    this->m_uart->setStopBits(QSerialPort::OneStop);
    this->m_uart->setParity(QSerialPort::NoParity);
    this->m_uart->setFlowControl(QSerialPort::NoFlowControl);
    connect(this->m_uart,SIGNAL(readyRead()),this,SLOT(ZSlotDataReady()));
    ////////////////////////////////////////////////////////////////////////
    this->m_rxBufLen=0;
    return true;
}
bool ZUARTLaser::ZIsOpened()
{
    if(nullptr==this->m_uart)
    {
        return false;
    }
    return this->m_uart->isOpen();
}

bool ZUARTLaser::ZOpenUART(QString uartName)
{
    this->m_uart->setPortName(uartName);
    if(!this->m_uart->open(QIODevice::ReadWrite))
    {
        emit this->ZSignalLog(this->m_uart->errorString());
        return false;
    }
    this->m_rxBufLen=0;
    return true;
}
void ZUARTLaser::ZCloseUART()
{
    if(nullptr==this->m_uart)
    {
        return;
    }
    this->m_uart->close();
}
QString ZUARTLaser::ZGetCfgString()
{
    return this->m_uart->portName()+",9600 8N1.";
}
quint32 ZUARTLaser::ZTxData(const quint8 *cmd, quint32 length)
{
    return this->m_uart->write((const char*)cmd,length);
}
void ZUARTLaser::ZSlotDataReady()
{
    qint64 iByteCanRead=this->m_uart->bytesAvailable();
    if(iByteCanRead<=0)
    {
        return;
    }
    if((LASER_RX_BUFFER_SIZE-this->m_rxBufLen)>=iByteCanRead)
    {
        qint64 iReadActually=this->m_uart->read((char*)(this->m_rxBuffer+this->m_rxBufLen),iByteCanRead);
        if(iReadActually<=0)
        {
            return;
        }
        this->m_rxBufLen+=iReadActually;
        this->ZParseRxBuffer(this->m_rxBuffer,this->m_rxBufLen);
    }else{
        emit this->ZSignalLog(tr("Rx Buffer overflow, reset."));
        this->m_rxBufLen=0;
    }
}
void ZUARTLaser::ZParseRxBuffer(const quint8 *buffer, quint32 length)
{
    for(quint32 i=0;i<length;i++)
    {
        qDebug("%02x ",buffer[i]);
    }

    if(nullptr==buffer || 0==length)
    {
        return;
    }

    //searching sync head bytes.
    qint32 iSyncHeadOffset=-1;
    for(quint32 i=0;i<length-2;i++)
    {
        if(0x55==buffer[i] && 0xaa==buffer[i+1])
        {
            iSyncHeadOffset=i;
            break;
        }
    }

    if(iSyncHeadOffset<0)
    {
        emit this->ZSignalLog(tr("Searching sync head bytes failed,reset."));
        this->m_rxBufLen=0;
        return;
    }

    quint32 i;
    for(i=iSyncHeadOffset;i<length-5;i++)
    {
        QString strSerialNo;
        uchar sync0=buffer[i+0];
        uchar sync1=buffer[i+1];
        uchar command=buffer[i+2];
        uchar frameLength=buffer[i+3];
        if((i+2+1+frameLength+2)>length) //avoid overflow.
        {
            return;
        }
        switch(command)
        {
        case 0xc1: //Laser On feedback.
            //55 aa c1 01 00 00 00
            emit this->ZSignalLaserON((0x1==buffer[i+4])?(1):(0));
            //sync head bytes(2)+command(1)+frame length(1)+frame length+check sum(2).
            //i+=7; //jump to next frame.
            i+=(2)+(1)+(1)+frameLength+(2); //jump to next frame.
            break;
        case 0xd1:
            //55 aa :[0-1], sync head bytes. fixed value.
            //d1 : [2]: command.
            //24 : [3] frame length, 24(hex)=36(decimal)
            //00 03 01 : [4-6] project number.003
            //01 01 : [7-8] hardware version.1.1
            //01 01 00 : [9-11] software number.1.01
            //02 00 02 04 00 07 01 07 08 : [12-20],serial number,format 201701001.
            //00 :[21], output power unit, 0-mW,1-W,2:uW.
            //e8 :[22], output power maximum (low bytes).
            //03 :[23], output power maximum (high bytes). 03e8(hex)=1000(decimal).
            //01 :[24], power setting method,01-set power.02-set current.03-set DA,04-resistor......
            //64 :[25}: temperature coffeicient.
            //00 0a 00 00 00 00 00 00 00 :[26-34]
            //00 :[35], product category. 0-Laser.
            //8f 70 5f 78 :[36-39], temperature seeds.
            //57 03 : [40-41], checksum, 2 bytes.
            strSerialNo=QString::asprintf("%d%d%d%d%d%d%d%d%d",///<
                                                    buffer[i+12],buffer[i+13], buffer[i+14],buffer[i+15],///<
                                                    buffer[i+16],buffer[i+17],///<
                                                    buffer[i+18],buffer[i+19],buffer[i+20]);
            emit this->ZSignalSerialNo(strSerialNo);
            //sync head bytes(2)+command(1)+frame length(1)+frame length+check sum(2).
            //i+=42; //jump to next frame. 3+1+36+2=42.
            i+=(2)+(1)+(1)+frameLength+(2); //jump to next frame.
            break;
        case 0xd2:
            //55 aa d2 05 00 1f 21 11 22 73 00
            break;
        case 0xd3:
            //55 aa d3 0a 00 00 66 76 00 00 6b 73 00 00 ba 01
            emit this->ZSignalWorkingStatus( ///<
                buffer[i+4], //<[4], product working status.
                buffer[i+5], //<[5], alarm bits.
                (buffer[i+7]<<8)|buffer[i+6], //<temperature.
                (buffer[i+9]<<8)|buffer[i+8], //<current.
                (buffer[i+11]<<8)|buffer[i+10], //<output power.
                (buffer[i+13]<<8)|buffer[i+12]); //<set power.
            //i+=16; //jump to next frame. 3+1+10+2=16.
            i+=(2)+(1)+(1)+frameLength+(2); //jump to next frame.
            break;
        default:
            qDebug("unknow command %02x,bypass it!\n",command);
            i+=(2)+(1)+(1)+frameLength+(2); //jump to next frame.
            break;
        }
    }
    if(i!=(length-1))
    {
        memmove(this->m_rxBuffer,this->m_rxBuffer+i,length-i+1);
        this->m_rxBufLen=length-i+1;
    }else{
        this->m_rxBufLen=0;
    }

}
