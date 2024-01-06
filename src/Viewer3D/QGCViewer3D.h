#ifndef QGCVIEWER3D_H
#define QGCVIEWER3D_H

#include <QObject>
#include "metadatastreamer.h"
#include "qml_backend.h"
#include "osmparser.h"

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class QGCViewer3D : public QObject
{
    Q_OBJECT
public:
    explicit QGCViewer3D(QObject *parent = nullptr);

    static QGCViewer3D*  _map_app;   ///< Our own singleton. Should be reference directly by qgcViewer3D

    void initQml(QQmlApplicationEngine *qmlEngine, QObject *parent);

    OsmParser* mapLoader(){ return city_osm_map_loader;}
    QmlBackend* qmlBackend(){return qml_bk;}
    MetaDataStreamer* metaDataLoader(){return data_loader;}

private:
    OsmParser *city_osm_map_loader = nullptr;
    MetaDataStreamer *data_loader = nullptr;
    QmlBackend *qml_bk = nullptr;

signals:

};

/// @brief Returns the QGCViewer3D object singleton.
QGCViewer3D* qgcViewer3D(void);

#endif // QGCVIEWER3D_H

