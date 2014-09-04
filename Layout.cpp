#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include <QScrollArea>

#include "Layout.h"
#include "FileManager.h"
#include "LineChart.h"
#include "BarChart.h"
#include "RadarChart.h"
#include "CaosChart.h"

void Layout::closeEvent(QCloseEvent *event)
{
    if(iWant2Quit())
        event->accept();
    else
        event->ignore();
}

void Layout::setChart(AbstractChart *newChart)
{
    AbstractChart *old;
    if(!firstEntry)old=grafico;
    grafico = newChart;                 //assegno il nuovo grafico
    lavagna->setChart2draw(grafico);    //assegno il grafico alla lavagna
    if(!firstEntry)delete old;         //cancello il grafico vecchio
}

void Layout::setUpCentralWidget(bool isLoadingAfile)
{
    //cancello il widget
    if(!firstEntry)
    delete chartEdit;

    //preparo il widget che mostra tabella + grafico
    chartEdit = new QWidget;
    //chartEdit->setStyleSheet("background-color: white;");

    if(isLoadingAfile){
        //se sto caricando un file creo una tabella 1x1
        tabellaDati = new DataTable("Empty qChart",1,1,chartEdit);
    }else{
        tabellaDati = new DataTable("Empty qChart",5,2,chartEdit);
    }

    if(!showLogo)setWindowTitle(QString("qCharts - ").append("Empty qChart"));

    AbstractChart *oldGrafico;
    if(!firstEntry)oldGrafico=grafico;
    grafico=new LineChart(tabellaDati);
    if(!firstEntry)delete oldGrafico;
    firstEntry=false;

    lavagna = new DrawChart(grafico,chartEdit);
    lavagna->setFixedSize(634,445); //H 465...

    QScrollArea *scrollArea = new QScrollArea(chartEdit);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(lavagna);

    //preparo il layout di chartEdit a griglia
    chartEditLayout = new QGridLayout ();
    chartEditLayout->setMargin(5);
    chartEditLayout->addWidget(tabellaDati,0,0);
    chartEditLayout->addWidget(scrollArea,0,1);
    chartEditLayout->setColumnStretch(0,39);
    chartEditLayout->setColumnStretch(1,71);
    chartEditLayout->setRowStretch(0,100);
    chartEdit->setLayout(chartEditLayout);

    //quando la tabella ha i dati pronti, richiamo il painter...
    connect(tabellaDati,SIGNAL(dataReady()),lavagna,SLOT(draw()));
    //oppure se cambio grafico in realTime eseguo il refresh
    connect(this,SIGNAL(refreshChart()),lavagna,SLOT(draw()));

}

void Layout::setUpEmptyLayout(bool isLoadingAfile)
{
    setUpCentralWidget(isLoadingAfile);
    setCentralWidget(chartEdit);
    saveAct->setEnabled(true);                          //ora permetto di salvare ed
    setMenuAvailable(true);                             //abilito le funzioni del menu
    if(!isLoadingAfile)tabellaDati->setLabelCol(0);     //per default imposto la prima colonna come etichetta
}

Layout::Layout() : showLogo(true), chartEdit(0), chartEditLayout(0), firstEntry(true)
{
    setWindowTitle("qCharts");
    setFixedSize(QSize(1000, 500));
    setWindowIcon(QPixmap(iconPath));

    //creo i menu e le azioni della MenuBar
    createActions();
    createMenus();

    saveAct->setEnabled(false);             //disabilitato per default
    setMenuAvailable(false);                //disabilito parte del menu

    //creo la label per il logo grande
    QLabel* Logo = new QLabel;
    Logo->setPixmap(QPixmap(logoPath));

    //preparo il widget che mostra il logo grande
    logoWidget = new QWidget;
    QVBoxLayout *logoLayout = new QVBoxLayout();
    logoLayout->setMargin(5);
    logoLayout->addWidget(Logo);
    logoWidget->setLayout(logoLayout);
    logoWidget->setStyleSheet("background-color: white;");

    //preparo il layout di default
    setUpCentralWidget(false);

    if(showLogo){   //se devo visualizzare il logo ***DEBUG***
        setCentralWidget(logoWidget);
    }else{
        setCentralWidget(chartEdit);           //cambio il widget centrale
        saveAct->setEnabled(true);             //ora permetto di salvare ed
        setMenuAvailable(true);                //abilito le funzioni del menu
    }

    setMenuIcon(lineShowAct);           //per default inizio con un grafico a linee

 }

