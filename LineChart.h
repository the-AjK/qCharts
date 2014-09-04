#ifndef LINECHART_H
#define LINECHART_H

#include "AbstractChart.h"
#include "DataTable.h"
#include "DrawChart.h"

class LineChart : public AbstractChart{
public:
    LineChart(const DataTable *);
    virtual void draw(QPainter *, DrawChart *) const;
};

#endif
