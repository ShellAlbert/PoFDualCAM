#include "zdialogconfigdev.h"

ZDialogConfigDev::ZDialogConfigDev(QWidget *parent) : QDialog(parent)
{
  this->m_gridLayout=NULL;
  this->m_llCOM=NULL;
  this->m_cbCOM=NULL;

  this->m_llBPS=NULL;
  this->m_cbBPS=NULL;

  this->m_llDatabits=NULL;
  this->m_cbDatabits=NULL;

  this->m_llStopbits=NULL;
  this->m_cbStopbits=NULL;

  this->m_llParity=NULL;
  this->m_cbParity=NULL;

  this->m_hLayout=NULL;
  this->m_btnOkay=NULL;
  this->m_btnCancel=NULL;

  this->m_vLayout=NULL;
}
ZDialogConfigDev::~ZDialogConfigDev()
{
  delete this->m_llCOM;
  delete this->m_cbCOM;

  delete this->m_llBPS;
  delete this->m_cbBPS;

  delete this->m_llDatabits;
  delete this->m_cbDatabits;

  delete this->m_llStopbits;
  delete this->m_cbStopbits;

  delete this->m_llParity;
  delete this->m_cbParity;
  delete this->m_gridLayout;

  delete this->m_btnOkay;
  delete this->m_btnCancel;
  delete this->m_hLayout;

  delete this->m_vLayout;
}
bool ZDialogConfigDev::ZDoInit()
{

  this->m_llCOM=new QLabel(tr("UART PORT"));
  this->m_cbCOM=new QComboBox;

  this->m_llBPS=new QLabel(tr("Baudrate"));
  this->m_cbBPS=new QComboBox;

  this->m_llDatabits=new QLabel(tr("Data Bits"));
  this->m_cbDatabits=new QComboBox;

  this->m_llStopbits=new QLabel(tr("Stop Bits"));
  this->m_cbStopbits=new QComboBox;

  this->m_llParity=new QLabel(tr("Parity"));
  this->m_cbParity=new QComboBox;

  this->m_gridLayout=new QGridLayout;
  this->m_gridLayout->addWidget(this->m_llCOM,0,0);
  this->m_gridLayout->addWidget(this->m_cbCOM,0,1);
  this->m_gridLayout->addWidget(this->m_llBPS,1,0);
  this->m_gridLayout->addWidget(this->m_cbBPS,1,1);
  this->m_gridLayout->addWidget(this->m_llDatabits,2,0);
  this->m_gridLayout->addWidget(this->m_cbDatabits,2,1);
  this->m_gridLayout->addWidget(this->m_llStopbits,3,0);
  this->m_gridLayout->addWidget(this->m_cbStopbits,3,1);
  this->m_gridLayout->addWidget(this->m_llParity,4,0);
  this->m_gridLayout->addWidget(this->m_cbParity,4,1);

  this->m_btnOkay=new QToolButton;
  this->m_btnOkay->setText(tr("OKAY"));
  this->m_btnCancel=new QToolButton;
  this->m_btnCancel->setText(tr("CANCEL"));
  this->m_hLayout=new QHBoxLayout;
  this->m_hLayout->addStretch(1);
  this->m_hLayout->addWidget(this->m_btnOkay);
  this->m_hLayout->addWidget(this->m_btnCancel);

  this->m_vLayout=new QVBoxLayout;
  this->m_vLayout->addLayout(this->m_gridLayout);
  this->m_vLayout->addLayout(this->m_hLayout);
  this->setLayout(this->m_vLayout);

  connect(this->m_btnOkay,SIGNAL(clicked(bool)),this,SLOT(ZSLotOkay()));
  connect(this->m_btnCancel,SIGNAL(clicked(bool)),this,SLOT(ZSlotCancel()));
  return true;
}
QSize ZDialogConfigDev::sizeHint() const
{
    return QSize(400,200);
}
void ZDialogConfigDev::ZSLotOkay()
{
  this->accept();
}
void ZDialogConfigDev::ZSlotCancel()
{
  this->reject();
}
