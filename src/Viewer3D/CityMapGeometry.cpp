#include "CityMapGeometry.h"

CityMapGeometry::CityMapGeometry()
{
    m_bld_map_reader = nullptr;
    m_model_name = "city_map_defualt_name";
    vertexData.clear();
    map_loaded_flag = 0;

    main_timer = new QTimer(this);
    main_timer->start(500);
    connect(main_timer, &QTimer::timeout, this, &CityMapGeometry::mainTimerEvent);

    updateData();
}

void CityMapGeometry::setModelName(QString m_name)
{
    m_model_name = m_name;
    //    setName(m_model_name);

    emit modelNameChanged();
}

void CityMapGeometry::setCityMap(QString map_path)
{
    if(m_city_map_path.compare(map_path) == 0)
        return;

    map_loaded_flag = 0;
    m_city_map_path = map_path;
    emit cityMapChanged();

    updateData();
}

//! [update data]
void CityMapGeometry::updateData()
{
    clear();

    if(!m_bld_map_reader)
        return;

    m_bld_map_reader->parseOsmFile(m_city_map_path);
    vertexData = m_bld_map_reader->buildingToMesh();


    int stride = 3 * sizeof(float);
    if(!vertexData.isEmpty())
    {
        setVertexData(vertexData);
        setStride(stride);

        setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

        addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                     0,
                     QQuick3DGeometry::Attribute::F32Type);

    }
    map_loaded_flag = 1;

    update();
}

void CityMapGeometry::mainTimerEvent()
{
    if(!map_loaded_flag)
    {
        updateData();
    }
}
