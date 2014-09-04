#include <QPainter>
#include "BarChart.h"

BarChart::BarChart(const DataTable *d) :AbstractChart(d) {}

void BarChart::draw(QPainter *painter, DrawChart *lavagna) const
{
    if(dati->isEmpty())return;      //se la tabella è vuota, non scrivo nulla!

    const int topOffset=80;
    const int leftOffset=60;
    const int bottomOffset=50;
    const int rightOffset=50;
    const int lavagnaHeight=445;    //? dimensione lavagna
    const int lavagnaWidth=634;     //?  da cambiare in caso di cambio layout
    const int offsetCharTitle=10;   //distanza titolo dal grafico

    int dimBarra=15;                //dimensioni barra iniziale
    int xStep=15;                   //spazio tra una barra e la successiva
    int serieStep=dimBarra*dati->ncolonne()+xStep*(dati->ncolonne()-1);

    //dimensioni assi XY
    int asseX=lavagnaWidth-rightOffset-leftOffset;
    int spazioOccupato=serieStep*dati->nrighe()+xStep*2*dati->nrighe();
    if(spazioOccupato>asseX)asseX=spazioOccupato;

    int asseY=lavagnaHeight-topOffset-bottomOffset;

    //aggiorno la grandezza della lavagna
    lavagna->setFixedWidth(asseX+leftOffset+rightOffset);

    //imposto il font
    QFont font=painter->font();
    font.setPointSize(20);
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);

    //scrivo il titolo del grafico all'interno del topOffset
    painter->setPen(QColor(Qt::green));
    painter->drawText(QRect(0,0,lavagnaWidth,topOffset-offsetCharTitle), Qt::AlignCenter, dati->titoloGrafico);

    //coloro lo sfondo di bianco
    painter->fillRect(QRect(leftOffset,topOffset,asseX,asseY),
                       QBrush(QColor(Qt::white),Qt::SolidPattern));

    //disegno l'asseX
    double altezzaZero=dati->getNegativeNorm(asseY);
    //l'asse X la disegno alla fine...

    //disegno la scala
    painter->setPen(QColor(Qt::gray));
    double yStepLadder=30;          //step di divisioni (30px)
    double labelCoeff=(yStepLadder*dati->getMax())/(asseY-altezzaZero-3);
    font.setPointSize(10);
    painter->setFont(font);

    //disegno la scala positiva
    for(int i=1; i<=(asseY-altezzaZero)/yStepLadder; i++)
    {
        painter->drawLine(QPoint(leftOffset,topOffset+asseY-altezzaZero-i*yStepLadder),
                          QPoint(asseX+leftOffset,topOffset+asseY-altezzaZero-i*yStepLadder));
        //scrivo le etichette dell'asse Ypositiva
        painter->drawText(QRect(0,topOffset+asseY-altezzaZero-i*yStepLadder-5,leftOffset,10),
                          Qt::AlignCenter, QString("%1").arg((int)(i*labelCoeff)));
    }

    //disegno la scala negativa
    for(int i=1; i<=altezzaZero/yStepLadder; i++)
    {
        painter->drawLine(QPoint(leftOffset,topOffset+asseY-altezzaZero+i*yStepLadder),
                          QPoint(asseX+leftOffset,topOffset+asseY-altezzaZero+i*yStepLadder));
        //scrivo le etichette dell'asse Ypositiva
        painter->drawText(QRect(0,topOffset+asseY-altezzaZero+i*yStepLadder-5,leftOffset,10),
                          Qt::AlignCenter, QString("%1").arg((int)(-i*labelCoeff)));
    }

    //disegno lo zero
    painter->drawText(QRect(0,topOffset+asseY-altezzaZero-5,leftOffset,10),
                      Qt::AlignCenter, QString("0"));

    //disegno l'etichetta dell'asse X se necessario
    font.setPointSize(13);
    painter->setFont(font);
    painter->setPen(QColor(Qt::darkGreen));
    if(dati->showLabelX)
        painter->drawText(QRect(0,topOffset+asseY+10,lavagnaWidth,bottomOffset), Qt::AlignCenter, dati->labelX);

    //disegno l'etichetta dell'asse Y se necessario
    if(dati->showLabelY)
        painter->drawText(QRect(0,0,leftOffset,topOffset), Qt::AlignCenter, dati->labelY);

    //reimposto il font a 10
    font.setPointSize(10);
    painter->setFont(font);

    bool ok;
    for(int i=0;i<dati->nrighe();i++)
    {
        for(int j=0;j<dati->ncolonne();j++)
        {
            //painter->setPen(dati->getColoreSerie(j),2); //prelevo il colore della serie
            painter->setPen(QPen(QBrush(dati->getColoreSerie(j)),2));
            QColor trasparente=dati->getColoreSerie(j);
            trasparente.setAlpha(150);
            painter->setBrush(QBrush(trasparente));
            //provo a convertire un valore double
            dati->getText(i,j).toDouble(&ok);
            bool iCanDraw=!dati->isLabel(j) || (dati->showLabelSerie && dati->isLabel(j));
            if(ok && iCanDraw)
            {
                double dato=dati->getNormValue(i,j,asseY-3);
                double hBarra=topOffset+asseY-dato-altezzaZero;

                QRect r(QPoint(leftOffset+
                               j*(dimBarra+xStep)+
                               i*(serieStep+2*xStep),
                               hBarra),QSize(dimBarra,dato));
                //painter->fillRect(r,dati->getColoreSerie(j));
                painter->drawRect(r);
            }
            if(dati->isLabel(j))    //se la serie è contrassegnata come etichetta
            {                       //scrivo le etichette
                painter->setPen(QColor(Qt::black)); //colore nero
                painter->drawText(QRect(leftOffset+i*(serieStep+2*xStep),topOffset+asseY+3,serieStep+2*xStep,20),
                                  Qt::AlignCenter, dati->getText(i,j));

                painter->setPen(dati->getColoreSerie(j));   //reimposto il colore
            }

        }//fine dati
    }//fine serie

    //disegno l'asse X
    painter->setPen(QColor(Qt::black));
    for(int i=0;i<2;i++)       //asse X spessa 2px
    painter->drawLine(QPoint(leftOffset, i+asseY+topOffset-altezzaZero),
                      QPoint(asseX+leftOffset, i+asseY+topOffset-altezzaZero));

}