//SLOTS:
void Layout::emptyChart()
{
    if(iWant2Quit()){   //se davvero voglio creare un grafico vuoto...
        if(showLogo)showLogo=false;     //resetto il flag
        setUpEmptyLayout(false);        //creo un nuovo layout vuoto false==5x2
        setMenuIcon(lineShowAct);

    }
    //se preme cancel non fa nulla...
}

void Layout::specChart1()   //BRAINSTORMING
{
    if(iWant2Quit()){   //se davvero voglio creare un grafico personalizzato...
        if(showLogo)showLogo=false;     //resetto il flag
        setUpEmptyLayout(true);         //creo un nuovo layout vuoto 1x1
        setMenuIcon(caosShowAct);       //***controllare! che sia il giusto grafico!
        caosShow();                     //visualizzo il grafico caos
        tabellaDati->titoloGrafico="Pantone Colors";
        setWindowTitle(QString("qCharts - ").append("Brainstorming"));
        tabellaDati->labelX="Colori";
        tabellaDati->labelY="num";
        tabellaDati->showLabelX=true;
        tabellaDati->showLabelY=false;
        tabellaDati->createTable(21,2);
        tabellaDati->setLabelCol(0);

        tabellaDati->setTitoloSerie(0,"Colors(L)");
        tabellaDati->setText(0,0,"Blue");
        tabellaDati->setText(1,0,"Sapphire");
        tabellaDati->setText(2,0,"Crystal");
        tabellaDati->setText(3,0,"Baby Blue");
        tabellaDati->setText(4,0,"Teal");
        tabellaDati->setText(5,0,"Emeral");
        tabellaDati->setText(6,0,"Green");
        tabellaDati->setText(7,0,"Burgundy");
        tabellaDati->setText(8,0,"Plum");
        tabellaDati->setText(9,0,"Grape");
        tabellaDati->setText(10,0,"Lilac");
        tabellaDati->setText(11,0,"Pink");
        tabellaDati->setText(12,0,"Raspberry");
        tabellaDati->setText(13,0,"Peach");
        tabellaDati->setText(14,0,"Cherry");
        tabellaDati->setText(15,0,"Red");
        tabellaDati->setText(16,0,"Caramel");
        tabellaDati->setText(17,0,"Cranberry");
        tabellaDati->setText(18,0,"Rust");
        tabellaDati->setText(19,0,"Gray");
        tabellaDati->setText(20,0,"Black");

        tabellaDati->setTitoloSerie(1,"Number");
        tabellaDati->setText(0,1,"295");
        tabellaDati->setText(1,1,"293");
        tabellaDati->setText(2,1,"299");
        tabellaDati->setText(3,1,"311");
        tabellaDati->setText(4,1,"325");
        tabellaDati->setText(5,1,"355");
        tabellaDati->setText(6,1,"348");
        tabellaDati->setText(7,1,"512");
        tabellaDati->setText(8,1,"267");
        tabellaDati->setText(9,1,"240");
        tabellaDati->setText(10,1,"522");
        tabellaDati->setText(11,1,"190");
        tabellaDati->setText(12,1,"225");
        tabellaDati->setText(13,1,"177");
        tabellaDati->setText(14,1,"186");
        tabellaDati->setText(15,1,"185");
        tabellaDati->setText(16,1,"1595");
        tabellaDati->setText(17,1,"187");
        tabellaDati->setText(18,1,"483");
        tabellaDati->setText(19,1,"445");
        tabellaDati->setText(20,1,"-");
    }
    //se preme cancel non fa nulla...
}

