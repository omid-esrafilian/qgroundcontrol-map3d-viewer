#include "Viewer3DManager.h"


Viewer3DManager::Viewer3DManager()
{
    const char * metadata_file_address = "map3d_viewer_meta_data.xml";
    _metadataLoader = new Viewer3DMetadata(metadata_file_address, this);
    _osmParser = new OsmParser();
    _qmlBackend  = new Viewer3DQmlBackend(this);

    _osmParser->setBuildingLevelHeight(3.5); // meters
    _qmlBackend->init(_metadataLoader, _osmParser);
}
