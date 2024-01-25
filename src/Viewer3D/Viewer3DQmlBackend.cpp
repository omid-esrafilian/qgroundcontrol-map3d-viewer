#include "Viewer3DQmlBackend.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QUrl>
#include "QGCViewer3D.h"
#include "QGCApplication.h"

Viewer3DQmlBackend::Viewer3DQmlBackend(QObject *parent)
    : QObject{parent}
{
    _gpsRef = new GpsType();
}

void Viewer3DQmlBackend::init()
{
    initMetadata();
    initOsmMapLoader();
}

void Viewer3DQmlBackend::setGpsRef(GpsType *gpsRef)
{
    _gpsRef->setLat(gpsRef->lat());
    _gpsRef->setLon(gpsRef->lon());
    _gpsRef->setAlt(gpsRef->alt());

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
    _gpsRef->setLat(newGpsRef.latitude());
    _gpsRef->setLon(newGpsRef.longitude());
    _gpsRef->setAlt(newGpsRef.altitude());

    emit gpsRefChanged();

    qDebug() << "3D map gps reference:" << _gpsRef->lat() << _gpsRef->lon() << _gpsRef->alt();
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
