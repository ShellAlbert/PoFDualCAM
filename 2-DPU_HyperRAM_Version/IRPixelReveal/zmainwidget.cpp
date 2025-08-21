#include "zmainwidget.h"
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QHeaderView>
#include "zdialogpalette.h"
#include <QInputDialog>
#include <QMessageBox>
#include "zglobal.h"
#include "ZDialogConfigDev/zdialogconfigdev.h"
#include "ZDialogHexCheck/zdialoghexcheck.h"
ZMainWidget::ZMainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->m_btnCfgDev=nullptr;
    this->m_btnOpenDir=nullptr;
    this->m_btnOpenUART=nullptr;
    this->m_btnSaveAs=nullptr;
    this->m_btnExport=nullptr;
    this->m_btnPalette=nullptr;
    this->m_btnHexDisplay=nullptr;
    this->m_cbTraceCursor=nullptr;
    this->m_listWidget=nullptr;
    this->m_vLayout=nullptr;
    this->m_widgetLeft=nullptr;
    ///////////////////////////////////
    this->m_imgCanvas=nullptr;
    this->m_tableWidget=nullptr;
    this->m_hSpliterIR=nullptr;
    this->m_hLayoutIR=nullptr;
    this->m_widgetIR=nullptr;
    this->m_jpegWidget=nullptr;
    this->m_tabWidget=nullptr;
    this->m_hSpliter=nullptr;
    /////////////////////////////////////
    this->m_textEdit=nullptr;
    this->m_vSpliter=nullptr;
    this->m_llRxBytes=nullptr;
    this->m_llRxFrames=nullptr;
    this->m_llMaxMinDiffTemp=nullptr;
    this->m_hLayoutBottom=nullptr;
    //////////////////////////////////////////////
    this->m_mainVLayout=nullptr;
    //////////////////////////////////////////////
    this->m_actRefresh=nullptr;
    this->m_actChgDir=nullptr;
    this->m_menuFileList=nullptr;

    this->setWindowTitle("Infrared Pixel Reveal - V0.0.1");
    this->setWindowIcon(QIcon(":/icons/camera.png"));
    this->m_uartRecv=nullptr;
}

