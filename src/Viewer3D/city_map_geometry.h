
#ifndef CITY_MAP_GEOMETRY_H
#define CITY_MAP_GEOMETRY_H

#include <QQuick3DGeometry>
#include "osmparser.h"
#include <QTimer>

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class CityMapGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CityMapGeometry)
    Q_PROPERTY(QString model_name READ modelName WRITE setModelName NOTIFY modelNameChanged)
    Q_PROPERTY(QString city_map READ cityMapPath WRITE setCityMap NOTIFY cityMapChanged)
    Q_PROPERTY(OsmParser* bld_map_reader MEMBER m_bld_map_reader NOTIFY bldMapReaderChanged)

public:

    CityMapGeometry();

    QString modelName() const { return m_model_name; }
    void setModelName(QString m_name);

    QString cityMapPath() const {return m_city_map_path;}
    void setCityMap(QString map_path);

signals:
    void modelNameChanged();
    void cityMapChanged();
    void gpsRefChanged();
    void bldMapReaderChanged();

private:
    void updateData();

    QString m_model_name;
    QString m_city_map_path;
    QByteArray vertexData;
    OsmParser *m_bld_map_reader;
    bool map_loaded_flag;
    QTimer *main_timer;

protected slots:
    void mainTimerEvent();
};

#endif // CITY_MAP_GEOMETRY_H
