#ifndef VIEWER3DCPPVARIABLETYPES_H
#define VIEWER3DCPPVARIABLETYPES_H
#include "qgeocoordinate.h"
#include <QObject>
#include <QVector3D>
#include <QVector2D>

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

struct gps_point
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

struct bld_str
{
    std::vector<QGeoCoordinate> points_gps;
    std::vector<QVector2D> points_local;
    std::vector<QVector3D> triangulated_mesh;
    QVector2D bb_max, bb_min; //bounding boxes
    float height;
    int levels;
};

struct meta_data_type
{
    float height_bias;
    QString city_map_path;

};

struct drone_str{
    int32_t lat, lon, alt;
    int32_t lat_raw, lon_raw, alt_raw;
    float gps_vel_x, gps_vel_y, gps_vel_z;
    float relative_alt;
    float local_x, local_y, local_z;
    float body_vel_x, body_vel_y, body_vel_z;
    float p_x_setpoint, p_y_setpoint, p_z_setpoint;
    float p_x_setpoint_body, p_y_setpoint_body;
    float roll, pitch, yaw;
    float roll_setpoint, pitch_setpoint, yaw_setpoint, yaw_relative_setpoint;
    float baro_height, est_height;
    int16_t raw_mag_x, raw_mag_y, raw_mag_z;
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;
    int16_t raw_acc_x, raw_acc_y, raw_acc_z;
    int16_t scaled_mag_x, scaled_mag_y, scaled_mag_z;
    int16_t scaled_gyro_x, scaled_gyro_y, scaled_gyro_z;
    int16_t scaled_acc_x, scaled_acc_y, scaled_acc_z;
    float rc_scaled[18];

};

#endif // VIEWER3DCPPVARIABLETYPES_H