ZMainWidget::~ZMainWidget()
{
    delete this->m_btnCfgDev;
    delete this->m_btnOpenDir;
    delete this->m_btnOpenUART;
    delete this->m_btnSaveAs;
    delete this->m_btnExport;
    delete this->m_btnPalette;
    delete this->m_btnHexDisplay;
    delete this->m_cbTraceCursor;
    delete this->m_listWidget;
    delete this->m_vLayout;
    delete this->m_widgetLeft;
    ///////////////////////////////////
    delete this->m_imgCanvas;
    delete this->m_tableWidget;
    delete this->m_hSpliterIR;
    delete this->m_hLayoutIR;
    delete this->m_widgetIR;
    delete this->m_jpegWidget;
    delete this->m_tabWidget;
    delete this->m_hSpliter;
    /////////////////////////////////////
    delete this->m_textEdit;
    delete this->m_vSpliter;
    delete this->m_llRxBytes;
    delete this->m_llRxFrames;
    delete this->m_llMaxMinDiffTemp;
    delete this->m_hLayoutBottom;

    delete this->m_mainVLayout;
    ///////////////////////////////
    delete this->m_actChgDir;
    delete this->m_actRefresh;
    delete this->m_menuFileList;
}
bool ZMainWidget::ZDoInit()
{
    //Left Layout.
    this->m_btnOpenDir=new QToolButton;
    this->m_btnOpenDir->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnOpenDir->setText("Change Dir");
    this->m_btnOpenDir->setIcon(QIcon(":/icons/change_dir.png"));
    this->m_btnOpenDir->setIconSize(QSize(24,24));
    this->m_btnOpenDir->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnOpenDir->setToolTip(tr("Change Directory\nIterate all .DAT files in the list."));

    this->m_btnCfgDev=new QToolButton;
    this->m_btnCfgDev->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnCfgDev->setText("Config Dev");
    this->m_btnCfgDev->setIcon(QIcon(":/icons/open_port.png"));
    this->m_btnCfgDev->setIconSize(QSize(24,24));
    this->m_btnCfgDev->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnCfgDev->setToolTip(tr("Configure Device\nShow Configure Dialog to configure appropriate parameters for serial port."));

    this->m_btnOpenUART=new QToolButton;
    this->m_btnOpenUART->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnOpenUART->setText("Listen Now");
    this->m_btnOpenUART->setIcon(QIcon(":/icons/listen.png"));
    this->m_btnOpenUART->setIconSize(QSize(24,24));
    this->m_btnOpenUART->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnOpenUART->setToolTip(tr("Listen Now\nAPP enters listen mode to render received data."));

    this->m_btnSaveAs=new QToolButton;
    this->m_btnSaveAs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnSaveAs->setText("Save As  ");
    this->m_btnSaveAs->setIcon(QIcon(":/icons/save_as.png"));
    this->m_btnSaveAs->setIconSize(QSize(24,24));
    this->m_btnSaveAs->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnSaveAs->setToolTip(tr("Save Images to Files\nGray-level and Pseudo-color files will be generated."));

    this->m_btnExport=new QToolButton;
    this->m_btnExport->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnExport->setText("Export  ");
    this->m_btnExport->setIcon(QIcon(":/icons/export.png"));
    this->m_btnExport->setIconSize(QSize(24,24));
    this->m_btnExport->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnExport->setToolTip(tr("Export Hex Data"));

    this->m_btnPalette=new QToolButton;
    this->m_btnPalette->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnPalette->setText("Palette  ");
    this->m_btnPalette->setIcon(QIcon(":/icons/palette.png"));
    this->m_btnPalette->setIconSize(QSize(24,24));
    this->m_btnPalette->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnPalette->setToolTip(tr("Palette Adjustment\nManual Adjustment for Pseudo-color Rendering"));

    this->m_btnHexDisplay=new QToolButton;
    this->m_btnHexDisplay->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->m_btnHexDisplay->setText("Hex Check");
    this->m_btnHexDisplay->setIcon(QIcon(":/icons/hex.png"));
    this->m_btnHexDisplay->setIconSize(QSize(24,24));
    this->m_btnHexDisplay->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_btnHexDisplay->setToolTip(tr("Hex Check\nDisplay file data in hex format."));

    this->m_cbTraceCursor=new QCheckBox("Trace Cursor");
    this->m_cbTraceCursor->setIconSize(QSize(24,24));
    this->m_cbTraceCursor->setCheckable(true);
    this->m_cbTraceCursor->setToolTip(tr("Track Cursor\nShow temperature in Celsius Degreen where cursor holds on."));

    this->m_listWidget=new QListWidget;
    this->m_listWidget->installEventFilter(this);
    //Context Menu for QListWidget.
    this->m_menuFileList=new QMenu;
    this->m_actRefresh=new QAction(tr("Refresh"),this);
    this->m_actChgDir=new QAction(tr("Change Dir..."),this);
    connect(this->m_actRefresh,SIGNAL(triggered(bool)),this,SLOT(ZSlotRefreshFileList()));
    connect(this->m_actChgDir,SIGNAL(triggered(bool)),this,SLOT(ZSlotChangeDir()));
    this->m_menuFileList->addAction(this->m_actRefresh);
    this->m_menuFileList->addAction(this->m_actChgDir);

    this->m_vLayout =new QVBoxLayout;
    this->m_vLayout->addWidget(this->m_btnOpenUART);
    this->m_vLayout->addWidget(this->m_btnCfgDev);
    this->m_vLayout->addWidget(this->m_btnOpenDir);
    this->m_vLayout->addWidget(this->m_btnSaveAs);
    this->m_vLayout->addWidget(this->m_btnExport);
    this->m_vLayout->addWidget(this->m_btnPalette);
    this->m_vLayout->addWidget(this->m_btnHexDisplay);
    this->m_vLayout->addWidget(this->m_cbTraceCursor);
    this->m_vLayout->addWidget(this->m_listWidget);
    this->m_vLayout->setMargin(1);
    this->m_widgetLeft=new QWidget;
    this->m_widgetLeft->setLayout(this->m_vLayout);
    ////////////////////////////////////////////////////////
    QObject::connect(this->m_btnCfgDev,SIGNAL(clicked(bool)),this,SLOT(ZSlotCfgDev()));
    QObject::connect(this->m_btnOpenUART,SIGNAL(clicked(bool)),this,SLOT(ZSlotOpenUART()));
    QObject::connect(this->m_btnOpenDir,SIGNAL(clicked(bool)),this,SLOT(ZSlotChangeDir()));
    QObject::connect(this->m_btnSaveAs,SIGNAL(clicked(bool)),this,SLOT(ZSlotSaveAs()));
    QObject::connect(this->m_btnPalette,SIGNAL(clicked(bool)),this,SLOT(ZSlotShowPalette()));
    QObject::connect(this->m_btnHexDisplay,SIGNAL(clicked(bool)),this,SLOT(ZSlotHexCheck()));
    QObject::connect(this->m_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(ZSlotListWidgetItemDoubleClicked(QListWidgetItem*)));
    QObject::connect(this->m_cbTraceCursor,SIGNAL(toggled(bool)),this,SLOT(ZSlotTrackCursorToggled(bool)));
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    this->m_imgCanvas=new ZImageCanvas;
    this->m_tableWidget=new QTableWidget(0,2); //1 row, 2 columns.
    this->m_tableWidget->verticalHeader()->setVisible(false);
    this->m_tableWidget->horizontalHeader()->setVisible(false);
    //  QStringList horHeader;
    //  horHeader.append(("OFFSET"));
    //  horHeader.append(("HEX"));
    //  this->m_tableWidget->setHorizontalHeaderLabels(horHeader);
    this->m_hSpliterIR=new QSplitter(Qt::Horizontal);
    this->m_hSpliterIR->addWidget(this->m_imgCanvas);
    this->m_hSpliterIR->addWidget(this->m_tableWidget);
    this->m_hSpliterIR->setStretchFactor(0,9);
    this->m_hSpliterIR->setStretchFactor(1,1);

    this->m_widgetIR=new QWidget;
    this->m_hLayoutIR=new QHBoxLayout;
    this->m_hLayoutIR->addWidget(this->m_hSpliterIR);
    this->m_widgetIR->setLayout(this->m_hLayoutIR);
    this->m_jpegWidget=new ZJpegWidget;
    this->m_tabWidget=new QTabWidget;
    this->m_tabWidget->addTab(this->m_widgetIR,tr("Infrared Image"));
    this->m_tabWidget->addTab(this->m_jpegWidget,tr("Visible-Light Image"));
    ///////////////////////////////////////////////////////////////////////////////////////////////
    this->m_hSpliter=new QSplitter(Qt::Horizontal);
    this->m_hSpliter->addWidget(this->m_widgetLeft);
    this->m_hSpliter->addWidget(this->m_tabWidget);
    this->m_hSpliter->setStretchFactor(0,1);
    this->m_hSpliter->setStretchFactor(1,9);

    connect(this->m_imgCanvas,SIGNAL(ZSignalLog(QString)),this,SLOT(ZSlotAppendLog(QString)));
    connect(this->m_imgCanvas,SIGNAL(ZSignalHexData(QString)),this,SLOT(ZSlotNewHexData(QString)));
    ///////////////////////////////////////////////////////////////
    this->m_textEdit=new QTextEdit;
    this->m_textEdit->setReadOnly(true);
    this->m_vSpliter=new QSplitter(Qt::Vertical);
    this->m_vSpliter->addWidget(this->m_hSpliter);
    this->m_vSpliter->addWidget(this->m_textEdit);
    ////////////////////////////////////////////////////////////////////
    //bottom layout.
    this->m_llRxBytes=new QLabel("Rx Buffer:0");
    this->m_llRxFrames=new QLabel("Rx Frames:0");
    this->m_llMaxMinDiffTemp=new QLabel("Max:0 Min:0 Diff:0");
    this->m_progressBar=new QProgressBar;
    this->m_progressBar->setRange(0,192-1);
    this->m_hLayoutBottom=new QHBoxLayout;
    this->m_hLayoutBottom->addWidget(this->m_llRxBytes);
    this->m_hLayoutBottom->addWidget(this->m_llRxFrames);
    this->m_hLayoutBottom->addWidget(this->m_llMaxMinDiffTemp);
    this->m_hLayoutBottom->addWidget(this->m_progressBar);
    this->m_hLayoutBottom->setStretchFactor(this->m_llRxBytes,1);
    this->m_hLayoutBottom->setStretchFactor(this->m_llRxFrames,1);
    this->m_hLayoutBottom->setStretchFactor(this->m_llMaxMinDiffTemp,1);
    this->m_hLayoutBottom->setStretchFactor(this->m_progressBar,7);
    ////////////////////////////////////////////////////////////
    this->m_mainVLayout=new QVBoxLayout;
    this->m_mainVLayout->addWidget(this->m_vSpliter);
    this->m_mainVLayout->addLayout(this->m_hLayoutBottom);
    this->m_mainVLayout->setStretchFactor(this->m_vSpliter,10);
    this->m_mainVLayout->setStretchFactor(this->m_textEdit,2);
    this->setLayout(this->m_mainVLayout);

    QObject::connect(this,SIGNAL(ZSignalLog(QString)),this,SLOT(ZSlotAppendLog(QString)));
    emit this->ZSignalLog("Welcome to use IRPixelReveal!\n"
                          "This APP helps to render pixel and temperature array data from Infrared Image Sensor!\n"
                          "Resolution: 256*192  Temperature: 16-bits\n"
                          "One Single Line: FF0000B6 FF0000AB FF00009D FF000080 256*2-Pixel 256*2-Temperature, 1040 Bytes in total.\n"
                          "One Frame Bytes: 192 Lines * 1040 Bytes = 199680 Bytes\n"
                          "Head 2 byts(0x1987) are used to verify Communication Correctness.\n"
                          "Following 20 bytes(Burst R/W DDR PSRAM) are used to verify PSRAM R/W.\n"
                          "Object Temperature=(Low8/High8)/10.0-273.2 (Celsius)");
    return true;
}

