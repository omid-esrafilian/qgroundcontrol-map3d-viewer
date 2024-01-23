#include "QGCViewer3D.h"

QGCViewer3D::QGCViewer3D(QObject *parent)
{
    const char * metadata_file_address = "map3d_viewer_meta_data.xml";
    _metadataLoader = new Viewer3DMetadata(metadata_file_address, parent);
    _osmParser = new OsmParser(this);
    _qmlBackend  = new Viewer3DQmlBackend(this);

}

void QGCViewer3D::init()
{
    _osmParser->setBuildingLevelHeight(3.5); // meters
    _qmlBackend->init();
}
