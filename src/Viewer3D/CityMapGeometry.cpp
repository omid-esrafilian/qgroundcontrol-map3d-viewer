#include "CityMapGeometry.h"

CityMapGeometry::CityMapGeometry()
{
    _osmParser = nullptr;
    _modelName = "city_map_defualt_name";
    _vertexData.clear();
    _mapLoadedFlag = 0;

    _mainTimer = new QTimer(this);
    _mainTimer->start(500);
    connect(_mainTimer, &QTimer::timeout, this, &CityMapGeometry::_mainTimerEvent);

    updateData();
}

void CityMapGeometry::setModelName(QString modelName)
{
    _modelName = modelName;
    //    setName(_modelName);

    emit modelNameChanged();
}

void CityMapGeometry::setOsmFilePath(QString filePath)
{
    if(_osmFilePath.compare(filePath) == 0){
        return;
    }

    _mapLoadedFlag = 0;
    _osmFilePath = filePath;
    emit osmFilePathChanged();

    updateData();
}

//! [update data]
void CityMapGeometry::updateData()
{
    clear();

    if(!_osmParser){
        return;
    }

    _osmParser->parseOsmFile(_osmFilePath);
    _vertexData = _osmParser->buildingToMesh();

    int stride = 3 * sizeof(float);
    if(!_vertexData.isEmpty()){
        setVertexData(_vertexData);
        setStride(stride);

        setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

        addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                     0,
                     QQuick3DGeometry::Attribute::F32Type);

    }
    _mapLoadedFlag = 1;

    update();
}

void CityMapGeometry::_mainTimerEvent()
{
    if(!_mapLoadedFlag){
        updateData();
    }
}
