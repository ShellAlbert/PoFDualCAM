#ifndef ZUARTRECV_H
#define ZUARTRECV_H

#include <QObject>
#include <QSerialPort>
#include <QImage>
#include <QVector>
//buffer to hold received data, Visible Light JPEG image or Infrared Image.
//buffer size: 5MBytes.

//Infrared Image Resolution: 256*192
//One single line: 16 bytes sync header+256*2B(Pixel)+256*2B(Temperature)=1040Bytes.
//One Complete Frame: 1040Bytes*192Lines=199680.
//Extend to 200K to hold some random data to make it more flexibility.
#define RX_BUFFER_SIZE (5*1024*1024*8)
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
    ///////////////////////////////////////////////
    quint8* ZGetBufferAddress();
    quint32 ZGetBufferLen();
signals:
    void ZSignalLog(const QString &log);
    void ZSignalHexData(const QString &hexData);
    void ZSignalNewImage(const QImage &imgPixel, const QImage &imgTemperature);
    void ZSignalNewJPEG(const QImage &jpegImg);
    void ZSignalRxBytes(qint32 rxBytes);
    void ZSignalRxNewFrames(quint32 rxIRFramesCnt, quint32 rxVLFramesCnt, quint32 rxErrFramesCnt);
    void ZSignalMaxMinDiffTempChanged(qint32 iMax, qint32 iMin, qint32 iDiff);
    void ZSignalRenderProgress(qint32 iProgress);
public slots:
    void ZSlotDataReady();
    void ZSlotFetchIRImageData(qint32 iRow, qint32 iCol);
    void ZSlotFetchTempImageData(qint32 iRow, qint32 iCol);
private:
    bool ZParseSingleFrame_OV5640(quint8 *data, quint32 length);
    bool ZParseSingleFrame_IR(quint8 *data, quint32 length);

private:
    QSerialPort *m_uart;

    //buffer to hold received data, Visible Light JPEG image or Infrared Image.
    quint8 *m_rxBuffer;
    quint32 m_rxBufLen;
    quint32 m_rxBufLenReplicated;

    //OV5640 Visible Light Image. resolution: 2592*1944.
    QImage m_imgOV5640;

    //InfiRay M3256. 256*192.
    QImage m_imgIR;
    QImage m_imgTemperature;
    /////////////////////////////
    qint32 m_iRxBytes;
    //how many Infrared Frames we have received.
    quint32 m_iRxIRFrames;
    //how many Visible Light Frames we have received.
    quint32 m_iRxVLFrames;
    //how many corrupted frames we have received.
    quint32 m_iRxErrFrames;

    QVector<QVector<quint16>> m_arrayTemp;
    quint16 m_minTemp;
    quint16 m_maxTemp;

    //////////////////////////////////////
    quint32 m_frameLength;
    quint32 m_frameOffset;
};

#endif // ZUARTRECV_H
