#ifndef ZDIALOGTEMPCHART_H
#define ZDIALOGTEMPCHART_H

#include <QDialog>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QVBoxLayout>
class ZDialogTempChart : public QDialog
{
public:
    ZDialogTempChart();
    ~ZDialogTempChart();
    bool ZDoInit();
    void ZAppendValue(qreal x, qreal y);
    void ZAutojustAxes();
protected:
    QSize sizeHint() const;
private:
    QChartView *m_chartView;
    QScatterSeries *m_scatter;
    QVBoxLayout *m_vLayout;

    qreal m_max;
    qreal m_min;
};

#endif // ZDIALOGTEMPCHART_H
