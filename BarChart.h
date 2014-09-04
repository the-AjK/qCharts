#ifndef BARCHART_H
#define BARCHART_H

#include "AbstractChart.h"
#include "DataTable.h"
#include "DrawChart.h"

class BarChart : public AbstractChart{
public:
    BarChart(const DataTable *);
    virtual void draw(QPainter *, DrawChart *) const;
};

#endif