void Layout::specChart2() //DATA LOGGER
{
    if(iWant2Quit()){   //se davvero voglio creare un grafico personalizzato...
        if(showLogo)showLogo=false;     //resetto il flag
        setUpEmptyLayout(true);        //creo un nuovo layout vuoto 1x1
        setMenuIcon(lineShowAct);
        lineShow();
        tabellaDati->titoloGrafico="Rilevamento meteo";
        setWindowTitle(QString("qCharts - ").append("Data Logger"));
        tabellaDati->labelX="Anno 2012";
        tabellaDati->labelY="gradi C";
        tabellaDati->showLabelY=true;
        tabellaDati->createTable(12,4);
        tabellaDati->setLabelCol(0);

        tabellaDati->setTitoloSerie(0,"Mesi(L)");
        tabellaDati->setText(0,0,"Gennaio");
        tabellaDati->setText(1,0,"Febbraio");
        tabellaDati->setText(2,0,"Marzo");
        tabellaDati->setText(3,0,"Aprile");
        tabellaDati->setText(4,0,"Maggio");
        tabellaDati->setText(5,0,"Giugno");
        tabellaDati->setText(6,0,"Luglio");
        tabellaDati->setText(7,0,"Agosto");
        tabellaDati->setText(8,0,"Settembre");
        tabellaDati->setText(9,0,"Ottobre");
        tabellaDati->setText(10,0,"Novembre");
        tabellaDati->setText(11,0,"Dicembre");

        tabellaDati->setTitoloSerie(1,"Arabba");
        tabellaDati->setText(0,1,"-10");
        tabellaDati->setText(1,1,"5");
        tabellaDati->setText(2,1,"8.5");
        tabellaDati->setText(3,1,"13");
        tabellaDati->setText(4,1,"15");
        tabellaDati->setText(5,1,"17");
        tabellaDati->setText(6,1,"20");
        tabellaDati->setText(7,1,"25");
        tabellaDati->setText(8,1,"21.5");
        tabellaDati->setText(9,1,"15");
        tabellaDati->setText(10,1,"8");
        tabellaDati->setText(11,1,"-2");

        tabellaDati->setTitoloSerie(2,"Padova");
        tabellaDati->setText(0,2,"5");
        tabellaDati->setText(1,2,"6.5");
        tabellaDati->setText(2,2,"8");
        tabellaDati->setText(3,2,"13");
        tabellaDati->setText(4,2,"17");
        tabellaDati->setText(5,2,"24");
        tabellaDati->setText(6,2,"29");
        tabellaDati->setText(7,2,"34");
        tabellaDati->setText(8,2,"28.5");
        tabellaDati->setText(9,2,"22");
        tabellaDati->setText(10,2,"17");
        tabellaDati->setText(11,2,"9.5");

        tabellaDati->setTitoloSerie(3,"Sahara");
        tabellaDati->setText(0,3,"45");
        tabellaDati->setText(1,3,"46");
        tabellaDati->setText(2,3,"43.5");
        tabellaDati->setText(3,3,"46");
        tabellaDati->setText(4,3,"48");
        tabellaDati->setText(5,3,"46.5");
        tabellaDati->setText(6,3,"50");
        tabellaDati->setText(7,3,"47.5");
        tabellaDati->setText(8,3,"46");
        tabellaDati->setText(9,3,"47");
        tabellaDati->setText(10,3,"45");
        tabellaDati->setText(11,3,"44.5");

    }
    //se preme cancel non fa nulla...
}

void Layout::specChart3() //FOOTBALL MANAGER
{
    if(iWant2Quit()){   //se davvero voglio creare un grafico personalizzato...
        if(showLogo)showLogo=false;     //resetto il flag
        setUpEmptyLayout(true);         //creo un nuovo layout vuoto 1x1
        setMenuIcon(radarShowAct);     //***controllare! che sia il giusto grafico!
        radarShow();
        tabellaDati->titoloGrafico="Giocatori Nazionale Brasile";
        setWindowTitle(QString("qCharts - ").append("Football Manager"));
        tabellaDati->labelX="Prestazioni";
        tabellaDati->labelY="";
        tabellaDati->showLabelY=false;
        tabellaDati->createTable(5,3);
        tabellaDati->setLabelCol(0);

        tabellaDati->setTitoloSerie(0,"Label(L)");
        tabellaDati->setText(0,0,"Potenza");
        tabellaDati->setText(1,0,"Precisione");
        tabellaDati->setText(2,0,"Difesa");
        tabellaDati->setText(3,0,"Attacco");
        tabellaDati->setText(4,0,"Resistenza");

        tabellaDati->setTitoloSerie(1,"Ronaldinho");
        tabellaDati->setText(0,1,"90");
        tabellaDati->setText(1,1,"75");
        tabellaDati->setText(2,1,"50");
        tabellaDati->setText(3,1,"78");
        tabellaDati->setText(4,1,"58");

        tabellaDati->setTitoloSerie(2,"Paulinho");
        tabellaDati->setText(0,2,"80");
        tabellaDati->setText(1,2,"50");
        tabellaDati->setText(2,2,"72");
        tabellaDati->setText(3,2,"49");
        tabellaDati->setText(4,2,"90");
    }
    //se preme cancel non fa nulla...
}

