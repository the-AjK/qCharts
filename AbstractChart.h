#ifndef ABSTRACTCHART_H
#define ABSTRACTCHART_H

#include "DataTable.h"
#include "DrawChart.h"

class DrawChart;

class AbstractChart{
public:
    AbstractChart(const DataTable *);
    virtual void draw(QPainter *, DrawChart *) const =0;
    virtual ~AbstractChart();

protected:
    const DataTable *dati;            //dati del grafico
};

#endif
