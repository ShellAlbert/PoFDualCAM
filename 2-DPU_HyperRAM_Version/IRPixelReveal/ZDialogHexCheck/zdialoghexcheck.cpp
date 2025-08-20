#include "zdialoghexcheck.h"

ZDialogHexCheck::ZDialogHexCheck()
{
    this->m_te=nullptr;
    this->m_vLayout=nullptr;
    this->setWindowTitle((tr("Hex Display")));
}
ZDialogHexCheck::~ZDialogHexCheck()
{
    delete this->m_te;
    delete this->m_vLayout;
}
QSize ZDialogHexCheck::sizeHint() const
{
    return QSize(800,600);
}

bool ZDialogHexCheck::ZDoInit()
{
    this->m_te=new QTextEdit;
    if(nullptr==this->m_te)
    {
        return false;
    }
    this->m_te->setReadOnly(true);
    this->m_te->setFont(QFont("SimHei",16));
    this->m_vLayout=new QVBoxLayout;
    if(nullptr==this->m_vLayout)
    {
        return false;
    }
    this->m_vLayout->addWidget(this->m_te);
    this->setLayout(this->m_vLayout);
    return true;
}
void ZDialogHexCheck::ZAppendText(const QString &text)
{
    this->m_te->append(text);
}
void ZDialogHexCheck::ZClearText(void)
{
    this->m_te->clear();
}
