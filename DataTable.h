#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QTableWidget>

//#define DEBUGTEST

enum ChartType{             //tipo di chart che l'utente sta visualizzando
    LINE,                   //0 - linee
    BAR,                    //1 - barre
    CAOS,                   //2 - bolle
    RADAR                   //3 - radar
};

//class QAction;

class DataTable : public QWidget
{
    Q_OBJECT

public:
    DataTable(QString, int, int, QWidget* =0);
    ~DataTable();

    int nrighe() const;
    int ncolonne() const;
    void addRow();
    void addColumn();
    void removeRow();
    void removeColumn();
    void createTable(int, int);

    bool isEmpty() const;
    QString getTitoloSerie(int) const;
    QColor getColoreSerie(int) const;
    QString getText(int, int) const;
    int getSelectedColumn() const;
    bool isLabel(int) const;
    double getNormValue(int, int, double) const;   //ritorna il valore normalizzato se possibile (valore numerico)
    double getNegativeNorm(double) const;          //restituisce il valore normalizzato per il calcolo dell'altezza dello zero (utile per valori negativi)
    double getMax() const;

    void setTitoloSerie(int, QString);
    void addColoreSerie(int =0);
    void setColoreSerie(int, QColor);
    void setText(int, int, QString);
    void setLabelCol(int);
    void refreshSerieIcon(int col);

    QString titoloGrafico;
    ChartType tipoGrafico;              //tipo di grafico
    QString labelX;                     //etichetta asse X
    QString labelY;                     //etichetta asse Y
    bool showLabelSerie;                //flag visualizza/nascondi serie di dati etichettati
    bool showLabelX;                    //flag visualizza/nascondi etichetta asse X
    bool showLabelY;                    //flag visualizza/nascondi etichetta asse Y
    int labelColumn;                    //colonna dedicata come etichette (-1 se nessuna)

private:
    void setMinMax();                   //calcola il minimo e massimo dei dati inseriti

    double min;                         //valori min, max per la normalizzazione
    double max;
    bool neverChanged;                  //flag true, neverchanged che diventa false la prima volta che si inserisce un dato in tabella
    QVector<QColor> *coloreSerie;       //puntatore alla lista di colori delle serie
    QTableWidget *tabella;

public slots:
    void updateCell(int, int);          //aggiorna la base dati con il nuovo valore della cella appena modificata x,y

signals:
    void dataReady();                   //segnale dataReady, emesso dopo l'updateCell...

};

#endif
