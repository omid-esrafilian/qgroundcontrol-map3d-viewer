#ifndef Viewer3DQmlBackend_H
#define Viewer3DQmlBackend_H

#include <QObject>
#include <qqml.h>
#include <QString>
#include "OsmParser.h"
#include "Viewer3DQmlVariableTypes.h"
#include "Viewer3DMetadata.h"
#include "Vehicle.h"


class Viewer3DMetadata;
class Vehicle;

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class Viewer3DQmlBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString city_map_path MEMBER m_city_map_path NOTIFY cityMapPathChanged)
    Q_PROPERTY(GpsType* gps_ref READ gpsRead NOTIFY gpsRefChanged)
    Q_PROPERTY(float height_bias MEMBER m_height_bias NOTIFY heightBiasChanged)

public:
    explicit Viewer3DQmlBackend(QObject *parent = nullptr);

    Viewer3DMetadata *metadata_loader_thr;
    OsmParser *bld_map_reader_thr;

    void init();

    GpsType* gpsRead(){return m_gps_ref;}
    void setGpsRef(GpsType* gps_ref);

    float getHeightBias(){return m_height_bias;}

    void initMetadata();
    void initOsmMapLoader();

signals:
    void userPoseChanged();
    void gpsRefChanged();
    void heightBiasChanged();
    void cityMapPathChanged();

private:
    QString m_city_map_path;
    GpsType* m_gps_ref;
    float m_height_bias;

protected slots:
    void gpsRefChangedEvent(QGeoCoordinate new_gps_ref);
    void heightBiasChangedEvent();
    void cityMapPathChangedEvent();
};

#endif // Viewer3DQmlBackend_H