//MENU EDIT
void Layout::addRow()
{
    tabellaDati->addRow();
    removeRowAct->setEnabled(true);
    if(tabellaDati->nrighe()==1000)
        addRowAct->setEnabled(false);

    emit refreshChart();            //eseguo il refresh
}

void Layout::removeRow()
{
    tabellaDati->removeRow();
    addRowAct->setEnabled(true);
    if(tabellaDati->nrighe()==1)
        removeRowAct->setEnabled(false);

    emit refreshChart();            //eseguo il refresh
}

void Layout::addColumn()
{
    tabellaDati->addColumn();
    removeColumnAct->setEnabled(true);
    if(tabellaDati->ncolonne()==1000)
        addColumnAct->setEnabled(false);

    emit refreshChart();            //eseguo il refresh
}

void Layout::removeColumn()
{
    tabellaDati->removeColumn();
    addColumnAct->setEnabled(true);
    if(tabellaDati->ncolonne()==1)
        removeColumnAct->setEnabled(false);

    emit refreshChart();            //eseguo il refresh
}

void Layout::changeLabelX()
{
    tabellaDati->labelX=askText("Set label","asse X:",tabellaDati->labelX);
    emit refreshChart();            //eseguo il refresh
}

void Layout::changeLabelY()
{
    tabellaDati->labelY=askText("Set label","asse Y:",tabellaDati->labelY);
    emit refreshChart();            //eseguo il refresh
}

void Layout::labelXShow()
{
    tabellaDati->showLabelX=!(tabellaDati->showLabelX);
    emit refreshChart();            //eseguo il refresh
}

void Layout::labelYShow()
{
    tabellaDati->showLabelY=!(tabellaDati->showLabelY);
    emit refreshChart();            //eseguo il refresh
}

void Layout::setChartTitle()
{
    tabellaDati->titoloGrafico=askText("Set qChart Title","title:",tabellaDati->titoloGrafico);
    setWindowTitle(QString("qCharts - ").append(tabellaDati->titoloGrafico));
    emit refreshChart();            //eseguo il refresh
}

void Layout::setSerieTitle()
{
    int i=tabellaDati->getSelectedColumn();
    if(i>=0)
    {
        QString newTitleS=askText(QString("Set Serie%1 Title").arg(i),"title:",tabellaDati->getTitoloSerie(i));
        if(tabellaDati->isLabel(i))
        {                                      //se la colonna che sto modificando era etichettata controllo che
            newTitleS=newTitleS.remove("(L)");
            newTitleS=newTitleS.append("(L)"); //se cambio il nome ad una serie etichettata costringo ad usare (L)
        }
        tabellaDati->setTitoloSerie(i,newTitleS);
        emit refreshChart();            //eseguo il refresh
    }else{
        QMessageBox::critical(this,tr("Errore"),"devi selezionare una colonna<br />oppure un elemento della colonna che vuoi modificare.");
    }
}

void Layout::changeSerieColor()
{
    int i=tabellaDati->getSelectedColumn();
    if(i>=0)
    {
        int r=rand()%256;
        int g=rand()%256;
        int b=rand()%256;
        tabellaDati->setColoreSerie(i,QColor(r,g,b));
        emit refreshChart();            //eseguo il refresh
    }else{
        QMessageBox::critical(this,tr("Errore"),"devi selezionare una colonna<br />oppure un elemento della colonna che vuoi modificare.");
    }
}

void Layout::setSerieAsLabel()
{
    int i=tabellaDati->getSelectedColumn();
    if(i>=0)
    {
        tabellaDati->setLabelCol(i);
        emit refreshChart();            //eseguo il refresh
    }else{
        QMessageBox::critical(this,tr("Errore"),"devi selezionare la colonna<br />oppure un elemento della colonna che vuoi modificare.");
    }
}

void Layout::showLabelSerie()
{
    tabellaDati->showLabelSerie=!(tabellaDati->showLabelSerie);
    emit refreshChart();            //eseguo il refresh
}