void ZMainWidget::ZSlotChangeDir()
{
    this->m_currentDirName=QFileDialog::getExistingDirectory(this,tr("Open Dir"),".",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(this->m_currentDirName.isEmpty())
    {
        return;
    }
    this->ZSlotRefreshFileList();
}
void ZMainWidget::ZSlotCfgDev()
{
    ZDialogConfigDev diaCfgDev;
    if(diaCfgDev.ZDoInit())
    {
        diaCfgDev.exec();
    }
}
void ZMainWidget::ZSlotSaveAs()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("Save Image As..."),QString(),"Images (*.JPEG *.JPG)");
    if(fileName.isEmpty())
    {
        return;
    }
    if(this->m_imgCanvas->ZSaveFile(fileName))
    {
        emit this->ZSignalLog(QString("Save Images to File %1 Successfully.").arg(fileName));
    }else{
        emit this->ZSignalLog("Save Images Failed.");
    }
}
void ZMainWidget::ZSlotShowPalette()
{
    ZDialogPalette dia;
    dia.exec();
}
void ZMainWidget::ZSlotHexCheck()
{
    QFile file(this->m_currentFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,tr("Error Message"),file.errorString());
        return;
    }

    ZDialogHexCheck diaHex;
    if(diaHex.ZDoInit())
    {
        while(!file.atEnd())
        {
            QString hexFmt;
            QByteArray ba=file.read(32);
            for(qint32 i=0;i<ba.size();i++)
            {
                hexFmt+=QString::asprintf("%02X  ", (unsigned char)ba.at(i));
                if(0==(i+1)%32)
                {
                    diaHex.ZAppendText(hexFmt);
                    hexFmt.clear();
                }
            }
        }
        diaHex.exec();
    }
    file.close();
}
void ZMainWidget::ZSlotListWidgetItemDoubleClicked(QListWidgetItem *item)
{
    bool bOkay;
    int nByassBytes=QInputDialog::getInt(this,tr("IRPixelReveal - User Input"), ///<
                                           tr("How many head bytes to bypass?\n\nSpecific bytes from head will be eliminated before rendering.\n\nDefault is 20."),///<
                                           20,0,100,1,&bOkay);
    if(bOkay)
    {
        emit this->ZSignalLog(QString("Head bytes %1 will be passed.").arg(nByassBytes));
        this->m_tableWidget->clearContents();
        this->m_tableWidget->setRowCount(0);
        this->m_imgCanvas->ZRedrwFile(this->m_currentDirName+"/"+item->text(),nByassBytes);
        this->m_currentFileName=this->m_currentDirName+"/"+item->text();
    }
}

