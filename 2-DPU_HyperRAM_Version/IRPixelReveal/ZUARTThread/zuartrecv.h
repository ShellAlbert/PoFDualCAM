#ifndef ZUARTRECV_H
#define ZUARTRECV_H

#include <QObject>
#include <QSerialPort>
#include <QImage>
#include <QVector>
class ZUARTRecv : public QObject
{
    Q_OBJECT
public:
    explicit ZUARTRecv(QObject *parent = nullptr);
    ~ZUARTRecv();
    bool ZDoInit();
    bool ZIsOpened();

    bool ZOpenUART(QString uartName);
    void ZCloseUART();
    QColor ZMapTemperature2Color(quint16 tTemp);
signals:
    void ZSignalLog(const QString &log);
    void ZSignalHexData(const QString &hexData);
    void ZSignalNewImage(const QImage &imgPixel, const QImage &imgTemperature);
    void ZSignalNewJPEG(const QImage &jpegImg);
    void ZSignalRxBytes(qint32 rxBytes);
    void ZSignalRxFrames(qint32 rxFrames);
    void ZSignalMaxMinDiffTempChanged(qint32 iMax, qint32 iMin, qint32 iDiff);
    void ZSignalRenderProgress(qint32 iProgress);
public slots:
    void ZSlotDataReady();
    void ZSlotFetchIRImageData(qint32 iRow, qint32 iCol);
    void ZSlotFetchTempImageData(qint32 iRow, qint32 iCol);
private:
    void ZParseSingleFrame_OV5640(char *data, int length);
    void ZParseSingleFrame_IR(char *data, int length);

private:
    QSerialPort *m_uart;

    //buffer to hold received data, Visible Light JPEG image or Infrared Image.
    QByteArray m_baBuffer;
    quint32 m_bufferLen;

    //OV5640 Visible Light Image. resolution: 2592*1944.
    QImage m_imgOV5640;

    //InfiRay M3256. 256*192.
    QImage m_imgIR;
    QImage m_imgTemperature;
    /////////////////////////////
    qint32 m_iRxBytes;
    qint32 m_iRxFrames;

    QVector<QVector<quint16>> m_arrayTemp;
    quint16 m_minTemp;
    quint16 m_maxTemp;
};

#endif // ZUARTRECV_H
