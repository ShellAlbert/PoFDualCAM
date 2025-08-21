#include "zmainwidget.h"
#include <QApplication>
#include <QLoggingCategory>

// warning: Environment variable QT_LOGGING_TO_CONSOLE is deprecated, use
// QT_ASSUME_STDERR_HAS_CONSOLE and/or QT_FORCE_STDERR_LOGGING instead.
// # QT_ASSUME_STDERR_HAS_CONSOLE=1 qtcreator
int main(int argc, char *argv[])
{
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg,true);
    QApplication app(argc, argv);
    QString appPath=app.applicationDirPath();
    app.addLibraryPath(appPath);
    QString qss("QWidget{background:#222222; color:#FFFFFF;}"
                "QToolButton{border:1px solid #CCCCCC; font-size:18px; min-width:120px; min-height:30px;}"
                "QListWidget{border: 1px solid #FFFFFF; margin:0px; min-width:160px;}"
                "QLabel{border:none; padding:0; background:none; font-size:20px;}"
                "QTextEdit{font-size: 18px;}"
                "QCheckBox{font-size: 18px;}"
                "QCheckBox::indicator:checked {image: url(:/icons/checked.png);}"
                "QCheckBox::indicator:unchecked {image: url(:/icons/unchecked.png);}"
                "QDialog{font-size:36px;}"
                "QSpinBox{font-size:36px;}"
                "QToolTip{font-size:24px;}");
    ZMainWidget win;
    win.setStyleSheet(qss);
    if(win.ZDoInit())
    {
        win.show();
    }

    return app.exec();
    return 0;
}