QString Layout::askText(QString winTitle, QString description, QString actualValue)
{
    bool ok;
    QString newText = QInputDialog::getText(this, winTitle,
                                         description, QLineEdit::Normal,
                                         actualValue, &ok);
    if(!ok){
        QMessageBox::critical(this,tr("Errore"),"inserimento FALLITO!");
        return actualValue;
    }else{
        return newText;
    }
}

//MENU VIEW
void Layout::lineShow()
{
    setMenuIcon(lineShowAct);
    tabellaDati->tipoGrafico=LINE;
    setChart(new LineChart(tabellaDati)); //creo un grafico linea con i dati
    emit refreshChart();            //eseguo il refresh
}

void Layout::barShow()
{
    setMenuIcon(barShowAct);
    tabellaDati->tipoGrafico=BAR;
    setChart(new BarChart(tabellaDati));  //creo un grafico bar con i dati
    emit refreshChart();            //eseguo il refresh
}

void Layout::caosShow()
{
   setMenuIcon(caosShowAct);
   tabellaDati->tipoGrafico=CAOS;
   setChart(new CaosChart(tabellaDati));
   emit refreshChart();            //eseguo il refresh
}

void Layout::radarShow()
{
    setMenuIcon(radarShowAct);
    tabellaDati->tipoGrafico=RADAR;
    setChart(new RadarChart(tabellaDati));
    emit refreshChart();            //eseguo il refresh
}

void Layout::openChart()
{
    if(iWant2Quit()){   //chiedo se non voglio salvare il lavoro corrente prima
                        //di aprire un file

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open qChart"),
                                                         "/",
                                                         tr("qChart (*.qch)"));

        if(fileName!=NULL)  //se l'utente ha selezionato un file
        {
            if(showLogo)showLogo=false;     //resetto il flag
            setUpEmptyLayout(true);         //apro un empty layout 1x1
            if(FileManager::openFile(tabellaDati,fileName))
            {
                //apertura file eseguita con successo
                emit refreshChart();
            }else{
                //apertura file fallito
                switch( QMessageBox::critical(this,tr("Errore"),
                        tr("Problemi con il file o con il parser XML<br />"
                           "I dati potrebbero essere corrotti/incompleti.<br /><br />"
                           "Vuoi continuare lo stesso?"),
                           QMessageBox::Yes |
                           QMessageBox::No,
                           QMessageBox::No ) )
                {
                case QMessageBox::Yes:
                    break;
                default:
                    QMessageBox::information(this,tr("Open qChart"),"Verr&#224; caricato un layout vuoto.<br />");
                    setUpEmptyLayout(false);             //creo un nuovo layout vuoto
                    break;
                }

            }
        }
        emit updateViewMenu();          //aggiorno il menu view
        emit refreshChart();            //eseguo il refresh del grafico
        setWindowTitle(QString("qCharts - ").append(tabellaDati->titoloGrafico));
    }
    //se preme cancel non fa nulla...
}

bool Layout::saveChart()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save qChart"),
                                                    QString("%1.qch").arg(tabellaDati->titoloGrafico),
                                                    tr("qChart (*.qch)"));
    if(fileName!=NULL)
    {
        if(FileManager::saveFile(tabellaDati,fileName)){
            return true; //salvataggio riuscito
        }else{
            QMessageBox::critical(this,tr("Errore"),"Salvataggio FALLITO!");
        }
    }
    return false;  //l'utente ha premuto CANCEL o problema di scrittura
}

bool Layout::iWant2Quit()
{
    if(!showLogo){  //se provengo da schermate secondarie...
    switch( QMessageBox::question(
                    this,
                    tr("Sei sicuro?"),
                    tr("Vuoi salvare il tuo lavoro?"),

                    QMessageBox::Yes |
                    QMessageBox::No |
                    QMessageBox::Cancel,

                    QMessageBox::Cancel ) )
        {
          case QMessageBox::Yes:
            return saveChart();
            break;
          case QMessageBox::No:
            return true;   //qui l'utente vuole uscire senza salvare
            break;
          case QMessageBox::Cancel:
            return false;   //non esce
            break;
          default:
            return false;
            break;
        }
    }else{
        return true;
    }
}

void Layout::close()
{
    if(iWant2Quit())
        qApp->quit();
}

void Layout::about()
{
    QMessageBox::about(
                this, "About",
                "<b>qCharts</b><br />"
                "di Alberto Garbui<br /><br />"
                "Universit&#224; degli Studi di Padova<br />"
                "Corso di Laurea Triennale in Informatica<br />"
                "Progetto del corso di Programmazione ad Oggetti<br />A.s. 2012-2013");
}

