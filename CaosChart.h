#ifndef CAOSCHART_H
#define CAOSCHART_H

#include "AbstractChart.h"
#include "DataTable.h"
#include "DrawChart.h"

class CaosChart : public AbstractChart{
public:
    CaosChart(const DataTable *);
    virtual void draw(QPainter *, DrawChart *) const;
};

#endif
