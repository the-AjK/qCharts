#ifndef RADARCHART_H
#define RADARCHART_H

#include <QPoint>
#include "AbstractChart.h"
#include "DataTable.h"
#include "DrawChart.h"

class RadarChart : public AbstractChart{
public:
    RadarChart(const DataTable *);
    virtual void draw(QPainter *, DrawChart *) const;
private:
    static QPoint getPoint(QPoint, double, int);
};

#endif
