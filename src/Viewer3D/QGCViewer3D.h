#ifndef QGCVIEWER3D_H
#define QGCVIEWER3D_H

#include <QObject>
#include "Viewer3DMetadata.h"
#include "Viewer3DQmlBackend.h"
#include "OsmParser.h"
#include "Viewer3DFacts.h"

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class QGCViewer3D : public Viewer3DFacts
{
    Q_OBJECT
public:
    explicit QGCViewer3D(QObject *parent = nullptr);

    void init();

    OsmParser* osmParser(){ return _osmParser;}
    Viewer3DQmlBackend* qmlBackend(){return _qmlBackend;}
    Viewer3DMetadata* metaDataLoader(){return _metadataLoader;}

private:
    Viewer3DMetadata *_metadataLoader = nullptr;

};

#endif // QGCVIEWER3D_H

