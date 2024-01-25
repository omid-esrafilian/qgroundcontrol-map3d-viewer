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

void Viewer3DQmlBackend::init()
{
    initMetadata();
    initOsmMapLoader();
}

void Viewer3DQmlBackend::setGpsRef(const QGeoCoordinate &gpsRef)
{
    if(_gpsRef == gpsRef){
        return;
    }

    _gpsRef = gpsRef;
    emit gpsRefChanged();
}

void Viewer3DQmlBackend::initMetadata()
{
    metadata_loader_thr = qgcApp()->viewer3D()->metaDataLoader();
    metadata_loader_thr->loadMetaDataFile();

    _heightBias = metadata_loader_thr->heightBias;
    _osmFilePath = metadata_loader_thr->osmFilePath;

    emit heightBiasChanged();
    emit cityMapPathChanged();

    connect(this, &Viewer3DQmlBackend::cityMapPathChanged, this, &Viewer3DQmlBackend::_cityMapPathChangedEvent);
    connect(this, &Viewer3DQmlBackend::heightBiasChanged, this, &Viewer3DQmlBackend::_heightBiasChangedEvent);
}

void Viewer3DQmlBackend::initOsmMapLoader()
{
    bld_map_reader_thr = qgcApp()->viewer3D()->osmParser();
    connect(bld_map_reader_thr, &OsmParser::gpsRefChanged, this, &Viewer3DQmlBackend::_gpsRefChangedEvent);
}

void Viewer3DQmlBackend::_gpsRefChangedEvent(QGeoCoordinate newGpsRef)
{   
    _gpsRef = newGpsRef;

    emit gpsRefChanged();

    qDebug() << "3D map gps reference:" << _gpsRef.latitude() << _gpsRef.longitude() << _gpsRef.altitude();
}

void Viewer3DQmlBackend::_heightBiasChangedEvent()
{
    bld_map_reader_thr->setBuildingMapHeightBias(_heightBias);
    metadata_loader_thr->heightBias = _heightBias;
    metadata_loader_thr->updateMetaDataFile();
}

void Viewer3DQmlBackend::_cityMapPathChangedEvent()
{
    metadata_loader_thr->osmFilePath = _osmFilePath;
    metadata_loader_thr->updateMetaDataFile();
}
