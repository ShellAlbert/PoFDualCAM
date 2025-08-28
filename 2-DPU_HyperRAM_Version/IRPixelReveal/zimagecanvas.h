#ifndef ZIMAGECANVAS_H
#define ZIMAGECANVAS_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QByteArray>
#include <QList>
#include <QMenu>
#include <QAction>
class ZImageCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit ZImageCanvas(QWidget *parent = nullptr);
    ~ZImageCanvas();

    void ZRedrwFile(const QString &fileName, int nBypassBytes);
    bool ZSaveFile(const QString &fileName);
    void ZRedrwFile640x512(const QString &fileName, int nBypassBytes);
signals:
    void ZSignalLog(const QString &log);
    void ZSignalHexData(const QString &HexData);
    void ZSignalInfraredImagePositionChanged(qint32 iRow, qint32 iCol);
    void ZSignalTemperatureImagePositionChanged(qint32 iRow, qint32 iCol);
public slots:
    void ZSlotUpdateImg(const QImage &img_Pixel, const QImage &img_Temperature);
    void ZSlotGrayHexData();
    void ZSlotExportGray2ImgFile();
    void ZSlotTemperatureHexData();
    void ZSlotTemperatureBarChart();
    void ZSlotExportTemperature2ImgFile();
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QColor ZMapTemperature2Color(float tTemp);
private:
    QImage m_img;
    QImage m_imgTemp;

    QPoint m_PosIR;
    QPoint m_PosTemp;
    //for C256*192 module of temperature data, each line is 256*2 bytes, 192 lines in total.
    //for Lite640*512 module of temperature data, each line is 640*2 bytes, 512 lines in total.
    //temperature is 16-bits, it takes up 2 bytes!!!!
    float m_ArrayTemp[512][640];

    //Context Menu.
    QMenu *m_menuGray;
    QAction *m_actGrayHexData;
    QAction *m_actExportGray2File;

    QMenu *m_menuTemp;
    QAction *m_actTempHexData;
    QAction *m_actTempBarChart;
    QAction *m_actExportTemp2File;
};

#endif // ZIMAGECANVAS_H
