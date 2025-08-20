#ifndef ZDIALOGHEXCHECK_H
#define ZDIALOGHEXCHECK_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
class ZDialogHexCheck : public QDialog
{
public:
    ZDialogHexCheck();
    ~ZDialogHexCheck();
    bool ZDoInit();
    void ZAppendText(const QString &text);
    void ZClearText(void);
protected:
    QSize sizeHint() const;
private:
    QTextEdit *m_te;
    QVBoxLayout *m_vLayout;
};

#endif // ZDIALOGHEXCHECK_H
