#ifndef ZMAINWIDGET_H
#define ZMAINWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QListWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include "zimagecanvas.h"
#include "ZUARTThread/zuartrecv.h"
#include "ZJpegWidget/zjpegwidget.h"
#include <QProgressBar>
#include <QTabWidget>
#include <QTimer>
class ZMainWidget : public QWidget
{
    Q_OBJECT

public:
    ZMainWidget(QWidget *parent = 0);
    ~ZMainWidget();
    bool ZDoInit();
private slots:
    void ZSlotCfgDev();
    void ZSlotChangeDir();
    void ZSlotSaveAs();
    void ZSlotShowPalette();
    void ZSlotHexCheck();
    void ZSlotBufferHex();
    void ZSlotListWidgetItemDoubleClicked(QListWidgetItem *item);
    void ZSlotAppendLog(const QString &log);
    void ZSlotNewHexData(const QString &hexData);
    void ZSlotOpenUART();
    void ZSlotUpdateRxBytes(qint32 rxBytes);
    void ZSlotUpdateRxFramesCnt(quint32 rxIRFramesCnt, quint32 rxVLFramesCnt, quint32 iErrFramesCnt);
    void ZSlotUpdateMaxMinDiffTemp(qint32 iMax, qint32 iMin, qint32 iDiff);
    void ZSlotUpdateProgressBar(qint32 iValue);
    void ZSlotTrackCursorToggled(bool checked);
    ////////////////////////////////////////////////
    void ZSlotRefreshFileList();
    void ZSlotTimeout();
    void ZSlotTextEditorChanged();
signals:
    void ZSignalLog(const QString &log);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
    QSize sizeHint() const;
private:
    //Left Layout.
    QToolButton *m_btnCfgDev;
    QToolButton *m_btnOpenDir;
    QToolButton *m_btnOpenUART;
    QToolButton *m_btnSaveAs;
    QToolButton *m_btnExport;
    QToolButton *m_btnPalette;
    QToolButton *m_btnHexDisplay;
    QToolButton *m_btnBufferHex;
    QCheckBox *m_cbTraceCursor;
    QListWidget *m_listWidget;
    QVBoxLayout *m_vLayout;
    QWidget *m_widgetLeft;

    ZImageCanvas *m_imgCanvas;
    QTableWidget *m_tableWidget;
    QSplitter *m_hSpliterIR;
    QWidget *m_widgetIR;
    QHBoxLayout *m_hLayoutIR;

    ZJpegWidget *m_jpegWidget;
    QTabWidget *m_tabWidget;

    QSplitter *m_hSpliter;

    QTextEdit *m_teLog;
    QSplitter *m_vSpliter;
    //bottom layout.
    QLabel *m_llRxBytes;
    QLabel *m_llRxFrames;
    QLabel *m_llMaxMinDiffTemp;
    QProgressBar *m_progressBar;
    QHBoxLayout *m_hLayoutBottom;

    QVBoxLayout *m_mainVLayout;

    //Context Menu for QListWidget.
    QMenu *m_menuFileList;
    QAction *m_actChgDir;
    QAction *m_actRefresh;
private:
    QString m_currentDirName;
    ZUARTRecv *m_uartRecv;
    QString m_currentFileName;

private:
    QTimer *m_timer;
    quint8 m_timerFlag;
};

#endif // ZMAINWIDGET_H
