#include "zdialogtempchart.h"
//https://blog.csdn.net/qq_43627907/article/details/124563851.
ZDialogTempChart::ZDialogTempChart()
{
    this->m_chartView=nullptr;
    this->m_scatter=nullptr;
    this->m_vLayout=nullptr;
    this->m_max=0;
    this->m_min=100.0;
}
ZDialogTempChart::~ZDialogTempChart()
{
    delete this->m_scatter;
    delete this->m_chartView;
    delete this->m_vLayout;
}
bool ZDialogTempChart::ZDoInit()
{
    this->m_chartView=new QChartView;
    if(nullptr==this->m_chartView)
    {
        return false;
    }
    this->m_chartView->setRenderHint(QPainter::Antialiasing);
    this->m_chartView->chart()->setFont(QFont("SimHei",16));
    this->m_chartView->chart()->setTitle("Infrared Temperature Chart");
    this->m_chartView->chart()->legend()->setMarkerShape(QLegend::MarkerShapeCircle);
    this->m_chartView->chart()->setTheme(QChart::ChartThemeDark);

    this->m_scatter=new QScatterSeries;
    if(nullptr==this->m_scatter)
    {
        return false;
    }
    this->m_scatter->setName("Temperature");
    this->m_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    this->m_scatter->setMarkerSize(5);

    this->m_vLayout=new QVBoxLayout;
    if(nullptr==this->m_vLayout)
    {
        return false;
    }
    this->m_vLayout->addWidget(this->m_chartView);
    this->setLayout(this->m_vLayout);
    return true;
}
void ZDialogTempChart::ZAppendValue(qreal x, qreal y)
{
    this->m_scatter->append(x,y);
    this->m_max=(y>this->m_max)?(y):(this->m_max);
    this->m_min=(y<this->m_min)?(y):(this->m_min);
}
QSize ZDialogTempChart::sizeHint() const
{
    return QSize(800,400);
}
void ZDialogTempChart::ZAutojustAxes()
{
    QString name=QString::asprintf("Max:%.2f Min:%.2f Diff:%.2f",this->m_max,this->m_min,this->m_max-this->m_min);
    this->m_scatter->setName(name);
    this->m_chartView->chart()->addSeries(this->m_scatter);
    this->m_chartView->chart()->createDefaultAxes();
}
