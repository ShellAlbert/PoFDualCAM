#include "zimagecanvas.h"
#include <QFile>
#include <QPainter>
#include <QDebug>
#include "math.h"
#include "zmainwidget.h"
#include "zglobal.h"
#include <QToolTip>
#include "ZDialogHexCheck/zdialoghexcheck.h"
#include "ZDialogTempChart/zdialogtempchart.h"
ZImageCanvas::ZImageCanvas(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    this->m_img=QImage(256,192,QImage::Format_RGB888);
    this->m_imgTemp=QImage(256,192,QImage::Format_RGB16);

    //Context Menu.
    this->installEventFilter(this);
    this->m_menuGray=new QMenu;
    this->m_actGrayHexData=new QAction(tr("Gray Hex Data"),this);
    connect(this->m_actGrayHexData,SIGNAL(triggered(bool)),this,SLOT(ZSlotGrayHexData()));
    this->m_actExportGray2File=new QAction(tr("Export Gray to Image File"),this);
    connect(this->m_actExportGray2File,SIGNAL(triggered(bool)),this,SLOT(ZSlotExportGray2ImgFile()));
    this->m_menuGray->addAction(this->m_actGrayHexData);
    this->m_menuGray->addAction(this->m_actExportGray2File);

    this->m_menuTemp=new QMenu;
    this->m_actTempHexData=new QAction(tr("Temperature Hex Data"),this);
    connect(this->m_actTempHexData,SIGNAL(triggered(bool)),this,SLOT(ZSlotTemperatureHexData()));
    this->m_actTempBarChart=new QAction(tr("Temperature Bar Chart"),this);
    connect(this->m_actTempBarChart,SIGNAL(triggered(bool)),this,SLOT(ZSlotTemperatureBarChart()));
    this->m_actExportTemp2File=new QAction(tr("Export Temperature to Image File"),this);
    connect(this->m_actExportTemp2File,SIGNAL(triggered(bool)),this,SLOT(ZSlotExportTemperature2ImgFile()));
    this->m_menuTemp->addAction(this->m_actTempHexData);
    this->m_menuTemp->addAction(this->m_actTempBarChart);
    this->m_menuTemp->addAction(this->m_actExportTemp2File);
}
ZImageCanvas::~ZImageCanvas()
{
    delete this->m_actGrayHexData;
    delete this->m_actExportGray2File;
    delete this->m_menuGray;

    delete this->m_actTempHexData;
    delete this->m_actTempBarChart;
    delete this->m_actExportTemp2File;
    delete this->m_menuTemp;
}
bool ZImageCanvas::ZSaveFile(const QString &fileName)
{
    this->m_img.save(fileName+"_gray.JPG","JPG",100);
    this->m_imgTemp.save(fileName+"_infrared.JPG","JPG",100);
    return true;
}
void ZImageCanvas::ZRedrwFile(const QString &fileName,int nBypassBytes)
{
    emit this->ZSignalLog("Loading file: "+fileName);
    QFile datFile(fileName);
    if(!datFile.open(QIODevice::ReadOnly))
    {
        emit this->ZSignalLog(datFile.errorString());
        return;
    }
    datFile.seek(nBypassBytes); //bypass previous 23 bytes.
    //create a file that eliminate sync head bytes.
    QFile eliminateHeadFile(fileName+".raw");
    if(!eliminateHeadFile.open(QIODevice::WriteOnly))
    {
        emit this->ZSignalLog(eliminateHeadFile.errorString());
        return;
    }

    quint8 rowNo=0, colNo=0;
    while(!datFile.atEnd())
    {
        QByteArray baLine=datFile.read(1040);
        if(baLine.size()!=1040)
        {
            qDebug()<<"baLine is not 1040!";
            break;
        }

        //eliminate Sync Head and write into raw file.
        //bypass first 16 bytes. FF0000B6,FF0000AB,FF00009D,FF000080.
        eliminateHeadFile.write(baLine.data()+16,1024);

        //qDebug("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", ///<
        //       (quint8)baLine.at(0),(quint8)baLine.at(1),(quint8)baLine.at(2),(quint8)baLine.at(3),
        //       (quint8)baLine.at(4),(quint8)baLine.at(5),(quint8)baLine.at(6),(quint8)baLine.at(7),
        //       (quint8)baLine.at(8),(quint8)baLine.at(9),(quint8)baLine.at(10),(quint8)baLine.at(11),
        //       (quint8)baLine.at(12),(quint8)baLine.at(13),(quint8)baLine.at(14),(quint8)baLine.at(15));

        QString syncHeader;
        syncHeader=QString::asprintf("%02x %02x %02x %02x",(quint8)baLine.at(0),(quint8)baLine.at(1),(quint8)baLine.at(2),(quint8)baLine.at(3));
        emit this->ZSignalHexData(syncHeader);
        syncHeader=QString::asprintf("%02x %02x %02x %02x",(quint8)baLine.at(4),(quint8)baLine.at(5),(quint8)baLine.at(6),(quint8)baLine.at(7));
        emit this->ZSignalHexData(syncHeader);
        syncHeader=QString::asprintf("%02x %02x %02x %02x",(quint8)baLine.at(8),(quint8)baLine.at(9),(quint8)baLine.at(10),(quint8)baLine.at(11));
        emit this->ZSignalHexData(syncHeader);
        syncHeader=QString::asprintf("%02x %02x %02x %02x",(quint8)baLine.at(12),(quint8)baLine.at(13),(quint8)baLine.at(14),(quint8)baLine.at(15));
        emit this->ZSignalHexData(syncHeader);
        //bypass 16 bytes.
        qint32 uPixelOffset=16; //1st 16 bytes are FFFFFFFF,00000001,FF00009D,FF000080.
        colNo=0;
        //256*2Bytes are pixel data, 512bytes/4=128.
        for(qint32 i=0; i<=128-1; i++)
        {
            quint8 uCb=baLine.at(uPixelOffset+0);
            quint8 uY1=baLine.at(uPixelOffset+1);
            quint8 uCr=baLine.at(uPixelOffset+2);
            quint8 uY2=baLine.at(uPixelOffset+3);
            uPixelOffset+=4;

            //qDebug("CbYCrY: %02x %02x %02x %02x\n", uCb,uY1,uCr,uY2);

            uint8_t R1=uY1+1.371*(uCr-128);
            uint8_t G1=uY1-0.698*(uCr-128)-0.336*(uCb-128);
            uint8_t B1=uY1+1.732*(uCb-128);

            uint8_t R2=uY2+1.371*(uCr-128);
            uint8_t G2=uY2-0.698*(uCr-128)-0.336*(uCb-128);
            uint8_t B2=uY2+1.732*(uCb-128);

            this->m_img.setPixelColor(colNo,rowNo,QColor(R1,G1,B1));
            this->m_img.setPixelColor(colNo+1,rowNo,QColor(R2,G2,B2));
            colNo+=2;
        }
        //256*2Bytes are temperature data, 512Bytes/4=128.
        colNo=0;
        for(qint32 j=0;j<=128-1; j++)
        {
            quint8 uCb=baLine.at(uPixelOffset+0);
            quint8 uY1=baLine.at(uPixelOffset+1);
            quint8 uCr=baLine.at(uPixelOffset+2);
            quint8 uY2=baLine.at(uPixelOffset+3);
            uPixelOffset+=4;

            quint16 tTemp1=(quint16)uY1<<8|uCb;
            quint16 tTemp2=(quint16)uY2<<8|uCr;
            float floatTemp1=tTemp1/10.0-273.2;
            float floatTemp2=tTemp2/10.0-273.2;
            qDebug("Temperature: %02x %02x %02x %02x, %04x/%04x, %.2f, %.2f\n", uCb,uY1,uCr,uY2,tTemp1,tTemp2,floatTemp1,floatTemp2);
            QColor color1=this->ZMapTemperature2Color(floatTemp1);
            QColor color2=this->ZMapTemperature2Color(floatTemp2);
            this->m_imgTemp.setPixelColor(colNo,rowNo,color1);
            this->m_imgTemp.setPixelColor(colNo+1,rowNo,color2);
            //save temperature in 2-dimension array.
            this->m_ArrayTemp[rowNo][colNo]=floatTemp1; //Real Temperature = 16bit/10.0-273.2
            this->m_ArrayTemp[rowNo][colNo+1]=floatTemp2; //Real Temperature = 16bit/10.0-273.2
            colNo+=2;
        }
        //next row. next 1040 bytes.
        rowNo++;
    } //while(!datFile.atEnd()).

    eliminateHeadFile.close();
    emit this->ZSignalLog(eliminateHeadFile.fileName()+" created.");
    //call paintEvent();
    this->update();
}
void ZImageCanvas::ZSlotUpdateImg(const QImage &img_Pixel, const QImage &img_Temperature)
{
    this->m_img=img_Pixel;
    this->m_imgTemp=img_Temperature;
    this->update();
}
void ZImageCanvas::paintEvent(QPaintEvent *e)
{
    (void)(e);
    QPixmap pixmap1,pixmap2;
    if(pixmap1.convertFromImage(this->m_img))
    {
        QPainter painter(this);
        painter.drawPixmap(0,0,this->width()/2,this->height(),pixmap1);
        painter.setPen(QPen(Qt::red,5));
        painter.setFont(QFont("Times", 20, QFont::Bold));
        painter.drawText(10,30,QString("Gray-Level (%1,%2)").arg(this->m_PosIR.x()).arg(this->m_PosIR.y()));
    }
    if(pixmap2.convertFromImage(this->m_imgTemp))
    {
        QPainter painter(this);
        painter.drawPixmap(this->width()/2+1,0,this->width()/2,this->height(),pixmap2);
        painter.setPen(QPen(Qt::red,5));
        painter.setFont(QFont("Times", 20, QFont::Bold));
        painter.drawText(this->width()/2+10,30,QString("Pseudo-Color (%1,%2)").arg(this->m_PosTemp.x()).arg(this->m_PosTemp.y()));
    }
}
void ZImageCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if(!g_bTrackCursor)
    {
        return;
    }
    float xScale=(this->width()/2)/256.0;
    float yScale=this->height()/192.0;
    if(event->pos().x()>=0 && event->pos().x()<=this->width()/2)
    {
        //Infrared Image.
        qint32 iCol=event->pos().x()/xScale;
        qint32 iRow=event->pos().y()/yScale;
        this->m_PosIR=QPoint(iCol,iRow);
        qDebug("Infrared Image Scale: %.2f,%.2f (%d,%d) map to (%d,%d)\n",xScale,yScale, ///<
               event->pos().x(),event->pos().y(),iCol, iRow);
        emit this->ZSignalInfraredImagePositionChanged(iRow, iCol);
    }else if(event->pos().x()>this->width()/2 && event->pos().x()<=this->width())
    {
        //Temperature Image.
        qint32 iCol=(event->pos().x()-this->width()/2)/xScale;
        qint32 iRow=event->pos().y()/yScale;
        this->m_PosTemp=QPoint(iCol,iRow);
        qDebug("Temperature Image Scale: %.2f,%.2f (%d,%d) map to (%d,%d)\n",xScale,yScale, ///<
               (event->pos().x()-this->width()/2),event->pos().y(),iCol, iRow);
        emit this->ZSignalTemperatureImagePositionChanged(iRow, iCol);
        if(iCol>=0 && iCol<=256-1 && iRow>=0 && iRow<=192-1)
        {
            QString temp=QString("Celsius Degree\n%1").arg(this->m_ArrayTemp[iCol][iRow]);
            QToolTip::showText(event->globalPos(),temp,this,QRect(event->globalPos(),QSize(200,50)));
            qDebug("(%d,%d)=%.2f",iCol,iRow,this->m_ArrayTemp[iCol][iRow]);
        }
    }
    this->update();
}
bool ZImageCanvas::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::ContextMenu)
    {
        QPoint curPoint=QCursor::pos();
        if(curPoint.x()>=0 && curPoint.x()<=this->width()/2)
        {
            this->m_menuGray->exec(QCursor::pos());
            return true;
        }else if(curPoint.x()>this->width()/2 && curPoint.x()<=this->width())
        {
            this->m_menuTemp->exec(QCursor::pos());
            return true;
        }
    }
    return QObject::eventFilter(watched,event);
}
void ZImageCanvas::ZSlotGrayHexData()
{

}
void ZImageCanvas::ZSlotExportGray2ImgFile()
{

}
void ZImageCanvas::ZSlotTemperatureHexData()
{
    ZDialogHexCheck diaHex;
    if(diaHex.ZDoInit())
    {
        for(qint32 i=0;i<192;i++)
        {
            QString hexFmt;
            for(qint32 j=0;j<256;j++)
            {
                hexFmt+=QString::asprintf("%.2f ",this->m_ArrayTemp[i][j]);
            }
            diaHex.ZAppendText(hexFmt);
        }
        diaHex.exec();
    }
}
void ZImageCanvas::ZSlotTemperatureBarChart()
{
    int x=0;
    ZDialogTempChart diaChart;
    if(diaChart.ZDoInit())
    {
        for(qint32 i=0;i<192;i++)
        {
            for(qint32 j=0;j<256;j++)
            {
                diaChart.ZAppendValue(x,this->m_ArrayTemp[i][j]);
                x+=1;
            }
        }
        diaChart.ZAutojustAxes();
        diaChart.exec();
    }
}
void ZImageCanvas::ZSlotExportTemperature2ImgFile()
{

}
QColor ZImageCanvas::ZMapTemperature2Color(float temperature)
{
    //How to map 16-bits temperature to RGB is a question here!!!
    //Find a 16-bits color map online and map 16-bits temperature to RGB.
    // quint8 tR=(tTemp&0xFF00)>>8;
    // quint8 tG=(tTemp&0xFF);
    // quint8 tB=0;
    //return QColor(tR,tG,tB);

    //Real Temperature = 16bit/10.0-273.2
    //float temperature=tTemp/10.0-273.2;
    //qDebug("temp=%d, /10-273.2=%.2f\n",tTemp, temperature);
    float red,green,blue;
    if(temperature<=66.0)
    {
        red=255;
    }else{
        red=temperature-60.0;
        red=329.698727446*pow(red,-0.1332047592);
        if(red<0) red=0;
        if(red>255) red=255;
    }

    if(temperature<=66.0)
    {
        green=temperature;
        green=99.4708025861*log(green)-161.1195681661;
        if(green<0) green=0;
        if(green>255) green=255;
    }else{
        green=temperature-60.0;
        green=288.1221695283*pow(green,-0.0755148492);
        if(green<0) green=0;
        if(green>255) green=255;
    }

    if(temperature>=66.0)
    {
        blue=255;
    }else{
        if(temperature<=19.0)
        {
            blue=0;
        }else{
            blue=temperature-10;
            blue=138.5177312231*log(blue)-305.0447927307;
            if(blue<0) blue=0;
            if(blue>255) blue=255;
        }
    }
    return QColor((uint8_t)red,(uint8_t)green,(uint8_t)blue);
}

