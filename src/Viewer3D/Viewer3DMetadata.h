#ifndef VIEWER3DMETADATA_H
#define VIEWER3DMETADATA_H

#include <QObject>
#include <QtXml>
#include <QTextStream>
#include <qxmlstream.h>

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class Viewer3DMetadata : public QObject
{
    Q_OBJECT
public:
    explicit Viewer3DMetadata(const char * filePath = nullptr, QObject *parent = nullptr);

    float heightBias;
    QString osmFilePath;

    bool loadMetaDataFile();
    bool updateMetaDataFile();

signals:


private:
    QString _metadataFilePath;

};

#endif // VIEWER3DMETADATA_H
