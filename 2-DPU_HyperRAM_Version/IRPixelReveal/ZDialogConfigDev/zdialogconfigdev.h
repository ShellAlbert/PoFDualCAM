#ifndef ZDIALOGCONFIGDEV_H
#define ZDIALOGCONFIGDEV_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QComboBox>
class ZDialogConfigDev : public QDialog
{
  Q_OBJECT
public:
  explicit ZDialogConfigDev(QWidget *parent = nullptr);
  ~ZDialogConfigDev();

  bool ZDoInit();

signals:
protected:
    QSize sizeHint() const;
public slots:
  void ZSLotOkay();
  void ZSlotCancel();
private:
  QGridLayout *m_gridLayout;
  QLabel *m_llCOM;
  QComboBox *m_cbCOM;

  QLabel *m_llBPS;
  QComboBox *m_cbBPS;

  QLabel *m_llDatabits;
  QComboBox *m_cbDatabits;

  QLabel *m_llStopbits;
  QComboBox *m_cbStopbits;

  QLabel *m_llParity;
  QComboBox *m_cbParity;

  QHBoxLayout *m_hLayout;
  QToolButton *m_btnOkay;
  QToolButton *m_btnCancel;

  QVBoxLayout *m_vLayout;
};

#endif // ZDIALOGCONFIGDEV_H