void Layout::updateViewMenu()
{
    switch(tabellaDati->tipoGrafico)
    {
    case LINE:
        setMenuIcon(lineShowAct);
        setChart(new LineChart(tabellaDati));
        break;
    case BAR:
        setMenuIcon(barShowAct);
        setChart(new BarChart(tabellaDati));
        break;
    case CAOS:
        setMenuIcon(caosShowAct);
        setChart(new CaosChart(tabellaDati));
        break;
    case RADAR:
        setMenuIcon(radarShowAct);
        setChart(new RadarChart(tabellaDati));
        break;
    }
}

void Layout::setMenuAvailable(bool flag)
{
    //Edit MENU
    addRowAct->setEnabled(flag);
    removeRowAct->setEnabled(flag);
    addColumnAct->setEnabled(flag);
    removeColumnAct->setEnabled(flag);
    changeLabelXAct->setEnabled(flag);
    changeLabelYAct->setEnabled(flag);
    labelXShowAct->setEnabled(flag);
    labelYShowAct->setEnabled(flag);
    setChartTitleAct->setEnabled(flag);
    setSerieTitleAct->setEnabled(flag);
    setSerieAsLabelAct->setEnabled(flag);
    showLabelSerieAct->setEnabled(flag);
    changeSerieColorAct->setEnabled(flag);

    //View MENU
    lineShowAct->setEnabled(flag);
    barShowAct->setEnabled(flag);
    caosShowAct->setEnabled(flag);
    radarShowAct->setEnabled(flag);
}

void Layout::clearMenuIcon()
{
    lineShowAct->setIconVisibleInMenu(false);
    barShowAct->setIconVisibleInMenu(false);
    caosShowAct->setIconVisibleInMenu(false);
    radarShowAct->setIconVisibleInMenu(false);
}

void Layout::setMenuIcon(QAction *act)
{
    clearMenuIcon();
    act->setIconVisibleInMenu(true);
}

