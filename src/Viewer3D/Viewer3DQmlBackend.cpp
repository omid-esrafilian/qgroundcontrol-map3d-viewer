#include "Viewer3DQmlBackend.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QUrl>
#include "QGCViewer3D.h"
#include "QGCApplication.h"

Viewer3DQmlBackend::Viewer3DQmlBackend(QObject *parent)
    : QObject{parent}
{
}

void Viewer3DQmlBackend::init(Viewer3DMetadata * metadataThr, OsmParser* osmThr)
{
    initMetadata(metadataThr);
    initOsmMapLoader(osmThr);
}

void Viewer3DQmlBackend::setGpsRef(const QGeoCoordinate &gpsRef)
{
    if(_gpsRef == gpsRef){
        return;
    }

    _gpsRef = gpsRef;
    emit gpsRefChanged();
}

void Viewer3DQmlBackend::initMetadata(Viewer3DMetadata * metadataThr)
{
    if(metadataThr) {
        _metadataThread = metadataThr;
    }else {
        _metadataThread = qgcApp()->viewer3D()->metaDataLoader();
    }
    _metadataThread->loadMetaDataFile();

    _heightBias = _metadataThread->heightBias;
    _osmFilePath = _metadataThread->osmFilePath;

    emit heightBiasChanged();
    emit cityMapPathChanged();

    connect(this, &Viewer3DQmlBackend::cityMapPathChanged, this, &Viewer3DQmlBackend::_cityMapPathChangedEvent);
    connect(this, &Viewer3DQmlBackend::heightBiasChanged, this, &Viewer3DQmlBackend::_heightBiasChangedEvent);
}

void Viewer3DQmlBackend::initOsmMapLoader(OsmParser* osmThr)
{
    if(osmThr){
        _osmParserThread = osmThr;
    }else{
        _osmParserThread = qgcApp()->viewer3D()->osmParser();
    }
    connect(_osmParserThread, &OsmParser::gpsRefChanged, this, &Viewer3DQmlBackend::_gpsRefChangedEvent);
}

void Viewer3DQmlBackend::_gpsRefChangedEvent(QGeoCoordinate newGpsRef)
{   
    _gpsRef = newGpsRef;

    emit gpsRefChanged();

    qDebug() << "3D map gps reference:" << _gpsRef.latitude() << _gpsRef.longitude() << _gpsRef.altitude();
}

void Viewer3DQmlBackend::_heightBiasChangedEvent()
{
    _osmParserThread->setBuildingMapHeightBias(_heightBias);
    _metadataThread->heightBias = _heightBias;
    _metadataThread->updateMetaDataFile();
}

void Viewer3DQmlBackend::_cityMapPathChangedEvent()
{
    _metadataThread->osmFilePath = _osmFilePath;
    _metadataThread->updateMetaDataFile();
}
