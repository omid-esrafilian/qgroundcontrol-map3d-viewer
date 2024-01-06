#ifndef QmlBackend_H
#define QmlBackend_H

#include <QObject>
#include <qqml.h>
#include <QString>
#include <QTimer>
#include "osmparser.h"
#include "qml_variable_types.h"
#include "metadatastreamer.h"
#include "Vehicle.h"


class MetaDataStreamer;
class Vehicle;

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class QmlBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString city_map_path MEMBER m_city_map_path NOTIFY cityMapPathChanged)
    Q_PROPERTY(GpsType* gps_ref READ gpsRead NOTIFY gpsRefChanged)
    Q_PROPERTY(float height_bias MEMBER m_height_bias NOTIFY heightBiasChanged)


    QML_ELEMENT

public:
    explicit QmlBackend(QObject *parent = nullptr);

    MetaDataStreamer *metadata_loader_thr;
    OsmParser *bld_map_reader_thr;

    GpsType* gpsRead(){return m_gps_ref;}
    void setGpsRef(GpsType* gps_ref);

    float getHeightBias(){return m_height_bias;}

    void initMetadata();
    void setActiveVehicle(Vehicle* _active_vehicle);
    void initOsmMapLoader();

signals:
    void userPoseChanged();
    void mainTimerTimeout();
    void gpsRefChanged();
    void heightBiasChanged();
    void cityMapPathChanged();

private:
    QObject *object;
    QTimer *main_timer;
    Vehicle * active_vehicle;

    bool mav_link_loaded_flag;
    bool vechicle_set_flag;

    QString m_city_map_path;
    GpsType* m_gps_ref;
    float m_height_bias;

protected slots:
    void activeVehicleChanged (Vehicle* newActiveVehicle);
    void mainTimerEvent();
    void gpsRefChangedEvent(QGeoCoordinate new_gps_ref);
    void heightBiasChangedEvent();
    void cityMapPathChangedEvent();
};

#endif // QmlBackend_H
