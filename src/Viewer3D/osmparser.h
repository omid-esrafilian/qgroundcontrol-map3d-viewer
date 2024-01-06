#ifndef OSMPARSER_H
#define OSMPARSER_H

#include "cpp_variable_types.h"
#include "qgeocoordinate.h"
#include "qqml.h"
#include <QObject>
#include <QtXml>
#include <QFile>
#include <QMap>
#include <QVector3D>
#include <QVector2D>

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class OsmParser : public QObject
{
    Q_OBJECT
    QML_ELEMENT
        public:
                 explicit OsmParser(QObject *parent = nullptr);

    QGeoCoordinate gps_ref_point;
    QMap<uint64_t, QGeoCoordinate> map_nodes;
    QMap<uint64_t, bld_str> map_buildings;


    void setGpsRef(QGeoCoordinate gps_ref);
    QGeoCoordinate getGpsRef(){ return gps_ref_point;}
    void setBuildingLevelHeight(float level_height){building_level_height = level_height;}
    float getBuildingLevelHeight(void){return building_level_height;}
    void parseOsmFile(QString file_path);
    void decodeNodeTags(QDomElement& xml_component, QMap<uint64_t, QGeoCoordinate> &node_map);
    void decodeBuildings(QDomElement& xml_component, QMap<uint64_t, bld_str > &building_map, QMap<uint64_t, QGeoCoordinate> &node_map, QGeoCoordinate gps_ref);

    void setBuildingMapHeightBias(float height_bias){ map_height_bias = height_bias;}
    float getBuildingMapHeightBias(void){ return map_height_bias;}


    QByteArray buildingToMesh();

    void trianglateWallsExtrudedPolygon(std::vector<QVector3D>& triangulated_mesh, std::vector<QVector2D> vertices_ccw, float h, bool inverse_order=0, bool duplicate_st_end_point=0);
    void trianglateRectangle(std::vector<QVector3D>& triangulated_mesh, std::vector<QVector3D> vertices_ccw, bool invert_normal);

private:
    bool gps_ref_set;
    float building_level_height;
    bool map_loaded;
    float map_height_bias;


signals:
    void gpsRefChanged(QGeoCoordinate new_gps_ref);
    void newMapLoaded();

};

#endif // OSMPARSER_H
