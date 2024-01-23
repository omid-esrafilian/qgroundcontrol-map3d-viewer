#include "Viewer3DQmlBackend.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QUrl>
#include "QGCViewer3D.h"
#include "QGCApplication.h"

Viewer3DQmlBackend::Viewer3DQmlBackend(QObject *parent)
    : QObject{parent}
{
    m_gps_ref = new GpsType();
}

void Viewer3DQmlBackend::init()
{
    initMetadata();
    initOsmMapLoader();
}

void Viewer3DQmlBackend::setGpsRef(GpsType *gps_ref)
{
    m_gps_ref->setLat(gps_ref->lat());
    m_gps_ref->setLon(gps_ref->lon());
    m_gps_ref->setAlt(gps_ref->alt());

    emit gpsRefChanged();
}

void Viewer3DQmlBackend::initMetadata()
{
    metadata_loader_thr = qgcApp()->viewer3D()->metaDataLoader();
    metadata_loader_thr->loadMetaDataFile();

    m_height_bias = metadata_loader_thr->meta_data.height_bias;
    m_city_map_path = metadata_loader_thr->meta_data.city_map_path;

    emit heightBiasChanged();
    emit cityMapPathChanged();

    connect(this, &Viewer3DQmlBackend::cityMapPathChanged, this, &Viewer3DQmlBackend::cityMapPathChangedEvent);
    connect(this, &Viewer3DQmlBackend::heightBiasChanged, this, &Viewer3DQmlBackend::heightBiasChangedEvent);
}

void Viewer3DQmlBackend::initOsmMapLoader()
{
    bld_map_reader_thr = qgcApp()->viewer3D()->osmParser();
    connect(bld_map_reader_thr, &OsmParser::gpsRefChanged, this, &Viewer3DQmlBackend::gpsRefChangedEvent);
}

void Viewer3DQmlBackend::gpsRefChangedEvent(QGeoCoordinate new_gps_ref)
{
    m_gps_ref->setLat(new_gps_ref.latitude());
    m_gps_ref->setLon(new_gps_ref.longitude());
    m_gps_ref->setAlt(new_gps_ref.altitude());

    emit gpsRefChanged();

    qDebug() << "3D map gps reference:" << m_gps_ref->lat() << m_gps_ref->lon() << m_gps_ref->alt();
}

void Viewer3DQmlBackend::heightBiasChangedEvent()
{
    bld_map_reader_thr->setBuildingMapHeightBias(m_height_bias);
    metadata_loader_thr->meta_data.height_bias = m_height_bias;
    metadata_loader_thr->updateMetaDataFile();
}

void Viewer3DQmlBackend::cityMapPathChangedEvent()
{
    metadata_loader_thr->meta_data.city_map_path = m_city_map_path;
    metadata_loader_thr->updateMetaDataFile();
}
