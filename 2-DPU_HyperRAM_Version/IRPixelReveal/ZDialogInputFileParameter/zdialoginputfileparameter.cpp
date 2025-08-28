#include "zdialoginputfileparameter.h"

ZDialogInputFileParameter::ZDialogInputFileParameter(QWidget *parent):QDialog(parent)
{
    this->m_llBypass=nullptr;
    this->m_sbBypass=nullptr;

    this->m_llResolution=nullptr;
    this->m_rbAutoDetect=nullptr;
    this->m_rb256x192=nullptr;
    this->m_rb640x512=nullptr;
    this->m_bgRes=nullptr;
    this->m_hResLayout=nullptr;

    this->m_hBtnLayout=nullptr;
    this->m_tbOkay=nullptr;
    this->m_tbCancel=nullptr;
    this->m_vLayout=nullptr;

    this->setWindowTitle(tr("Input File Parameter Confirmation"));
}
ZDialogInputFileParameter::~ZDialogInputFileParameter()
{
    delete this->m_llBypass;
    delete this->m_sbBypass;

    delete this->m_llResolution;
    delete this->m_rbAutoDetect;
    delete this->m_rb256x192;
    delete this->m_rb640x512;
    delete this->m_bgRes;
    delete this->m_hResLayout;

    delete this->m_tbOkay;
    delete this->m_tbCancel;
    delete this->m_hBtnLayout;
    delete this->m_vLayout;
}
bool ZDialogInputFileParameter::ZDoInit()
{
    this->m_llBypass=new QLabel(tr("How many head bytes to bypass?\n\nSpecific bytes from head will be eliminated before rendering.\n\nDefault is 20."));
    if(nullptr==this->m_llBypass)
    {
        return false;
    }
    this->m_sbBypass=new QSpinBox;
    if(nullptr==this->m_sbBypass)
    {
        return false;
    }
    this->m_sbBypass->setValue(20);
    this->m_sbBypass->setRange(0,100);

    this->m_llResolution=new QLabel(tr("\n\nRAW Data File Resolution\n\nCurrently, only Yantai InfiRay C256x192 and Lite640x512 are supported.\n\nAuto Detect needs protocol supporting.\n\n"));
    this->m_rbAutoDetect=new QRadioButton(tr("Auto Detect"));
    this->m_rbAutoDetect->setChecked(true);
    this->m_rb256x192=new QRadioButton(tr("256*192"));
    this->m_rb640x512=new QRadioButton(tr("640x512"));
    this->m_bgRes=new QButtonGroup;
    this->m_bgRes->addButton(this->m_rbAutoDetect);
    this->m_bgRes->addButton(this->m_rb256x192);
    this->m_bgRes->addButton(this->m_rb640x512);
    this->m_hResLayout=new QHBoxLayout;
    this->m_hResLayout->addWidget(this->m_rbAutoDetect);
    this->m_hResLayout->addWidget(this->m_rb256x192);
    this->m_hResLayout->addWidget(this->m_rb640x512);

    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("OKAY"));
    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(accept()));

    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("CANCEL"));
    connect(this->m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));

    this->m_hBtnLayout=new QHBoxLayout;
    this->m_hBtnLayout->addStretch(1);
    this->m_hBtnLayout->addWidget(this->m_tbOkay);
    this->m_hBtnLayout->addWidget(this->m_tbCancel);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_llBypass);
    this->m_vLayout->addWidget(this->m_sbBypass);
    this->m_vLayout->addWidget(this->m_llResolution);
    this->m_vLayout->addLayout(this->m_hResLayout);
    this->m_vLayout->addLayout(this->m_hBtnLayout);
    this->setLayout(this->m_vLayout);
    return true;
}
quint32 ZDialogInputFileParameter::ZGetBypassHeadBytes()
{
    return this->m_sbBypass->value();
}
QString ZDialogInputFileParameter::ZGetResolution()
{
    if(this->m_rb256x192->isChecked())
    {
        return QString("256x192");
    }else if(this->m_rb640x512->isChecked())
    {
        return QString("640x512");
    }else{
        return QString("AutoDetect");
    }
}

