#ifndef ZUARTLASER_H
#define ZUARTLASER_H

#include <QObject>
#include <QObject>
#include <QSerialPort>
#include <QImage>
#include <QVector>
#define LASER_RX_BUFFER_SIZE  (1024)
class ZUARTLaser : public QObject
{
    Q_OBJECT
public:
    explicit ZUARTLaser(QObject *parent = nullptr);
    ~ZUARTLaser();
    bool ZDoInit();
    bool ZIsOpened();

    bool ZOpenUART(QString uartName);
    void ZCloseUART();
    QString ZGetCfgString();
    quint32 ZTxData(const quint8 *cmd, quint32 length);
signals:
    void ZSignalLog(const QString &log);
    void ZSignalLaserON(const quint8 &on);
    void ZSignalSerialNo(const QString &sno);
    void ZSignalWorkingStatus(quint8 on_off,quint8 alarm,quint16 temp,quint16 current,quint16 outPwr,quint16 setPwr);
private slots:
    void ZSlotDataReady();
private:
    void ZParseRxBuffer(const quint8 *buffer, quint32 length);
private:
    QSerialPort *m_uart;
    quint8 *m_rxBuffer;
    quint32 m_rxBufLen;
};
#endif // ZUARTLASER_H
