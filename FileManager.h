#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QXmlStreamReader>
#include "DataTable.h"

class FileManager{
public:
    static bool saveFile(const DataTable *, QString);
    static bool openFile(DataTable *, QString);

private:
    static QString readTextValue(QXmlStreamReader &);
};

#endif
