#ifndef VIEWER3DCPPVARIABLETYPES_H
#define VIEWER3DCPPVARIABLETYPES_H
#include "qgeocoordinate.h"
#include <QObject>
#include <QVector3D>
#include <QVector2D>

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

struct GpsPoint
{
    double lat, lon, alt;
};

typedef union {
    uint array[3];

    struct {
        uint x;
        uint y;
        uint z;
    } axis;
} vec3i;

struct BuildingType
{
    std::vector<QGeoCoordinate> points_gps;
    std::vector<QVector2D> points_local;
    std::vector<QVector3D> triangulated_mesh;
    QVector2D bb_max, bb_min; //bounding boxes
    float height;
    int levels;
};

struct Viewer3DMetadataType
{
    float height_bias;
    QString city_map_path;

};

#endif // VIEWER3DCPPVARIABLETYPES_H