//creo le varie azioni del menu, assegnando i vari connect...
void Layout::createActions()
{
    //***NEW MENU
    emptyAct = new QAction("Empty Chart", this);
    emptyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(emptyAct, SIGNAL(triggered()), this, SLOT(emptyChart()));

    spec1Act = new QAction("Brainstorming", this);
    connect(spec1Act, SIGNAL(triggered()), this, SLOT(specChart1()));

    spec2Act = new QAction("Data logger", this);
    connect(spec2Act, SIGNAL(triggered()), this, SLOT(specChart2()));

    spec3Act = new QAction("Football manager", this);
    connect(spec3Act, SIGNAL(triggered()), this, SLOT(specChart3()));

    //***EDIT MENU
    addRowAct = new QAction("Aggiungi riga", this);
    addRowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(addRowAct, SIGNAL(triggered()), this, SLOT(addRow()));

    removeRowAct = new QAction("Rimuovi ultima riga", this);
    removeRowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    connect(removeRowAct, SIGNAL(triggered()), this, SLOT(removeRow()));

    addColumnAct = new QAction("Aggiungi colonna", this);
    addColumnAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(addColumnAct, SIGNAL(triggered()), this, SLOT(addColumn()));

    removeColumnAct = new QAction("Rimuovi ultima colonna", this);
    removeColumnAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    connect(removeColumnAct, SIGNAL(triggered()), this, SLOT(removeColumn()));

    setChartTitleAct = new QAction("Cambia nome grafico", this);
    connect(setChartTitleAct, SIGNAL(triggered()), this, SLOT(setChartTitle()));

    setSerieTitleAct = new QAction("Cambia nome serie", this);
    connect(setSerieTitleAct, SIGNAL(triggered()), this, SLOT(setSerieTitle()));

    changeSerieColorAct = new QAction("Cambia colore serie (random)", this);
    changeSerieColorAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    connect(changeSerieColorAct, SIGNAL(triggered()), this, SLOT(changeSerieColor()));

    setSerieAsLabelAct = new QAction("Imposta serie come etichette (on/off)", this);
    setSerieAsLabelAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(setSerieAsLabelAct, SIGNAL(triggered()), this, SLOT(setSerieAsLabel()));

    showLabelSerieAct = new QAction("Visualizza/Nascondi serie etichettata (on/off)", this);
    showLabelSerieAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
    connect(showLabelSerieAct, SIGNAL(triggered()), this, SLOT(showLabelSerie()));

    changeLabelXAct = new QAction("Cambia etichetta asse X", this);
    connect(changeLabelXAct, SIGNAL(triggered()), this, SLOT(changeLabelX()));

    changeLabelYAct = new QAction("Cambia etichetta asse Y", this);
    connect(changeLabelYAct, SIGNAL(triggered()), this, SLOT(changeLabelY()));

    labelXShowAct = new QAction("Visualizza/Nascondi etichetta asse X", this);
    labelXShowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    connect(labelXShowAct, SIGNAL(triggered()), this, SLOT(labelXShow()));

    labelYShowAct = new QAction("Visualizza/Nascondi etichetta asse Y", this);
    labelYShowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    connect(labelYShowAct, SIGNAL(triggered()), this, SLOT(labelYShow()));

    //***VIEW MENU
    lineShowAct = new QAction(QIcon(menuArrowPath),"LineChart", this);
    lineShowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    connect(lineShowAct, SIGNAL(triggered()), this, SLOT(lineShow()));

    barShowAct = new QAction(QIcon(menuArrowPath),"BarChart", this);
    barShowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    connect(barShowAct, SIGNAL(triggered()), this, SLOT(barShow()));

    caosShowAct = new QAction(QIcon(menuArrowPath),"CaosChart", this);
    caosShowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    connect(caosShowAct, SIGNAL(triggered()), this, SLOT(caosShow()));

    radarShowAct = new QAction(QIcon(menuArrowPath),"RadarChart", this);
    radarShowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));
    connect(radarShowAct, SIGNAL(triggered()), this, SLOT(radarShow()));

    //***FILE MENU
    openAct = new QAction("Open", this);
    openAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openChart()));

    saveAct = new QAction("Save", this);
    saveAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveChart()));

    exitAct = new QAction("Exit", this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    //***ABOUT MENU
    aboutAct = new QAction("About", this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

//creo la struttura del menuBar iniziale
void Layout::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
        newMenu = fileMenu->addMenu("New");                 //menu new
            newMenu->addAction(emptyAct);                   //emptyChart
            newMenu->addSeparator();                        //---
            newMenu->addAction(spec1Act);                   //spec1
            newMenu->addAction(spec2Act);                   //spec2
            newMenu->addAction(spec3Act);                   //spec3
            //...

        fileMenu->addAction(openAct);          //apri file
        fileMenu->addAction(saveAct);          //salva file
        fileMenu->addSeparator();              // ---------
        fileMenu->addAction(exitAct);          //exit

   editMenu = menuBar()->addMenu("Edit");     //Edit menu
        editMenu->addAction(addRowAct);         //aggiungi riga
        editMenu->addAction(removeRowAct);      //rimuovi riga
        editMenu->addAction(addColumnAct);      //aggiungi colonna
        editMenu->addAction(removeColumnAct);   //rimuovi colonna
        editMenu->addSeparator();               //---------
        editMenu->addAction(setChartTitleAct); //cambia il titolo del grafico
        editMenu->addAction(setSerieTitleAct); //cambia il titolo alla serie selezionata
        editMenu->addAction(changeSerieColorAct); //cambia il colore alla serie
        editMenu->addAction(setSerieAsLabelAct);//imposta la serie selezionata come etichette X
        editMenu->addAction(showLabelSerieAct); //visualizza/nascondi serie etichettata
        editMenu->addSeparator();               //---------
        editMenu->addAction(changeLabelXAct);  //cambia etichetta asse X
        editMenu->addAction(labelXShowAct);    //visualizza/nascondi etichette asse X
        editMenu->addSeparator();              //---------
        editMenu->addAction(changeLabelYAct);  //cambia etichetta asse Y
        editMenu->addAction(labelYShowAct);    //visualizza/nascondi etichette asse Y

   viewMenu = menuBar()->addMenu("View");     //View menu
        viewMenu->addAction(lineShowAct);     //LineChart
        viewMenu->addAction(barShowAct);
        viewMenu->addAction(caosShowAct);
        viewMenu->addAction(radarShowAct);

    helpMenu = menuBar()->addMenu("Help");     //menu help
        helpMenu->addAction(aboutAct);             //about
}
