#include "osmparser.h"
#include "earcut.hpp"
#include "util_functions.h"

OsmParser::OsmParser(QObject *parent)
    : QObject{parent}
{
    gps_ref_set = false;
    building_level_height = 0; // meters
}

void OsmParser::setGpsRef(QGeoCoordinate gps_ref)
{
    gps_ref_point = gps_ref;
    gps_ref_set = true;
    emit gpsRefChanged(gps_ref_point);
}

void OsmParser::parseOsmFile(QString file_path)
{
    //The QDomDocument class represents an XML document.
    QDomDocument xml_content;
// Load xml file as raw data
#ifdef __unix__
    file_path = QString("/") + file_path;
#endif
    QFile f(file_path);
    if (!f.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        qDebug() << "Error while loading file";
        return;
    }
    // Set data into the QDomDocument before processing
    xml_content.setContent(&f);
    f.close();

    // Extract the root markup
    QDomElement root = xml_content.documentElement();

    QDomElement component = root.firstChild().toElement();

    map_nodes.clear();
    map_buildings.clear();
    gps_ref_set = false;
    map_loaded = false;

    while(!component.isNull())
    {
        decodeNodeTags(component, map_nodes);
        decodeBuildings(component, map_buildings, map_nodes, gps_ref_point);

        component = component.nextSibling().toElement();
    }
    map_loaded = true;
    emit newMapLoaded();
    qDebug() << map_buildings.size() << " Buildings added to the 3D viewer!!!";
}

void OsmParser::decodeNodeTags(QDomElement &xml_component, QMap<uint64_t, QGeoCoordinate> &node_map)
{
    int64_t id_tmp=0;
    QGeoCoordinate gps_tmp;
    QString attribute;
    if (xml_component.tagName()=="node")
    {

        attribute = xml_component.attribute("id","-1");
        id_tmp = attribute.toLongLong();

        attribute = xml_component.attribute("lat","0");
        gps_tmp.setLatitude(attribute.toDouble());

        attribute = xml_component.attribute("lon","0");
        gps_tmp.setLongitude(attribute.toDouble());

        gps_tmp.setAltitude(0);

        if(id_tmp > 0)
        {
            node_map.insert((uint64_t)id_tmp, gps_tmp);
            if(!gps_ref_set)
            {
                setGpsRef(gps_tmp);
            }
        }
    }
}

void OsmParser::decodeBuildings(QDomElement &xml_component, QMap<uint64_t, bld_str> &building_map, QMap<uint64_t, QGeoCoordinate> &node_map, QGeoCoordinate gps_ref)
{
    int64_t id_tmp = xml_component.attribute("id","0").toLongLong();

    bld_str bld_tmp;
    QGeoCoordinate gps_pt_tmp;
    QVector3D local_pt_tmp;
    std::vector<QGeoCoordinate> bld_points;
    std::vector<QVector2D> bld_points_local;
    double bld_x_max, bld_x_min, bld_y_max, bld_y_min;
    bld_x_max = bld_y_max = -1e10;
    bld_x_min = bld_y_min = 1e10;

    int64_t ref_id;
    QDomElement Child = xml_component.firstChild().toElement();
    QString attribute;

    if(id_tmp == 0)
        return;


    bld_tmp.height = 0;
    bld_tmp.levels = 0;

    while (!Child.isNull())
    {
        if (Child.tagName()=="nd")
        {
            ref_id = Child.attribute("ref","0").toLongLong();

            if(ref_id > 0)
            {
                gps_pt_tmp = node_map[ref_id];
                bld_points.push_back(gps_pt_tmp);
                local_pt_tmp = mapGpsToLocalPoint(gps_pt_tmp, gps_ref);
                bld_points_local.push_back(QVector2D(local_pt_tmp.x(), local_pt_tmp.y()));

                bld_x_max = (bld_x_max < local_pt_tmp.x())?(local_pt_tmp.x()):(bld_x_max);
                bld_y_max = (bld_y_max < local_pt_tmp.y())?(local_pt_tmp.y()):(bld_y_max);
                bld_x_min = (bld_x_min > local_pt_tmp.x())?(local_pt_tmp.x()):(bld_x_min);
                bld_y_min = (bld_y_min > local_pt_tmp.y())?(local_pt_tmp.y()):(bld_y_min);
            }
        }else if (Child.tagName()=="tag")
        {
            attribute = Child.attribute("k","0");
            if(attribute == "building:levels")
            {
                bld_tmp.levels = Child.attribute("v","0").toInt();
            }else if(attribute == "height")
            {
                bld_tmp.height = Child.attribute("v","0").toFloat();
            }
        }
        Child = Child.nextSibling().toElement();
    }

    if(bld_points.size() > 2 && (bld_tmp.height > 0 || bld_tmp.levels > 0))
    {
        float bld_height = (bld_tmp.height >= bld_tmp.levels * building_level_height)?(bld_tmp.height):(bld_tmp.levels * building_level_height);
        bld_tmp.height = bld_height;
        bld_tmp.points_gps = bld_points;
        bld_tmp.points_local = bld_points_local;
        bld_tmp.bb_max = QVector2D(bld_x_max, bld_y_max);
        bld_tmp.bb_min = QVector2D(bld_x_min, bld_y_min);
        building_map.insert(id_tmp, bld_tmp);
    }
}

