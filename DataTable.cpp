#include <QMessageBox>
#include <QWidgetItem>
#include "DataTable.h"
#include "DrawChart.h"
#include "float.h"

DataTable::DataTable(QString title, int rows, int cols, QWidget* parent)
    : QWidget(parent), titoloGrafico(title), tipoGrafico(LINE), labelX("asse X"), labelY("asse Y"), showLabelSerie(true), showLabelX(true), showLabelY(false), labelColumn(-1),
      min(0), max(DBL_MIN), neverChanged(true), coloreSerie(new QVector<QColor>), tabella(new QTableWidget(parent))
{
    tabella->setRowCount(rows);
    tabella->setColumnCount(cols);
    tabella->setGeometry(QRect(5,5,345,470));

    //preparo i dati, in base alla tabella appena creata
    for(int i=0; i<cols; i++)                              //per ogni colonna del grafico
    {
        //creo la nuova serie e la aggiungo
        tabella->setHorizontalHeaderItem(i,new QTableWidgetItem(QString("Serie%1").arg(i)));

        //creo l'icona con il colore della serie
        addColoreSerie();        //aggiungo un colore random alla serie appena creata
        refreshSerieIcon(i);     //eseguo il refresh dell'icona in tabella

        for(int j=0;j<rows;j++) //creo una cella vuota...
            tabella->setItem(j,i,new QTableWidgetItem(""));
    }

    connect(tabella,SIGNAL(cellChanged(int,int)),this,SLOT(updateCell(int,int)));
}

DataTable::~DataTable()
{
    delete coloreSerie;
    delete tabella;
}

int DataTable::getSelectedColumn() const
{
    int selectedColumns=0;
    int index=-1;
    bool found=false;
    for(int i=0; i<ncolonne(); i++) //per tutte le colonne
    {

        for(int j=0; j<nrighe() && !found; j++) //per tutte le righe finche non trovo un elemento selezionato
        {
            if(tabella->item(j,i)->isSelected()) //se trovo un elemento selezionato
            {
                index=i;    //ho trovato l'elemento nella colonna index
                found=true;
            }
        }
        if(found)               //se sono uscito perche ho trovato un elemento selezionato
        {
            selectedColumns++;  //incremento il numero di colonne
            found=false;        //resetto il flag per il prossimo ciclo
        }
    }
    if(selectedColumns==1)      //solo se ho trovato un'unica colonna selezionata
    {
        return index;           //ritorno l'indice della colonna selezionata
    }else{
        return -1;              //se ci sono piu colonne selezionate ritorno -1
    }
}

int DataTable::nrighe() const
{
    return tabella->rowCount();
}

int DataTable::ncolonne() const
{
    return tabella->columnCount();
}

void DataTable::addRow()
{
   tabella->setRowCount(nrighe()+1);
   for(int i=0;i<ncolonne();i++)
   {
       #ifdef DEBUGTEST
       tabella->setItem(nrighe()-1,i,new QTableWidgetItem(QString("%1").arg(rand()%1000)));
       #else
       tabella->setItem(nrighe()-1,i,new QTableWidgetItem(""));
       #endif
   }
}

void DataTable::removeRow()
{
    if(nrighe()>1)
    {
        tabella->setRowCount(nrighe()-1);
        setMinMax();    //aggiorno il nuovo min-max
    }
}

void DataTable::addColumn()
{
    tabella->setColumnCount(ncolonne()+1);
    tabella->setHorizontalHeaderItem(ncolonne()-1,new QTableWidgetItem(QString("Serie%1").arg(ncolonne()-1)));
    addColoreSerie();                   //aggiungo un colore alla serie (random)
    refreshSerieIcon(ncolonne()-1);     //eseguo il refresh dell'icona in tabella
    for(int i=0; i<nrighe(); i++)
    {
        #ifdef DEBUGTEST
        tabella->setItem(i,ncolonne()-1,new QTableWidgetItem(QString("%1").arg(rand()%100)));
        #else
        tabella->setItem(i,ncolonne()-1,new QTableWidgetItem(""));
        #endif
    }
}

void DataTable::removeColumn()
{
    if(ncolonne()>1)
    {
        if(isLabel(ncolonne()-1))
        {
            labelColumn=-1; //se la colonna che rimuovo era etichettata
        }                   //resetto il labelColumn
        tabella->setColumnCount(ncolonne()-1);
        coloreSerie->pop_back();    //rimuovo il colore
        setMinMax();                //aggiorno il nuovo min-max
    }
}

void DataTable::createTable(int righe, int colonne)
{
    if(righe<1 || colonne<1)return;
    int oldr=nrighe();
    int oldc=ncolonne();
    if(righe<oldr)for(int i=0;i<oldr-righe;i++)removeRow();
    if(righe>oldr)for(int i=0;i<righe-oldr;i++)addRow();
    if(colonne<oldc)for(int i=0;i<oldc-colonne;i++)removeColumn();
    if(colonne>oldc)for(int i=0;i<colonne-oldc;i++)addColumn();
}

