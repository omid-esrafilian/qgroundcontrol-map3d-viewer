#ifndef QML_VARIABLE_TYPES_H
#define QML_VARIABLE_TYPES_H
#include <QObject>
#include <qqml.h>
#include "cpp_variable_types.h"
#include "util_functions.h"

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class Vec3f: public QObject
{
    Q_OBJECT
    Q_PROPERTY(float x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(float z READ z WRITE setZ NOTIFY zChanged)
    QML_ELEMENT

        public:

                 explicit Vec3f(QObject *parent = nullptr): QObject{parent}
    {
        setAll(0, 0, 0);
    }

    explicit Vec3f(float _x, float _y, float _z, QObject *parent = nullptr): QObject{parent}
    {
        setAll(_x, _y, _z);
    }

    float x() {return m_x;}
    float y() {return m_y;}
    float z() {return m_z;}

    void setAll(const float x, const float y, const float z)
    {
        m_x=x; m_y=y; m_z=z;
        emit xChanged(); emit yChanged(); emit zChanged();
    }
    void setX(const float x){m_x=x; emit xChanged();}
    void setY(const float y){m_y=y; emit yChanged();}
    void setZ(const float z){m_z=z; emit zChanged();}

    Vec3f& operator=(const Vec3f& b)
    {
        this->m_x = b.m_x;
        this->m_y = b.m_y;
        this->m_z = b.m_z;

        emit this->xChanged();
        emit this->yChanged();
        emit this->zChanged();

        return *this;
    }

    bool operator==(const Vec3f &other) const
    {
        return (m_x == other.m_x) && (m_y == other.m_y) && (m_z == other.m_z);
    }

    inline bool operator!=(const Vec3f &other) const {
        return !operator==(other);
    }

    Q_INVOKABLE QVector3D toQVec3D()
    {
        return QVector3D(m_x, m_y, m_z);
    }

signals:
    void xChanged();
    void yChanged();
    void zChanged();

private:
    float m_x, m_y, m_z;
};

// ref for defining custom property_type: https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html
class GpsType: public QObject
{
    Q_OBJECT
    Q_PROPERTY(float lat READ lat WRITE setLat NOTIFY latChanged)
    Q_PROPERTY(float lon READ lon WRITE setLon NOTIFY lonChanged)
    Q_PROPERTY(float alt READ alt WRITE setAlt NOTIFY altChanged)
    QML_ELEMENT

        public:
                 explicit GpsType(QObject *parent = nullptr, float lat_=0, float lon_=0, float alt_=0)
    {
        m_lat = lat_;
        m_lon = lon_;
        m_alt = alt_;
    }

    float lat() {return m_lat;}
    float lon() {return m_lon;}
    float alt() {return m_alt;}

    void setLat(const float lat){m_lat=lat; emit latChanged();}
    void setLon(const float lon){m_lon=lon; emit lonChanged();}
    void setAlt(const float alt){m_alt=alt; emit altChanged();}

    void operator=(const GpsType& b)
    {
        this->m_lat = b.m_lat;
        this->m_lon = b.m_lon;
        this->m_alt = b.m_alt;

        emit this->latChanged();
        emit this->lonChanged();
        emit this->altChanged();
    }

    bool operator==(const GpsType& b)
    {
        if(this->m_lat != b.m_lat)
            return 0;
        if(this->m_lon != b.m_lon)
            return 0;
        if(this->m_alt != b.m_alt)
            return 0;

        return 1;
    }

signals:
    void latChanged();
    void lonChanged();
    void altChanged();

private:
    float m_lat, m_lon, m_alt;
};


class GeoCoordinateType: public QObject
{
    Q_OBJECT
    Q_PROPERTY(GpsType* gps_ref READ gpsRead WRITE setGpsRef NOTIFY gpsRefChanged)
    Q_PROPERTY(GpsType* coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged)
    Q_PROPERTY(Vec3f* localCoordinate READ localCoordinate NOTIFY localCoordinateChanged)

    QML_ELEMENT

        public:
                 explicit GeoCoordinateType(QObject *parent = nullptr){
        m_gps_ref = new GpsType();
        m_coordinate = new GpsType();
        m_local_coordinate = new Vec3f();

        connect(m_gps_ref, &GpsType::latChanged, this, &GeoCoordinateType::gpsRefChangedEvent);
        connect(m_gps_ref, &GpsType::lonChanged, this, &GeoCoordinateType::gpsRefChangedEvent);
        connect(m_gps_ref, &GpsType::altChanged, this, &GeoCoordinateType::gpsRefChangedEvent);

        connect(m_coordinate, &GpsType::latChanged, this, &GeoCoordinateType::coordinateChangedEvent);
        connect(m_coordinate, &GpsType::lonChanged, this, &GeoCoordinateType::coordinateChangedEvent);
        connect(m_coordinate, &GpsType::altChanged, this, &GeoCoordinateType::coordinateChangedEvent);

    }

    void gps_to_local()
    {
        gps_point gps_tmp = {m_coordinate->lat(), m_coordinate->lon(), m_coordinate->alt()};
        gps_point gps_ref_tmp = {m_gps_ref->lat(), m_gps_ref->lon(), m_gps_ref->alt()};

        QVector3D local_pose_tmp = mapGpsToLocalPoint(gps_tmp, gps_ref_tmp);

        if(m_local_coordinate->x() != local_pose_tmp.x() ||
            m_local_coordinate->y() != local_pose_tmp.y() ||
            m_local_coordinate->z() != local_pose_tmp.z())
        {
            m_local_coordinate->setAll(local_pose_tmp.x(), local_pose_tmp.y(), m_coordinate->alt());
            emit localCoordinateChanged();
        }
    }



    GpsType *gpsRead() const{
        return m_gps_ref;
    }
    void setGpsRef(GpsType *newGps_ref){
        if (*m_gps_ref == *newGps_ref)
            return;
        m_gps_ref = newGps_ref;

        emit gpsRefChanged();
    }

    GpsType *coordinate() const{
        return m_coordinate;
    }
    void setCoordinate(GpsType *newCoordinate){
        if (*m_coordinate == *newCoordinate)
            return;
        m_coordinate = newCoordinate;

        gps_to_local();
        emit coordinateChanged();
    }

    Vec3f* localCoordinate(){return m_local_coordinate;}

signals:
    void gpsRefChanged();
    void coordinateChanged();
    void localCoordinateChanged();

protected slots:
    void gpsRefChangedEvent(){
        gps_to_local();

        emit gpsRefChanged();
    }

    void coordinateChangedEvent(){
        gps_to_local();

        emit coordinateChanged();
    }

private:
    GpsType *m_gps_ref = nullptr;
    GpsType *m_coordinate = nullptr;
    Vec3f* m_local_coordinate;
};


#endif // QML_VARIABLE_TYPES_H