QByteArray OsmParser::buildingToMesh()
{
    QByteArray vertexData;
    QMapIterator<uint64_t, bld_str> ii(map_buildings);

    for (auto ii = map_buildings.begin(), end = map_buildings.end(); ii != end; ++ii)
    {
        float bld_height = 0;
        std::vector<std::array<float, 2> > bld_points;
        std::vector<std::vector<std::array<float, 2> > > polygon;
        std::vector<QVector3D> triangulated_mesh;

        bld_height = (ii.value().height >= ii.value().levels * building_level_height)?(ii.value().height):(ii.value().levels * building_level_height);

        for(unsigned int jj=0; jj<ii.value().points_gps.size(); jj++)
        {
            bld_points.push_back({ii.value().points_local[jj].x(), ii.value().points_local[jj].y()});
        }

        polygon.push_back(bld_points);
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

        for(uint i_i=0; i_i<indices.size(); i_i+=3)
        {
            // mesh for roof
            uint n_idx = indices[i_i];
            triangulated_mesh.push_back(QVector3D(bld_points[n_idx][0], bld_points[n_idx][1], bld_height));
            n_idx = indices[i_i+1];
            triangulated_mesh.push_back(QVector3D(bld_points[n_idx][0], bld_points[n_idx][1], bld_height));
            n_idx = indices[i_i+2];
            triangulated_mesh.push_back(QVector3D(bld_points[n_idx][0], bld_points[n_idx][1], bld_height));

            // mesh for floor
            n_idx = indices[i_i+2];
            triangulated_mesh.push_back(QVector3D(bld_points[n_idx][0], bld_points[n_idx][1], 0));
            n_idx = indices[i_i+1];
            triangulated_mesh.push_back(QVector3D(bld_points[n_idx][0], bld_points[n_idx][1], 0));
            n_idx = indices[i_i];
            triangulated_mesh.push_back(QVector3D(bld_points[n_idx][0], bld_points[n_idx][1], 0));
        }

        if(bld_height > 0)
        {
            trianglateWallsExtrudedPolygon(triangulated_mesh, ii.value().points_local, bld_height, 0, 0); // mesh for wall outside
            trianglateWallsExtrudedPolygon(triangulated_mesh, ii.value().points_local, bld_height, 1, 0);// mesh for wall inside
        }

        QByteArray vertexData_tmp(triangulated_mesh.size() * 3 * sizeof(float), Qt::Initialization::Uninitialized);
        float *p = reinterpret_cast<float *>(vertexData_tmp.data());

        for(uint i_m=0; i_m<triangulated_mesh.size(); i_m++)
        {
            *p++ =  (float)triangulated_mesh[i_m].x(); *p++ =  (float)triangulated_mesh[i_m].y(); *p++ =  (float)triangulated_mesh[i_m].z();
        }

        ii.value().triangulated_mesh.insert(ii.value().triangulated_mesh.begin(), triangulated_mesh.begin(), triangulated_mesh.end());
        vertexData.append(vertexData_tmp);
    }
    return vertexData;
}

