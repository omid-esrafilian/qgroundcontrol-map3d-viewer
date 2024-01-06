#ifndef METADATASTREAMER_H
#define METADATASTREAMER_H

#include "cpp_variable_types.h"
#include <QObject>
#include <QtXml>
#include <QTextStream>
#include <qxmlstream.h>

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class MetaDataStreamer : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataStreamer(const char * data_file_path = nullptr, QObject *parent = nullptr);

    meta_data_type meta_data;

    bool loadMetaDataFile();
    bool updateMetaDataFile();

signals:


private:
    QString metadata_file_path;
};

#endif // METADATASTREAMER_H
