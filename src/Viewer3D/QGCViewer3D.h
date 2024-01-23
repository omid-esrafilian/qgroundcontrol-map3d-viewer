#ifndef QGCVIEWER3D_H
#define QGCVIEWER3D_H

#include <QObject>
#include "Viewer3DMetadata.h"
#include "Viewer3DQmlBackend.h"
#include "OsmParser.h"

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class QGCViewer3D : public QObject
{
    Q_OBJECT
public:
    explicit QGCViewer3D(QObject *parent = nullptr);

    void initQml(QQmlApplicationEngine *qmlEngine, QObject *parent);

    OsmParser* mapLoader(){ return city_osm_map_loader;}
    Viewer3DQmlBackend* qmlBackend(){return qml_bk;}
    Viewer3DMetadata* metaDataLoader(){return data_loader;}

private:
    OsmParser *city_osm_map_loader = nullptr;
    Viewer3DMetadata *data_loader = nullptr;
    Viewer3DQmlBackend *qml_bk = nullptr;

signals:

};

#endif // QGCVIEWER3D_H

