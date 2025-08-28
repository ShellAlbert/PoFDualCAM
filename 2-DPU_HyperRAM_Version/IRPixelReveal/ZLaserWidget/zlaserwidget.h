#ifndef ZLASERWIDGET_H
#define ZLASERWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QGridLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QTimer>
#include "zuartlaser.h"
class ZLaserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZLaserWidget(QWidget *parent = nullptr);
    ~ZLaserWidget();
    bool ZDoInit();
private slots:
    void ZSlotLog(const QString &log);
    void ZSlotRxSerialNumber(const QString &serialNum);
    void ZSlotRxLaserON(quint8 on);
    void ZSlotRxWorkingStatus(quint8 on_off,quint8 alarm,quint16 temp,quint16 current,quint16 outPwr,quint16 setPwr);
    ///////////////////////////////////////////////
    void ZSlotConnect();
    void ZSlotLaserON();
    void ZSlotOutputPwrSetPoint();
    void ZSlotQueryBasicInfo();
    void ZSlotTimeout();
signals:

private:
    QToolButton *m_tbConnect;
    QHBoxLayout *m_hTopLayout;

    QGridLayout *m_gridLayout;

    QLabel *m_llSerialNum;
    QLineEdit *m_leSerialNum;

    QLabel *m_llLaserStatus;
    QLineEdit *m_leLaserStatus;
    QToolButton *m_btnLaserOn;

    QLabel *m_llWorkingStatus;
    QLineEdit *m_leWorkingStatus;

    QLabel *m_llOutputPwrSetPoint;
    QSpinBox *m_sbOutputPwr;
    QToolButton *m_btnSetPwr;

    QLabel *m_llStep;
    QLineEdit *m_leStep;
    ////////////////////////////////////
    QTextEdit *m_teLog;
    QHBoxLayout *m_hLayout;
    ///////////////////////////////
    QVBoxLayout *m_vLayout;
private:
    ZUARTLaser *m_uart;
    QTimer *m_timer;

    bool m_bLaserOn;
    bool m_bGetSerialNumDone;
    bool m_bGetTempCurrentPowerDone;
};

#endif // ZLASERWIDGET_H
