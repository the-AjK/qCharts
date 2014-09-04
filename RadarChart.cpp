#include <QPainter>
#include "RadarChart.h"
#include "math.h"

RadarChart::RadarChart(const DataTable *d) :AbstractChart(d) {}

void RadarChart::draw(QPainter *painter, DrawChart *lavagna) const
{
    if(dati->isEmpty())return;              //se la tabella è vuota, non scrivo nulla!

    const int topOffset=70;
    const int bottomOffset=20;
    const int lavagnaHeight=445;    //? dimensione lavagna
    const int lavagnaWidth=634;     //?  da cambiare in caso di cambio layout
    const int offsetCharTitle=10;   //distanza titolo dal grafico
    const int divisioniRaggio=8;    //divisioni raggio

    lavagna->setFixedWidth(lavagnaWidth);

    int raggioMax=(lavagnaHeight-topOffset-bottomOffset)/2;
    double raggioZero=dati->getNegativeNorm(raggioMax)+3;
    const QPoint centre=QPoint(lavagnaWidth/2,raggioMax+topOffset);
    const int raggioStep=raggioMax/divisioniRaggio;
    const double angle=360/dati->nrighe();

    //imposto il font
    QFont font=painter->font();
    font.setPointSize(20);
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);

    //scrivo il titolo del grafico all'interno del topOffset
    painter->setPen(QColor(Qt::darkCyan));
    painter->drawText(QRect(0,0,lavagnaWidth,topOffset-offsetCharTitle), Qt::AlignCenter, dati->titoloGrafico);

    painter->setPen(QColor(Qt::white));
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(centre,raggioMax,raggioMax);

    //disegno il radar
    for(int i=0; i<dati->nrighe(); i++)
    {
        //disegno i raggi
        painter->setPen(QPen(Qt::black, 2));
        painter->drawLine(centre,getPoint(centre,angle*i,raggioMax));
        painter->setPen(QPen(Qt::gray, 1));
        //disegno le corde
        for(int j=1;j<=divisioniRaggio;j++)
        {
            if(i==dati->nrighe()-1)
            {
                painter->drawLine(getPoint(centre,angle*i,raggioStep*j),getPoint(centre,0,raggioStep*j));
            }else{
                painter->drawLine(getPoint(centre,angle*i,raggioStep*j),getPoint(centre,angle*(i+1),raggioStep*j));
            }
        }
    }

    for(int i=0;i<dati->ncolonne();i++)
    {
        painter->setPen(QPen(QBrush(dati->getColoreSerie(i)),3));
        QColor trasparente=dati->getColoreSerie(i);
        trasparente.setAlpha(100);
        painter->setBrush(QBrush(trasparente));
        //creo il poligono
        QVector<QPoint> poligono;
        for(int j=0;j<dati->nrighe();j++)
        {
            bool ok;
            //provo a convertire un valore double
            dati->getText(j,i).toDouble(&ok);
            //nel caso i dati siano etichettati, posso scriverli solo se il flag showLabelXValues è true
            bool iCanDraw=!dati->isLabel(i) || (dati->showLabelSerie && dati->isLabel(i));
            if(ok && iCanDraw)
            {
                poligono.push_back(getPoint(centre,angle*j,dati->getNormValue(j,i,raggioMax-3)+raggioZero));
            }
            if(dati->isLabel(i))    //se la serie è contrassegnata come etichetta
            {                       //scrivo le etichette
                font.setPointSize(10);
                painter->setFont(font);
                painter->setPen(QColor(Qt::black)); //colore nero

                QPoint estremoRaggio=getPoint(centre,angle*j,raggioMax);
                int deltaLabelX=dati->getText(j,i).size()*10;
                int deltaLabelY=20;

                if(estremoRaggio.x()==centre.x())//se sono ai estremi verticali
                {
                    if(estremoRaggio.y()<centre.y()){deltaLabelY*=-1;}else{deltaLabelY=0;}
                    painter->drawText(QRect(centre.x()-deltaLabelX/2,estremoRaggio.y()+deltaLabelY,deltaLabelX ,20),
                                      Qt::AlignCenter, dati->getText(j,i));
                }else{
                    if(estremoRaggio.y()<centre.y()) //se sono nella semiparte superiore
                    {
                        if(estremoRaggio.x()<centre.x())deltaLabelX*=-1;
                        painter->drawText(QRect(estremoRaggio.x(),estremoRaggio.y()-deltaLabelY,deltaLabelX ,deltaLabelY),
                                          Qt::AlignCenter, dati->getText(j,i));

                    }else{//semiparte inferiore
                        if(estremoRaggio.x()<centre.x())deltaLabelX*=-1;
                        painter->drawText(QRect(estremoRaggio.x(),estremoRaggio.y(),deltaLabelX ,deltaLabelY),
                                          Qt::AlignCenter, dati->getText(j,i));

                    }
                }
                //reimposto il colore
                painter->setPen(QPen(QBrush(dati->getColoreSerie(i)),3));
            }
        }
        QPolygon pol(poligono);
        painter->drawPolygon(pol);
    }
}

QPoint RadarChart::getPoint(QPoint centre, double angolo, int raggio)
{
    double x,y;
    if(angolo>360)angolo-=360;
    if(angolo<=90){
        x=centre.x()+raggio*sin(angolo * 3.14159265 / 180.); //gradi a radianti
        y=centre.y()-raggio*cos(angolo * 3.14159265 / 180.);
    }else if(angolo<=180){
        angolo-=90;
        x=centre.x()+raggio*cos(angolo * 3.14159265 / 180.);
        y=centre.y()+raggio*sin(angolo * 3.14159265 / 180.);
    }else if(angolo<=270){
        angolo-=180;
        x=centre.x()-raggio*sin(angolo * 3.14159265 / 180.);
        y=centre.y()+raggio*cos(angolo * 3.14159265 / 180.);
    }else{
        angolo-=270;
        x=centre.x()-raggio*cos(angolo * 3.14159265 / 180.);
        y=centre.y()-raggio*sin(angolo * 3.14159265 / 180.);
    }
    return QPoint(x,y);
}
