#include "zjpegwidget.h"
#include <QPainter>
ZJpegWidget::ZJpegWidget(QWidget *parent)
    : QWidget{parent}
{}
ZJpegWidget::~ZJpegWidget()
{

}
void ZJpegWidget::ZUpdatePixmap(const QPixmap &pixmap)
{
    this->m_pixmap=pixmap;
    this->update();
}
void ZJpegWidget::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),this->m_pixmap);
}
