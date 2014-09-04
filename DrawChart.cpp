#include <QWidget>
#include <QPainter>
#include "DrawChart.h"

DrawChart::DrawChart (AbstractChart *ch, QWidget* parent) : QWidget(parent), chart(ch) {}

void DrawChart::setChart2draw(AbstractChart *ch)
{
    chart=ch;
}

void DrawChart::draw()
{
	update();
}

void DrawChart::paintEvent(QPaintEvent *)
{
    QPainter *p = new QPainter(this);
    chart->draw(p,this);
}
