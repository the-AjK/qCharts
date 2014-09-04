#ifndef LAYOUT_H
#define LAYOUT_H

//per visualizzare correttamente le immagini...
#define USE_RESOURCES
//#define DO_NOT_USE_RESOURCES

#include <QMainWindow>
#include <QGridLayout>
#include "DataTable.h"
#include "DrawChart.h"
#include "AbstractChart.h"

class QAction;
class QMenu;

#ifdef USE_RESOURCES
    #define logoPath        ":/immagini/qChartsLogo.png"
    #define iconPath        ":/immagini/qChartsIcona.png"
    #define menuArrowPath   ":/immagini/menuArrow.png"
#else
    #define logoPath        "/immagini/qChartsLogo.png"
    #define iconPath        "/immagini/qChartsIcona.png"
    #define menuArrowPath   "/immagini/menuArrow.png"
#endif

class Layout : public QMainWindow
{
    Q_OBJECT

public:
    Layout();

signals:
    void refreshChart();        //richiama il painter per eseguire il refresh al cambio di grafico

private slots:
    void emptyChart();          //crea un nuovo grafico vuoto di default
    void specChart1();          //crea il grafico specifico per il tipo1
    void specChart2();
    void specChart3();

    void changeLabelX();        //menu Edit
    void changeLabelY();
    void labelXShow();
    void labelYShow();
    void addRow();
    void removeRow();
    void addColumn();
    void removeColumn();
    void setChartTitle();
    void setSerieTitle();
    void setSerieAsLabel();
    void showLabelSerie();
    void changeSerieColor();

    void lineShow();            //menu view
    void barShow();
    void caosShow();
    void radarShow();

    void updateViewMenu();

    void openChart();           //apre un file
    bool saveChart();           //salva il grafico corrente
    void about();               //visualizza il menu about
    void close();               //chiude l'applicazione

private:
    void closeEvent(QCloseEvent *);

    bool iWant2Quit();          //chiede all'utente se vuole uscire senza salvare
    void createActions();
    void createMenus();
    void setChart(AbstractChart *);
    void setUpCentralWidget(bool);
    void setUpEmptyLayout(bool);
    void setMenuAvailable(bool);
    void clearMenuIcon();
    void setMenuIcon(QAction*);
    QString askText(QString, QString, QString);

    QMenu *fileMenu;                //MENU
    QMenu *newMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction *emptyAct;              //NEW actions
    QAction *spec1Act;
    QAction *spec2Act;
    QAction *spec3Act;

    QAction *changeLabelXAct;       //EDIT actions
    QAction *changeLabelYAct;
    QAction *labelXShowAct;
    QAction *showLabelSerieAct;
    QAction *labelYShowAct;
    QAction *addRowAct;
    QAction *removeRowAct;
    QAction *addColumnAct;
    QAction *removeColumnAct;
    QAction *setChartTitleAct;
    QAction *setSerieTitleAct;
    QAction *setSerieAsLabelAct;
    QAction *changeSerieColorAct;

    QAction *lineShowAct;           //VIEW actions
    QAction *barShowAct;
    QAction *caosShowAct;
    QAction *radarShowAct;

    QAction *openAct;               //FILE actions
    QAction *saveAct;
    QAction *exitAct;

    QAction *aboutAct;              //ABOUT actions

    bool showLogo;                  //flag per visualizzare il logo alla prima apertura

    QWidget *logoWidget;            //widget contenente il logo
    QWidget *chartEdit;             //widget contenente tabella + grafico
    QGridLayout *chartEditLayout;   //layout del precedente widget (chartEdit)

    AbstractChart *grafico;         //grafico generico
    DataTable *tabellaDati;         //tabella dati
    DrawChart *lavagna;             //lavagna per visualizzare il grafico

    bool firstEntry;

};

#endif
