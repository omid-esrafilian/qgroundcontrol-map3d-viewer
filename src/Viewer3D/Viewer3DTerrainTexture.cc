#include "Viewer3DTerrainTexture.h"
#include <QImage>
#include <QUrl>
#include <unistd.h>

#include "QGCApplication.h"
#include "SettingsManager.h"
#include "QGCMapEngine.h"


Viewer3DTerrainTexture::Viewer3DTerrainTexture()
{
    _flightMapSettings = qgcApp()->toolbox()->settingsManager()->flightMapSettings();
    mapTypeChangedEvent();

    setTextureGeometryDone(false);
    setTextureLoaded(false);

    // connect(_flightMapSettings->mapProvider(), &Fact::rawValueChanged, this, &Viewer3DTerrainTexture::mapTypeChangedEvent);
    connect(_flightMapSettings->mapType(), &Fact::rawValueChanged, this, &Viewer3DTerrainTexture::mapTypeChangedEvent);
    connect(&_terrainTileLoader, &MapTileQuery::loadingMapCompleted, this, &Viewer3DTerrainTexture::updateEarthTexture);
    connect(this, &Viewer3DTerrainTexture::mapProviderIdChanged, this, &Viewer3DTerrainTexture::loadTexture);
}

void Viewer3DTerrainTexture::loadTexture()
{
    setTextureLoaded(false);
    if(_osmParser->mapLoaded()){
        MapTileQuery::TileStatistics_t tileInfo = _terrainTileLoader.adaptiveMapTilesLoader(_mapType, _mapId,
                                                                                            _osmParser->getMapBoundingBoxCoordinate().first,
                                                                                            _osmParser->getMapBoundingBoxCoordinate().second);
        setRoiMinCoordinate(tileInfo.coordinateMin);
        setRoiMaxCoordinate(tileInfo.coordinateMax);
        setTileCount(tileInfo.tileCounts);
    }
}

void Viewer3DTerrainTexture::updateEarthTexture()
{
    setSize(_terrainTileLoader.getMapSize());
    setFormat(QQuick3DTextureData::RGBA32F);
    setHasTransparency(false);

    setTextureData(_terrainTileLoader.getMapData());
    setTextureLoaded(true);
    setTextureGeometryDone(true);
}

void Viewer3DTerrainTexture::mapTypeChangedEvent(void)
{
    _mapType.clear();
    _mapType = _flightMapSettings->mapProvider()->rawValue().toString() + QString(" ");
    _mapType += _flightMapSettings->mapType()->rawValue().toString();

    int mapId = getQGCMapEngine()->urlFactory()->getQtMapIdFromProviderType(_mapType);

    if(mapId == _mapId){
        return;
    }

    _mapId = mapId;
    emit mapProviderIdChanged();
}

QGeoCoordinate Viewer3DTerrainTexture::roiMinCoordinate() const
{
    return _roiMinCoordinate;
}

void Viewer3DTerrainTexture::setRoiMinCoordinate(const QGeoCoordinate &newRoiMinCoordinate)
{
    if (_roiMinCoordinate == newRoiMinCoordinate){
        return;
    }
    _roiMinCoordinate = newRoiMinCoordinate;
    emit roiMinCoordinateChanged();
}

QGeoCoordinate Viewer3DTerrainTexture::roiMaxCoordinate() const
{
    return _roiMaxCoordinate;
}

void Viewer3DTerrainTexture::setRoiMaxCoordinate(const QGeoCoordinate &newRoiMaxCoordinate)
{
    if (_roiMaxCoordinate == newRoiMaxCoordinate){
        return;
    }
    _roiMaxCoordinate = newRoiMaxCoordinate;
    emit roiMaxCoordinateChanged();
}

bool Viewer3DTerrainTexture::textureLoaded() const
{
    return _textureLoaded;
}

int Viewer3DTerrainTexture::zoomLevel() const
{
    return _zoomLevel;
}

void Viewer3DTerrainTexture::setZoomLevel(int newZoomLevel)
{
    if (_zoomLevel == newZoomLevel){
        return;
    }
    _zoomLevel = newZoomLevel;
    emit zoomLevelChanged();
}

OsmParser *Viewer3DTerrainTexture::osmParser() const
{
    return _osmParser;
}

void Viewer3DTerrainTexture::setOsmParser(OsmParser *newOsmParser)
{
    if (_osmParser == newOsmParser){
        return;
    }
    _osmParser = newOsmParser;

    if(_osmParser){
        connect(_osmParser, &OsmParser::mapChanged, this, &Viewer3DTerrainTexture::loadTexture);
    }

    emit osmParserChanged();
}

QSize Viewer3DTerrainTexture::tileCount() const
{
    return _tileCount;
}

void Viewer3DTerrainTexture::setTileCount(const QSize &newTileCount)
{
    if (_tileCount == newTileCount){
        return;
    }
    _tileCount = newTileCount;
    emit tileCountChanged();
}

bool Viewer3DTerrainTexture::textureGeometryDone() const
{
    return _textureGeometryDone;
}

void Viewer3DTerrainTexture::setTextureGeometryDone(bool newTextureGeometryDone)
{
    if (_textureGeometryDone == newTextureGeometryDone){
        return;
    }
    _textureGeometryDone = newTextureGeometryDone;
    emit textureGeometryDoneChanged();
}
