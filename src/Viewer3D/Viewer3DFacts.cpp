#include "Viewer3DFacts.h"
#include "QGCApplication.h"


Viewer3DFacts::Viewer3DFacts()
{
//    if(qgcApp()->viewer3D()){
//        _osmParser = qgcApp()->viewer3D()->osmParser();
//        _qmlBackend = qgcApp()->viewer3D()->qmlBackend();
//    }

    const char * metadata_file_address = "map3d_viewer_meta_data.xml";
    _metadataLoader = new Viewer3DMetadata(metadata_file_address, this);
    _osmParser = new OsmParser(this);
    _qmlBackend  = new Viewer3DQmlBackend(this);

    _osmParser->setBuildingLevelHeight(3.5); // meters
    _qmlBackend->init(_metadataLoader, _osmParser);
}
