#include "zlaserwidget.h"
#include <QMessageBox>
ZLaserWidget::ZLaserWidget(QWidget *parent)
    : QWidget{parent}
{
    this->m_tbConnect=nullptr;
    this->m_hTopLayout=nullptr;

    this->m_gridLayout=nullptr;
    this->m_llSerialNum=nullptr;
    this->m_leSerialNum=nullptr;

    this->m_llLaserStatus=nullptr;
    this->m_leLaserStatus=nullptr;
    this->m_btnLaserOn=nullptr;

    this->m_llWorkingStatus=nullptr;
    this->m_leWorkingStatus=nullptr;

    this->m_llOutputPwrSetPoint=nullptr;
    this->m_sbOutputPwr=nullptr;
    this->m_btnSetPwr=nullptr;

    this->m_llStep=nullptr;
    this->m_leStep=nullptr;
    ////////////////////////////////////
    this->m_teLog=nullptr;
    this->m_hLayout=nullptr;
    this->m_vLayout=nullptr;
    //////////////////////////////////
    this->m_uart=nullptr;
    this->m_timer=nullptr;
}
ZLaserWidget::~ZLaserWidget()
{
    delete this->m_tbConnect;
    delete this->m_hTopLayout;

    delete this->m_llSerialNum;
    delete this->m_leSerialNum;
    delete this->m_llLaserStatus;
    delete this->m_leLaserStatus;
    delete this->m_btnLaserOn;

    delete this->m_llWorkingStatus;
    delete this->m_leWorkingStatus;

    delete this->m_llOutputPwrSetPoint;
    delete this->m_sbOutputPwr;
    delete this->m_btnSetPwr;

    delete this->m_llStep;
    delete this->m_leStep;
    delete this->m_gridLayout;
    ////////////////////////////////////
    delete this->m_teLog;
    delete this->m_hLayout;
    delete this->m_vLayout;
    delete this->m_uart;

    delete this->m_timer;
}
bool ZLaserWidget::ZDoInit()
{
    this->m_tbConnect=new QToolButton;
    if(nullptr==this->m_tbConnect)
    {
        return false;
    }
    this->m_tbConnect->setText(tr("Connect"));
    connect(this->m_tbConnect,SIGNAL(clicked(bool)),this,SLOT(ZSlotConnect()));

    this->m_hTopLayout=new QHBoxLayout;
    this->m_hTopLayout->addStretch(1);
    this->m_hTopLayout->addWidget(this->m_tbConnect);
    /////////////////////////////////////////////////////////////////////////
    this->m_llSerialNum=new QLabel(tr("Serial Number"));
    if(nullptr==this->m_llSerialNum)
    {
        return false;
    }
    this->m_leSerialNum=new QLineEdit;
    if(nullptr==this->m_leSerialNum)
    {
        return false;
    }
    this->m_leSerialNum->setEnabled(false);

    this->m_llLaserStatus=new QLabel(tr("Laser Status"));
    if(nullptr==this->m_llLaserStatus)
    {
        return false;
    }
    this->m_leLaserStatus=new QLineEdit;
    if(nullptr==this->m_leLaserStatus)
    {
        return false;
    }
    this->m_leLaserStatus->setEnabled(false);
    this->m_btnLaserOn=new QToolButton;
    if(nullptr==this->m_btnLaserOn)
    {
        return false;
    }
    this->m_btnLaserOn->setText(tr("Laser ON"));
    connect(this->m_btnLaserOn,SIGNAL(clicked(bool)),this,SLOT(ZSlotLaserON()));
    //////////////////////////////////////////////////////////////////
    this->m_llWorkingStatus=new QLabel(tr("Working Status"));
    if(nullptr==this->m_llWorkingStatus)
    {
        return false;
    }
    this->m_leWorkingStatus=new QLineEdit;
    if(nullptr==this->m_leWorkingStatus)
    {
        return false;
    }
    //////////////////////////////////////////////////////////////////
    this->m_llOutputPwrSetPoint=new QLabel(tr("Output Power SetPoint"));
    if(nullptr==this->m_llOutputPwrSetPoint)
    {
        return false;
    }
    this->m_sbOutputPwr=new QSpinBox;
    if(nullptr==this->m_sbOutputPwr)
    {
        return false;
    }
    this->m_sbOutputPwr->setValue(0);
    this->m_btnSetPwr=new QToolButton;
    this->m_btnSetPwr->setText(tr("Set"));
    if(nullptr==this->m_btnSetPwr)
    {
        return false;
    }
    connect(this->m_btnSetPwr,SIGNAL(clicked(bool)),this,SLOT(ZSlotOutputPwrSetPoint()));
    /////////////////////////////////////////////
    this->m_llStep=new QLabel(tr("Step"));
    if(nullptr==this->m_llStep)
    {
        return false;
    }
    this->m_leStep=new QLineEdit;
    if(nullptr==this->m_leStep)
    {
        return false;
    }
    this->m_gridLayout=new QGridLayout;
    this->m_gridLayout->addWidget(this->m_llSerialNum,0,0);
    this->m_gridLayout->addWidget(this->m_leSerialNum,0,1,1,2);
    /////////////////////////////////////////////////////////
    this->m_gridLayout->addWidget(this->m_llLaserStatus,1,0);
    this->m_gridLayout->addWidget(this->m_leLaserStatus,1,1);
    this->m_gridLayout->addWidget(this->m_btnLaserOn,1,2);
    ////////////////////////////////////////////////////////////////
    this->m_gridLayout->addWidget(this->m_llWorkingStatus,2,0);
    this->m_gridLayout->addWidget(this->m_leWorkingStatus,2,1,1,2);
    /////////////////////////////////////////////////////////////
    this->m_gridLayout->addWidget(this->m_llOutputPwrSetPoint,3,0);
    this->m_gridLayout->addWidget(this->m_sbOutputPwr,3,1);
    this->m_gridLayout->addWidget(this->m_btnSetPwr,3,2);
    //////////////////////////////////////////////////////////
    this->m_gridLayout->addWidget(this->m_llStep,4,0);
    this->m_gridLayout->addWidget(this->m_leStep,4,1);
    /////////////////////////////////////////////////////////////
    this->m_teLog=new QTextEdit;
    if(nullptr==this->m_teLog)
    {
        return false;
    }
    this->m_hLayout=new QHBoxLayout;
    if(nullptr==this->m_hLayout)
    {
        return false;
    }
    this->m_hLayout->addLayout(this->m_gridLayout);
    this->m_hLayout->addWidget(this->m_teLog);
    //////////////////////////////////////////////
    this->m_vLayout=new QVBoxLayout;
    if(nullptr==this->m_vLayout)
    {
        return false;
    }
    this->m_vLayout->addLayout(this->m_hTopLayout);
    this->m_vLayout->addLayout(this->m_hLayout);
    this->setLayout(this->m_vLayout);

    this->m_timer=new QTimer(this);
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(ZSlotTimeout()));

    //reset flags and counters.
    this->m_bLaserOn=false;
    this->m_bGetSerialNumDone=false;
    this->m_bGetTempCurrentPowerDone=false;
    return true;
}
void ZLaserWidget::ZSlotLog(const QString &log)
{
    this->m_teLog->append(log);
}
void ZLaserWidget::ZSlotRxSerialNumber(const QString &serialNum)
{
    this->m_leSerialNum->setText(serialNum);
    this->m_bGetSerialNumDone=true;
}
void ZLaserWidget::ZSlotRxLaserON(quint8 on)
{
    this->m_leLaserStatus->setText((on)?tr("ON"):tr("OFF"));
    this->m_bLaserOn=(on)?(true):(false);
}
void ZLaserWidget::ZSlotRxWorkingStatus(quint8 on_off,quint8 alarm,quint16 temp,quint16 current,quint16 outPwr,quint16 setPwr)
{
    QString strON(tr("1.Working Status="));
    strON+=(on_off)?tr("ON"):tr("OFF");
    QString strAlarm;
    if(!alarm)
    {
        strAlarm=tr("2.No Alarm, Working Normally.");
    }else{
        if(alarm&(0x1<<0))
        {
            strAlarm+=tr("Temperature Alarm,");
        }
        if(alarm&(0x1<<1))
        {
            strAlarm+=tr("Seed Laser Temperature Alarm,");
        }
        if(alarm&(0x1<<2))
        {
            strAlarm+=tr("Input Power Alarm,");
        }
        if(alarm&(0x1<<3))
        {
            strAlarm+=tr("Pump Temperature Alarm,");
        }
        if(alarm&(0x1<<4))
        {
            strAlarm+=tr("Pump Power Alarm,");
        }
        if(alarm&(0x1<<5))
        {
            strAlarm+=tr("Seed Laser Power Alarm,");
        }
        if(alarm&(0x1<<7))
        {
            strAlarm+=tr("Save Data Error");
        }
    }
    QString strTemp=QString::asprintf("Temperature:%.2f ℃",(temp-27315)/100.0);
    QString strCurrent=QString::asprintf("Current:%d mA",current);
    QString strOutPwr=QString::asprintf("Out Power:%d mW",outPwr);
    QString strSetPwr=QString::asprintf("Set Power:%d mW",setPwr);
    this->m_teLog->clear();
    this->m_teLog->append(strON);
    this->m_teLog->append(strAlarm);
    this->m_teLog->append(strTemp);
    this->m_teLog->append(strCurrent);
    this->m_teLog->append(strOutPwr);
    this->m_teLog->append(strSetPwr);
}
void ZLaserWidget::ZSlotConnect()
{
    if(nullptr==this->m_uart)
    {
        this->m_uart=new ZUARTLaser;
        if(nullptr==this->m_uart)
        {
            QMessageBox::critical(this,tr("Error Message"),tr("Create thread for UART."));
            return;
        }
        connect(this->m_uart,SIGNAL(ZSignalLog(QString)),this,SLOT(ZSlotLog(QString)));
        connect(this->m_uart,SIGNAL(ZSignalSerialNo(QString)),this,SLOT(ZSlotRxSerialNumber(QString)));
        connect(this->m_uart,SIGNAL(ZSignalLaserON(quint8)),this,SLOT(ZSlotRxLaserON(quint8)));
        connect(this->m_uart,SIGNAL(ZSignalWorkingStatus(quint8,quint8,quint16,quint16,quint16,quint16)),this,SLOT(ZSlotRxWorkingStatus(quint8,quint8,quint16,quint16,quint16,quint16)));
        if(!this->m_uart->ZDoInit())
        {
            QMessageBox::critical(this,tr("Error Message"),tr("Initialize UART failed."));
            return;
        }
    }
    if(!this->m_uart->ZIsOpened())
    {
        if(this->m_uart->ZOpenUART("/dev/ttyUSB0")) ///dev/ttyS0.
        {
            this->m_tbConnect->setText("Disconnect");
            this->ZSlotLog(this->m_uart->ZGetCfgString());
        }
    }else {
        this->m_uart->ZCloseUART();
        this->m_tbConnect->setText("Connect");
    }
}
void ZLaserWidget::ZSlotLaserON()
{
    if(nullptr==this->m_uart)
    {
        return;
    }
    if(!this->m_bLaserOn)
    {
        //Step-1: Laser ON.
        //T | AA 55 C1 01 00 00 00
        //R | 55 AA C1 01 00 00 00
        const uchar cmdLaserOn[]={0xAA,0x55,0xC1,0x01,0x0,0x0,0x0};
        this->m_uart->ZTxData(cmdLaserOn,sizeof(cmdLaserOn));

        //Step-2: Set Output Power to 0mW.
        //T | AA 55 C3 00 00 00
        //R | 55 AA C3 02 00 00 00 00
        const uchar cmdSetPwr0[]={0xAA,0x55,0xC3,0x0,0x0,0x0};
        this->m_uart->ZTxData(cmdSetPwr0,sizeof(cmdSetPwr0));

        //start Query timer.
        this->m_timer->start(1000);
    }
}
void ZLaserWidget::ZSlotOutputPwrSetPoint()
{
    //Set output power to 0mW.
    //T | AA 55 C3 00 00 00
    //R | 55 AA C3 02 00 00 00 00
    const uchar cmdPwr0[]={0xAA,0x55,0xC3,0x0,0x0,0x0};

}
void ZLaserWidget::ZSlotQueryBasicInfo()
{
    if(nullptr==this->m_uart)
    {
        return;
    }



    //Read back realtime information.
    //T | AA 55 D5 00 00 00
    //R | 55 AA D5 05 00 00 00 00 00 00 00
    const uchar cmdD5[]={0xAA,0x55,0xD5,0x0,0x0,0x0};


}
void ZLaserWidget::ZSlotTimeout()
{
    if(!this->m_bGetSerialNumDone)
    {
        //Read back basic information.
        //T | AA 55 D1 00 00 00
        //R | 55 AA D1 24 00 03 01 01 01 01 01 00 02 00 02 04 00 07 01 07 08 00 E8 03 01 64 00 0A 00 00 00 00 00 00 00 00 8F 70 5F 78 57 03
        const uchar cmdD1[]={0xAA,0x55,0xD1,0x0,0x0,0x0};
        this->m_uart->ZTxData(cmdD1,sizeof(cmdD1));
    }

    if(0/*!m_bGetTempCurrentPowerDone*/)
    {
        //Read back realtime temperature & current & power information.
        //T | AA 55 D2 00 00 00
        //R | 55 AA D2 05 00 1F 21 11 22 73 00
        const uchar cmdD2[]={0xAA,0x55,0xD2,0x0,0x0,0x0};
        this->m_uart->ZTxData(cmdD2,sizeof(cmdD2));
    }

    if(1)
    {
        //T | AA 55 D3 00 00 00
        //R | 55 AA D3 0A 00 00 60 75 00 00 55 73 00 00 9D 01
        const uchar cmdD3[]={0xAA,0x55,0xD3,0x00,0x00,0x00};
        this->m_uart->ZTxData(cmdD3,sizeof(cmdD3));
    }
}
