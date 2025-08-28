#ifndef ZDIALOGINPUTFILEPARAMETER_H
#define ZDIALOGINPUTFILEPARAMETER_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
class ZDialogInputFileParameter : public QDialog
{
    Q_OBJECT
public:
    ZDialogInputFileParameter(QWidget *parent=nullptr);
    ~ZDialogInputFileParameter();

    bool ZDoInit();
    quint32 ZGetBypassHeadBytes();
    QString ZGetResolution();
protected:

private:
    QLabel *m_llBypass;
    QSpinBox *m_sbBypass;

    QLabel *m_llResolution;
    QRadioButton *m_rbAutoDetect;
    QRadioButton *m_rb256x192;
    QRadioButton *m_rb640x512;
    QButtonGroup *m_bgRes;
    QHBoxLayout *m_hResLayout;

    QHBoxLayout *m_hBtnLayout;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;

    QVBoxLayout *m_vLayout;
};

#endif // ZDIALOGINPUTFILEPARAMETER_H
