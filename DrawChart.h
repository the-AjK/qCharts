#ifndef DRAWCHART_H
#define DRAWCHART_H

#include <QWidget>
#include "AbstractChart.h"

class AbstractChart;

class DrawChart : public QWidget
{
    Q_OBJECT

public:
    DrawChart(AbstractChart *, QWidget* =0);
    void setChart2draw(AbstractChart *);

private:
    AbstractChart *chart;

public slots:
	void draw(); 

protected: 
	void paintEvent(QPaintEvent*); 

};

#endif