void ZMainWidget::ZSlotAppendLog(const QString &log)
{
    this->m_textEdit->append(log);
}
void ZMainWidget::ZSlotNewHexData(const QString &hexData)
{
    QTableWidgetItem *itemOffset=new QTableWidgetItem;
    itemOffset->setText(QString("%1").arg(this->m_tableWidget->rowCount()));
    itemOffset->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *itemValue=new QTableWidgetItem;
    itemValue->setText(hexData);
    itemValue->setTextAlignment(Qt::AlignCenter);

    this->m_tableWidget->setRowCount(this->m_tableWidget->rowCount()+1);
    quint32 rowNo=this->m_tableWidget->rowCount();
    this->m_tableWidget->setItem(rowNo-1,0,itemOffset);
    this->m_tableWidget->setItem(rowNo-1,1,itemValue);

}
void ZMainWidget::ZSlotOpenUART()
{
    if(nullptr==this->m_uartRecv)
    {
        this->m_uartRecv=new ZUARTRecv;
        if(nullptr==this->m_uartRecv)
        {
            QMessageBox::critical(this,tr("Error Message"),tr("Create thread for UART failed."));
            return;
        }
        if(!this->m_uartRecv->ZDoInit())
        {
            QMessageBox::critical(this,tr("Error Message"),tr("Initialize UART failed."));
            return;
        }
        connect(this->m_uartRecv,SIGNAL(ZSignalLog(QString)),this,SLOT(ZSlotAppendLog(QString)));
        connect(this->m_uartRecv,SIGNAL(ZSignalHexData(QString)),this,SLOT(ZSlotNewHexData(QString)));
        connect(this->m_uartRecv,SIGNAL(ZSignalNewImage(QImage,QImage)),this->m_imgCanvas,SLOT(ZSlotUpdateImg(QImage,QImage)));
        connect(this->m_uartRecv,SIGNAL(ZSignalRxBytes(qint32)),this,SLOT(ZSlotUpdateRxBytes(qint32)));
        connect(this->m_uartRecv,SIGNAL(ZSignalRxFrames(qint32)),this,SLOT(ZSlotUpdateRxFrames(qint32)));
        connect(this->m_uartRecv,SIGNAL(ZSignalMaxMinDiffTempChanged(qint32,qint32,qint32)),this,SLOT(ZSlotUpdateMaxMinDiffTemp(qint32,qint32,qint32)));
        connect(this->m_uartRecv,SIGNAL(ZSignalRenderProgress(qint32)),this,SLOT(ZSlotUpdateProgressBar(qint32)));
        connect(this->m_imgCanvas,SIGNAL(ZSignalInfraredImagePositionChanged(qint32,qint32)),this->m_uartRecv,SLOT(ZSlotFetchIRImageData(qint32,qint32)));
        connect(this->m_imgCanvas,SIGNAL(ZSignalTemperatureImagePositionChanged(qint32,qint32)),this->m_uartRecv,SLOT(ZSlotFetchTempImageData(qint32,qint32)));
    }
    if(!this->m_uartRecv->ZIsOpened())
    {
        if(this->m_uartRecv->ZOpenUART("/dev/ttyUSB1"))
        {
            this->m_btnOpenUART->setText("Pause Now");
            this->m_btnOpenUART->setIcon(QIcon(":/icons/halt.png"));
            emit this->ZSignalLog("Infrared Image Sensor needs 7 seconds to start up, please be patient...");
        }
    }else {
        this->m_uartRecv->ZCloseUART();
        this->m_btnOpenUART->setText("Listen Now");
        this->m_btnOpenUART->setIcon(QIcon(":/icons/listen.png"));
    }
}
void ZMainWidget::ZSlotUpdateRxBytes(qint32 rxBytes)
{
    this->m_llRxBytes->setText(QString("Rx Buffer:%1").arg(rxBytes));
}
void ZMainWidget::ZSlotUpdateRxFrames(qint32 rxFrames)
{
    this->m_llRxFrames->setText(QString("Rx Frames:%1").arg(rxFrames));
}
void ZMainWidget::ZSlotUpdateMaxMinDiffTemp(qint32 iMax, qint32 iMin, qint32 iDiff)
{
    this->m_llMaxMinDiffTemp->setText(QString("Max:%1 Min:%2 Diff:%3").arg(iMax).arg(iMin).arg(iDiff));
}
void ZMainWidget::ZSlotUpdateProgressBar(qint32 iValue)
{
    this->m_progressBar->setValue(iValue);
}

