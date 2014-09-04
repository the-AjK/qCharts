#include <QFileDialog>
#include <QXmlStreamWriter>
#include "FileManager.h"
#include "Layout.h"

bool FileManager::saveFile(const DataTable *dati, QString filename)
{
    QString value;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("QCHART");
    xmlWriter.writeTextElement("titoloGrafico", dati->titoloGrafico);
    xmlWriter.writeTextElement("labelX", dati->labelX);
    xmlWriter.writeTextElement("showLabelX", value.setNum(dati->showLabelX));
    xmlWriter.writeTextElement("labelY", dati->labelY);
    xmlWriter.writeTextElement("showLabelY", value.setNum(dati->showLabelY));
    xmlWriter.writeTextElement("chartType", value.setNum(dati->tipoGrafico));
    xmlWriter.writeTextElement("rows", value.setNum(dati->nrighe()));
    xmlWriter.writeTextElement("columns", value.setNum(dati->ncolonne()));

    for(int i=0; i<dati->ncolonne(); i++)   //per ogni serie
    {
        xmlWriter.writeTextElement(QString("titoloSerie%1").arg(i), dati->getTitoloSerie(i));
        xmlWriter.writeTextElement(QString("coloreSerie%1").arg(i), dati->getColoreSerie(i).name());
        for(int j=0; j<dati->nrighe(); j++)
            xmlWriter.writeTextElement(QString("serie%1dato%2").arg(i).arg(j),dati->getText(j,i));
    }

    xmlWriter.writeTextElement("labelColumn", value.setNum(dati->labelColumn));
    xmlWriter.writeTextElement("showLabelSerie", value.setNum(dati->showLabelSerie));
    xmlWriter.writeEndElement(); //ELEMENT QCHART
    xmlWriter.writeEndDocument();
    file.close();
    return true;
}

bool FileManager::openFile(DataTable *dati, QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false; //impossibile leggere il file
    }
    QXmlStreamReader Rxml;
    Rxml.setDevice(&file);
    Rxml.readNext();

    int i=0; //contatore serie
    int j=0;

    while(!Rxml.atEnd())
    {
        if(Rxml.isStartElement())
        {
            if(Rxml.name() == "QCHART")
            {
                Rxml.readNext();
            }
            else if(Rxml.name() == "titoloGrafico")
            {
                dati->titoloGrafico=readTextValue(Rxml);
            }
            else if(Rxml.name() == "labelX")
            {
                dati->labelX=readTextValue(Rxml);
            }
            else if(Rxml.name() == "showLabelX")
            {
                dati->showLabelX=readTextValue(Rxml).toUInt();
            }
            else if(Rxml.name() == "labelY")
            {
                dati->labelY=readTextValue(Rxml);
            }
            else if(Rxml.name() == "showLabelY")
            {
                dati->showLabelY=readTextValue(Rxml).toUInt();
            }
            else if(Rxml.name() == "showLabelSerie")
            {
                dati->showLabelSerie=readTextValue(Rxml).toUInt();
            }
            else if(Rxml.name() == "labelColumn")
            {
                int labelC=readTextValue(Rxml).toInt();
                if(labelC>=0)
                {
                    dati->setLabelCol(labelC);                      //setLabel aggiunge un "(L)"
                    QString newTitleL=dati->getTitoloSerie(labelC); //quindi carico il nome
                    newTitleL=newTitleL.remove("(L)");              //rimuovo tutte le occorrenze
                    newTitleL=newTitleL.append("(L)");              //e ne aggiungo una sola
                    dati->setTitoloSerie(labelC,newTitleL);         //aggiorno il titolo
                }
            }
            else if(Rxml.name() == "chartType")
            {
                dati->tipoGrafico=static_cast<ChartType>(readTextValue(Rxml).toUInt());
            }
            else if(Rxml.name() == "rows")
            {
                int righe=readTextValue(Rxml).toUInt();
                for(int i=0;i<righe-1;i++)dati->addRow();
            }
            else if(Rxml.name() == "columns")
            {
                int colonne=readTextValue(Rxml).toUInt();
                for(int i=0;i<colonne-1;i++)dati->addColumn();
            }
            else if(Rxml.name() == QString("titoloSerie%1").arg(i))
            {
                dati->setTitoloSerie(i,readTextValue(Rxml));
            }
            else if(Rxml.name() == QString("coloreSerie%1").arg(i))
            {
                dati->setColoreSerie(i,QColor(readTextValue(Rxml)));
            }
            else if(Rxml.name() == QString("serie%1dato%2").arg(i).arg(j))
            {
                dati->setText(j,i,readTextValue(Rxml));
                //se ho scritto l'ultimo elemento della tabella, azzero il contatore j(righe)
                //ed incremento il contatore i(serie)
                if(++j==dati->nrighe()){j=0;i++;}
            }
            else
            {
              Rxml.raiseError(QObject::tr("Not a qChart file!"));
            }
        }
        else
        {
            Rxml.readNext();
        }
    }
    file.close();
    if (Rxml.hasError())
    {
        return false; //parse error
    }
    else if (file.error() != QFile::NoError)
    {
        return false; //impossibile leggere il file
    }
    return true;
}

QString FileManager::readTextValue(QXmlStreamReader &Rxml)
{
    QString text="";
    while(!Rxml.atEnd())
    {
        if(Rxml.isEndElement())
        {
            Rxml.readNext();
            break;
        }
        else if(Rxml.isStartElement())
        {
            text=Rxml.readElementText();
            Rxml.readNext();
            break;
        }
        else if(Rxml.isCharacters())
        {
            Rxml.readNext();
        }
        else
        {
            Rxml.readNext();
        }
    }
    return text;
}
