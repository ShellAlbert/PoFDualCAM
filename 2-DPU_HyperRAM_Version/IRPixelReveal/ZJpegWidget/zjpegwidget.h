#ifndef ZJPEGWIDGET_H
#define ZJPEGWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
class ZJpegWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZJpegWidget(QWidget *parent = nullptr);
    ~ZJpegWidget();
    void ZUpdatePixmap(const QPixmap &pixmap);

signals:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_pixmap;
};

#endif // ZJPEGWIDGET_H
