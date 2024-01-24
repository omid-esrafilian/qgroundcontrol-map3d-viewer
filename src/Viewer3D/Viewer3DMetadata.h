#ifndef VIEWER3DMETADATA_H
#define VIEWER3DMETADATA_H

#include <QObject>
#include <QtXml>
#include <QTextStream>
#include <qxmlstream.h>

#include "Viewer3DCppVariableTypes.h"

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class Viewer3DMetadata : public QObject
{
    Q_OBJECT
public:
    explicit Viewer3DMetadata(const char * data_file_path = nullptr, QObject *parent = nullptr);

    Viewer3DMetadataType  meta_data;

    bool loadMetaDataFile();
    bool updateMetaDataFile();

signals:


private:
    QString metadata_file_path;
};

#endif // VIEWER3DMETADATA_H