QString DataTable::getTitoloSerie(int col) const
{
    return tabella->horizontalHeaderItem(col)->text();
}

void DataTable::setTitoloSerie(int col, QString title)
{
    tabella->horizontalHeaderItem(col)->setText(title);
}

void DataTable::refreshSerieIcon(int col)
{
    QPixmap pixmap(100,100);
    pixmap.fill(getColoreSerie(col));
    QIcon serieIcon(pixmap);
    tabella->horizontalHeaderItem(col)->setIcon(serieIcon);
}

void DataTable::addColoreSerie(int colore)
{
    int r,g,b;
    QColor col;
    if(!colore)                 //se non specifico nulla
    {
        r=rand()%256;           //random color
        g=rand()%256;
        b=rand()%256;
    }else{
        r=colore&255;           //estrapolo RGB
        g=(colore>>8)&255;
        b=(colore>>16)&255;
    }
    col.setRgb(r,g,b);
    coloreSerie->push_back(col); //aggiungo il colore alla serie
}

void DataTable::setColoreSerie(int col, QColor colore)
{
    QVector<QColor>::Iterator it = coloreSerie->begin();
    it[col]=colore;
    //aggiorno l'icona della tabella
    refreshSerieIcon(col);
}

QColor DataTable::getColoreSerie(int col) const
{
    if(coloreSerie->size()<=col)return QColor(0,0,0);
    QVector<QColor>::Iterator it = coloreSerie->begin();
    return it[col];
}

QString DataTable::getText(int riga, int colonna) const
{
    return tabella->item(riga,colonna)->text();
}

void DataTable::setText(int riga, int colonna, QString value)
{
    return tabella->item(riga,colonna)->setText(value);
}

void DataTable::setLabelCol(int col)
{
    if(labelColumn==col)                //se la colonna era già stata selezionata
    {
        labelColumn=-1;                 //deseleziono la colonna
        QString newTitle=getTitoloSerie(col).remove("(L)");
        setTitoloSerie(col,newTitle);   //rimuovo l'etichetta sul nome serie
    }else if(labelColumn==-1)           //se non c'è nessuna colonna selezionata
    {
        labelColumn=col;                //imposto la colonna attuale
        QString newTitle=getTitoloSerie(col).remove("(L)");
                newTitle=getTitoloSerie(col).append("(L)");
        setTitoloSerie(col,newTitle);
    }else{                              //se una colonna è selezionata come label
        QString newTitle=getTitoloSerie(labelColumn).remove("(L)");
        setTitoloSerie(labelColumn,newTitle);   //rimuovo l'etichetta sul nome serie precedentemente selezionata
        labelColumn=col;                //imposto la colonna attuale
        newTitle=getTitoloSerie(col).append("(L)");
        setTitoloSerie(col,newTitle);
    }
}

bool DataTable::isLabel(int col) const
{
    return labelColumn==col;
}

double DataTable::getNormValue(int riga, int colonna, double normalizza) const
{
    bool ok;
    double num=tabella->item(riga,colonna)->text().toDouble(&ok);
    if(ok)
    {
        return num*normalizza/(max-min);
    }else
    {
        return 0;
    }
}

double DataTable::getNegativeNorm(double normalizza) const
{
    return -min*normalizza/(max-min);
}

double DataTable::getMax() const
{
    return max;
}

//SLOTS:
void DataTable::setMinMax()
{
    max=DBL_MIN;
    min=0;
    bool ok;
    double num;
    for(int i=0;i<ncolonne();i++)   //per tutte le colonne della tabella
    {
        //if(!isLabel(i))             //se la colonna non è definita come label
        //{
            for(int j=0;j<nrighe();j++)
            {
                ok=true;
                num=tabella->item(j,i)->text().toDouble(&ok);
                if(ok)// && num!=0)
                {
                    if(num<min)min=num;
                    if(num>max)max=num;
                }
            }
        //}
    }
}

bool DataTable::isEmpty() const
{
    return neverChanged;
}

void DataTable::updateCell(int row, int col)
{

    if(neverChanged)neverChanged=false;
    QString contenutoCella=tabella->item(row,col)->text();

    if(contenutoCella!="0" && contenutoCella!="0.0" && contenutoCella!="0,0")
    {
        if(contenutoCella.toFloat()) //non ho zero, ma controllo se c'è testo
        {
            setMinMax();    //se ho un numero, calcolo il nuovo min e max

        }else{
            //ho testo
            //QMessageBox::about(this, "error","ciao");//"inserisci un numero!");
        }
    }else{

    }
    //ora emetto il segnale dataready per richiamare il painter...
    emit dataReady();
}
