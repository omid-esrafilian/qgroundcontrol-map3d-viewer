#include "qml_backend.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QUrl>
#include "QGCViewer3D.h"
#include "QGCApplication.h"

QmlBackend::QmlBackend(QObject *parent)
    : QObject{parent}
{
    mav_link_loaded_flag = false;
    vechicle_set_flag = false;
    m_gps_ref = new GpsType();

    initMetadata();
    initOsmMapLoader();

    main_timer = new QTimer();
    main_timer->start(100);

    connect(main_timer, &QTimer::timeout, this, &QmlBackend::mainTimerEvent);
}

void QmlBackend::setGpsRef(GpsType *gps_ref)
{
    m_gps_ref->setLat(gps_ref->lat());
    m_gps_ref->setLon(gps_ref->lon());
    m_gps_ref->setAlt(gps_ref->alt());

    emit gpsRefChanged();
}

void QmlBackend::initMetadata()
{
    metadata_loader_thr = qgcViewer3D()->metaDataLoader();
    metadata_loader_thr->loadMetaDataFile();

    m_height_bias = metadata_loader_thr->meta_data.height_bias;
    m_city_map_path = metadata_loader_thr->meta_data.city_map_path;

    emit heightBiasChanged();
    emit cityMapPathChanged();

    connect(this, &QmlBackend::cityMapPathChanged, this, &QmlBackend::cityMapPathChangedEvent);
    connect(this, &QmlBackend::heightBiasChanged, this, &QmlBackend::heightBiasChangedEvent);
}

void QmlBackend::setActiveVehicle(Vehicle *_active_vehicle)
{
    active_vehicle = _active_vehicle;
    vechicle_set_flag = true;

    connect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &QmlBackend::activeVehicleChanged);
}

void QmlBackend::initOsmMapLoader()
{
    bld_map_reader_thr = qgcViewer3D()->mapLoader();
    connect(bld_map_reader_thr, &OsmParser::gpsRefChanged, this, &QmlBackend::gpsRefChangedEvent);
}

void QmlBackend::activeVehicleChanged(Vehicle *newActiveVehicle)
{
    active_vehicle = newActiveVehicle;
}

void QmlBackend::mainTimerEvent()
{
    emit mainTimerTimeout();
}

void QmlBackend::gpsRefChangedEvent(QGeoCoordinate new_gps_ref)
{

    m_gps_ref->setLat(new_gps_ref.latitude());
    m_gps_ref->setLon(new_gps_ref.longitude());
    m_gps_ref->setAlt(new_gps_ref.altitude());

    emit gpsRefChanged();

    qDebug() << "3D map gps reference:" << m_gps_ref->lat() << m_gps_ref->lon() << m_gps_ref->alt();
}

void QmlBackend::heightBiasChangedEvent()
{
    bld_map_reader_thr->setBuildingMapHeightBias(m_height_bias);
    metadata_loader_thr->meta_data.height_bias = m_height_bias;
    metadata_loader_thr->updateMetaDataFile();
}

void QmlBackend::cityMapPathChangedEvent()
{
    metadata_loader_thr->meta_data.city_map_path = m_city_map_path;
    metadata_loader_thr->updateMetaDataFile();
    qDebug("New 3D map file selected!!!!!!");
}