void ZMainWidget::ZSlotTrackCursorToggled(bool checked)
{
    if(checked)
    {
        emit this->ZSignalLog(("Track Cursor Enabled."));
        g_bTrackCursor=1;
    }else
    {
        emit this->ZSignalLog(("Track Cursor Disabled."));
        g_bTrackCursor=0;
    }
}
bool ZMainWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==this->m_listWidget)
    {
        if(event->type()==QEvent::ContextMenu)
        {
            this->m_menuFileList->exec(QCursor::pos());
            return true;
        }
    }
    return QObject::eventFilter(watched,event);
}
QSize ZMainWidget::sizeHint() const
{
    return QSize(1024,768);
}
void ZMainWidget::ZSlotRefreshFileList()
{
    QDir dir(this->m_currentDirName,QString("*.DAT"));
    if(!dir.exists())
    {
        emit this->ZSignalLog("Directory does not exist, "+dir.absolutePath());
        return;
    }
    this->m_listWidget->clear();
    QFileInfoList  fileInfoList=dir.entryInfoList();
    for(int i=0;i<fileInfoList.size();i++)
    {
        QFileInfo fileInfo=fileInfoList.at(i);
        QListWidgetItem *item=new QListWidgetItem;
        item->setText(fileInfo.fileName());
        item->setToolTip(fileInfo.absoluteFilePath());
        this->m_listWidget->addItem(item);
    }
    emit this->ZSignalLog("Refresh directory "+this->m_currentDirName+" done.");
}
