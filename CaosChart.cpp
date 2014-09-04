#include <QPainter>
#include "CaosChart.h"

CaosChart::CaosChart(const DataTable *d) : AbstractChart(d) {}

void CaosChart::draw(QPainter *painter, DrawChart *lavagna) const
{
    if(dati->isEmpty())return;              //se la tabella è vuota, non scrivo nulla!

    const int topOffset=30;
    const int leftOffset=-30;
    const int bottomOffset=-30;
    const int rightOffset=80;
    const int lavagnaHeight=445;    //? dimensione lavagna
    const int lavagnaWidth=634;     //?  da cambiare in caso di cambio layout

    //dimensioni assi XY
    int asseX=lavagnaWidth-rightOffset-leftOffset;
    int asseY=lavagnaHeight-topOffset-bottomOffset;

    lavagna->setFixedWidth(lavagnaWidth);

    //imposto il font
    QFont font=painter->font();
    font.setPointSize(20);
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);

   /* //scrivo il titolo del grafico all'interno del topOffset
    painter->setPen(QColor(Qt::red));
    painter->drawText(QRect(0,0,lavagnaWidth,topOffset-offsetCharTitle), Qt::AlignCenter, dati->titoloGrafico);
*/
    //coloro lo sfondo di bianco
    painter->fillRect(QRect(0,0,lavagnaWidth,lavagnaHeight),
                       QBrush(QColor(Qt::white),Qt::SolidPattern));

    font.setPointSize(10);
    painter->setFont(font);

    bool ok;
    for(int j=0;j<dati->ncolonne();j++) //per tutte le serie
    {
        //disegno le varie serie dati
        QColor trasparente=dati->getColoreSerie(j);
        trasparente.setAlpha(120);
        painter->setPen(QPen(QBrush(trasparente),2));

        for(int i=0;i<dati->nrighe();i++)
        {
            //provo a convertire un valore double
            dati->getText(i,j).toDouble(&ok);
            int size;
            if(ok){
                size=dati->getNormValue(i,j,120);//normalizzo su 120
                size=100-size;
                if(size<10)size=10;
            }else{
                //se ho testo, controllo il numero di lettere
                size=9*dati->getText(i,j).size();
            }

            int x=rand()%asseX;
            int y=rand()%asseY;

            if(size<0)size*=-1;
            if(size==0)size=10;
            font.setPointSize(size);
            painter->setFont(font);

            painter->drawText(x+leftOffset,y+topOffset,dati->getText(i,j));
          }//fine dati
    }//fine serie
}
