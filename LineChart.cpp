#include <QPainter>
#include "LineChart.h"
#include "math.h"

LineChart::LineChart(const DataTable *d) : AbstractChart(d) {}

void LineChart::draw(QPainter *painter, DrawChart *lavagna) const
{
    if(dati->isEmpty())return;              //se la tabella è vuota, non scrivo nulla!

    const int topOffset=80;
    const int leftOffset=60;
    const int bottomOffset=50;
    const int rightOffset=50;
    const int lavagnaHeight=445;    //? dimensione lavagna
    const int lavagnaWidth=634;     //?  da cambiare in caso di cambio layout
    const int offsetCharTitle=10;   //distanza titolo dal grafico
    const int minimumXstep=85;      //step minimo tra un punto ed il successivo

    //dimensioni assi XY
    int asseX=lavagnaWidth-rightOffset-leftOffset;
    int asseY=lavagnaHeight-topOffset-bottomOffset;

    //dimensione lavagna orizzontale (dinamica)
    int dynamicWidth=lavagnaWidth;

    //imposto il font
    QFont font=painter->font();
    font.setPointSize(20);
    font.setWeight(QFont::DemiBold);
    painter->setFont(font);

    //scrivo il titolo del grafico all'interno del topOffset
    painter->setPen(QColor(Qt::blue));
    painter->drawText(QRect(0,0,lavagnaWidth,topOffset-offsetCharTitle), Qt::AlignCenter, dati->titoloGrafico);

    //calcolo se ho abbastanza spazio
    int xStep=asseX;
    if(dati->nrighe()>1)xStep/=dati->nrighe()-1; //faccio attenzione a non dividere per zero!!
    if(xStep<minimumXstep)                      //se lo step è sotto lo step minimo
    {
        xStep=minimumXstep;
        asseX=minimumXstep*(dati->nrighe()-1);
        dynamicWidth=asseX+leftOffset+rightOffset;
    }
    lavagna->setFixedWidth(dynamicWidth);  //imposto la dimensione della lavagna

    //coloro lo sfondo di bianco
    painter->fillRect(QRect(leftOffset,topOffset,asseX,asseY),
                       QBrush(QColor(Qt::white),Qt::SolidPattern));

    //disegno l'asseX
    double altezzaZero=dati->getNegativeNorm(asseY-3);
    painter->setPen(QColor(Qt::black));
    for(int i=0;i<2;i++)       //asse X spessa 2px
    painter->drawLine(QPoint(leftOffset, i+asseY+topOffset-altezzaZero),
                      QPoint(asseX+leftOffset, i+asseY+topOffset-altezzaZero));

    //disegno la scala
    painter->setPen(QColor(Qt::gray));
    double yStepLadder=30;                  //step di divisioni (30px)
    if(dati->getMax()<10)yStepLadder=60;
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

    bool ok1,ok2;
    for(int j=0;j<dati->ncolonne();j++) //per tutte le serie
    {
        //disegno le varie serie dati
        painter->setPen(QPen(QBrush(dati->getColoreSerie(j)),3));//prelevo il colore della serie width=3
        for(int i=0;i<dati->nrighe()-1;i++)
        {
            //provo a convertire un valore double
            dati->getText(i,j).toDouble(&ok1);
            ok2=false;
            if(dati->nrighe()>1)    //se ho almeno due righe controllo il secondo elemento
                dati->getText(i+1,j).toDouble(&ok2);

            //collego i due punti solamente se entrambi i punti sono numerici
            //nel caso i dati siano etichettati, posso scriverli solo se il flag showLabelXValues è true
            bool iCanDraw=!dati->isLabel(j) || (dati->showLabelSerie && dati->isLabel(j));
            if(ok1 && ok2 && iCanDraw)
            {
                double uno=topOffset+asseY-(dati->getNormValue(i,j,asseY-3))-altezzaZero;
                double due=topOffset+asseY-(dati->getNormValue(i+1,j,asseY-3))-altezzaZero;
                QPoint start((i*xStep)+leftOffset,uno);
                QPoint stop(((i+1)*xStep)+leftOffset,due);
                painter->drawLine(start,stop);
            }
            if(dati->isLabel(j))    //se la serie è contrassegnata come etichetta
            {                       //scrivo le etichette
                painter->setPen(QColor(Qt::black)); //colore nero
                painter->drawText(QRect((i*xStep)+leftOffset-(minimumXstep/2),topOffset+asseY-altezzaZero+3,minimumXstep,20),
                                  Qt::AlignCenter, dati->getText(i,j));

                if(dati->nrighe()>1)    //se ci sono almeno due righe vado a leggere la seconda
                painter->drawText(QRect(((i+1)*xStep)+leftOffset-(minimumXstep/2),topOffset+asseY-altezzaZero+3,minimumXstep,20),
                                  Qt::AlignCenter, dati->getText(i+1,j));

                painter->setPen(QPen(QBrush(dati->getColoreSerie(j)),3)); //reimposto il colore
            }
        }//fine dati
    }//fine serie
}