void OsmParser::trianglateWallsExtrudedPolygon(std::vector<QVector3D>& triangulated_mesh, std::vector<QVector2D> vertices_ccw, float h, bool inverse_order, bool duplicate_st_end_point)
{
    std::vector<QVector3D> tmp_rec_ccw(4);
    uint vertices_size = vertices_ccw.size() - (uint)(duplicate_st_end_point);

    if(inverse_order)
    {
        for(uint i_p=0; i_p<vertices_size; i_p++)
        {
            int i_p_p = (i_p < vertices_size-1)?(i_p+1):(0);
            tmp_rec_ccw[0] = QVector3D(vertices_ccw[i_p_p].x(), vertices_ccw[i_p_p].y(), 0);
            tmp_rec_ccw[1] = QVector3D(vertices_ccw[i_p].x(), vertices_ccw[i_p].y(), 0);
            tmp_rec_ccw[2] = QVector3D(vertices_ccw[i_p].x(), vertices_ccw[i_p].y(), h);
            tmp_rec_ccw[3] = QVector3D(vertices_ccw[i_p_p].x(), vertices_ccw[i_p_p].y(), h);
            trianglateRectangle(triangulated_mesh, tmp_rec_ccw, 0);
        }
        trianglateRectangle(triangulated_mesh, tmp_rec_ccw, 1);
    }
    else
    {
        for(uint i_p=0; i_p<vertices_size; i_p++)
        {
            int i_p_p = (i_p < vertices_size-1)?(i_p+1):(0);
            tmp_rec_ccw[0] = QVector3D(vertices_ccw[i_p].x(), vertices_ccw[i_p].y(), 0);
            tmp_rec_ccw[1] = QVector3D(vertices_ccw[i_p_p].x(), vertices_ccw[i_p_p].y(), 0);
            tmp_rec_ccw[2] = QVector3D(vertices_ccw[i_p_p].x(), vertices_ccw[i_p_p].y(), h);
            tmp_rec_ccw[3] = QVector3D(vertices_ccw[i_p].x(), vertices_ccw[i_p].y(), h);
            trianglateRectangle(triangulated_mesh, tmp_rec_ccw, 0);
        }
        trianglateRectangle(triangulated_mesh, tmp_rec_ccw, 1);
    }
}

void OsmParser::trianglateRectangle(std::vector<QVector3D>& triangulated_mesh, std::vector<QVector3D> vertices_ccw, bool invert_normal)
{
    std::vector<vec3i> mesh_set_idx;
    mesh_set_idx.resize(2);

    if(invert_normal)
    {
        mesh_set_idx[0] = {{3, 1, 0}};
        mesh_set_idx[1] = {{3, 2, 1}};
    }
    else
    {
        mesh_set_idx[0] = {{0, 1, 3}};
        mesh_set_idx[1] = {{1, 2, 3}};
    }

    uint idx_tmp;

    for(uint i_m=0; i_m<mesh_set_idx.size(); i_m++)
    {
        for(uint i_v=0; i_v<3; i_v++)
        {
            idx_tmp = mesh_set_idx[i_m].array[i_v];
            triangulated_mesh.push_back(QVector3D(vertices_ccw[idx_tmp].x(), vertices_ccw[idx_tmp].y(), vertices_ccw[idx_tmp].z()));
        }
    }
}