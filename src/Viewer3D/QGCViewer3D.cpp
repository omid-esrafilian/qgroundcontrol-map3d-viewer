#include "QGCViewer3D.h"
QGCViewer3D* QGCViewer3D::_map_app = nullptr;

QGCViewer3D* qgcViewer3D(void)
{
    return QGCViewer3D::_map_app;
}

QGCViewer3D::QGCViewer3D(QObject *parent)
    : QObject{parent}
{
    _map_app = this;

    const char * metadata_file_address = "map3d_viewer_meta_data.xml";
    data_loader = new MetaDataStreamer(metadata_file_address, parent);

    city_osm_map_loader = new OsmParser(parent);
    city_osm_map_loader->setBuildingLevelHeight(3.5); // meters
}

void QGCViewer3D::initQml(QQmlApplicationEngine *qmlEngine, QObject *parent)
{
    qml_bk = new QmlBackend(parent);

    qmlEngine->rootContext()->setContextProperty("Viewer3DOsmReader", qgcViewer3D()->mapLoader());
    qmlEngine->rootContext()->setContextProperty("Viewer3DQmlBackend", qgcViewer3D()->qmlBackend());
}
