#include "earthtexturedata.h"
#include <QImage>
#include <QUrl>
#include <unistd.h>


EarthTextureData::EarthTextureData()
{
    setTextureGeometryDone(false);
    setTextureLoaded(false);
}

void EarthTextureData::loadTexture()
{
    setTextureLoaded(false);
    MapTileQuery::TileStatistics_t tileInfo = earthTileLoader.adaptiveMapTilesLoader(_osmParser->getMapBoundingBoxCoordinate().first, _osmParser->getMapBoundingBoxCoordinate().second);
    setRoiMinCoordinate(tileInfo.coordinateMin);
    setRoiMaxCoordinate(tileInfo.coordinateMax);
    setTileCount(tileInfo.tileCounts);



    connect(&earthTileLoader, &MapTileQuery::loadingMapCompleted, this, &EarthTextureData::updateEarthTexture);
    // connect(&earthTileLoader, &MapTileQuery::mapTileDownloaded, this, &EarthTextureData::updateEarthTexture);
}

void EarthTextureData::updateEarthTexture()
{
    setSize(earthTileLoader.getMapSize());
    setFormat(QQuick3DTextureData::RGBA32F);
    setHasTransparency(false);

    setTextureData(earthTileLoader.getMapData());
    setTextureLoaded(true);
    setTextureGeometryDone(true);
}

QGeoCoordinate EarthTextureData::roiMinCoordinate() const
{
    return _roiMinCoordinate;
}

void EarthTextureData::setRoiMinCoordinate(const QGeoCoordinate &newRoiMinCoordinate)
{
    if (_roiMinCoordinate == newRoiMinCoordinate)
        return;
    _roiMinCoordinate = newRoiMinCoordinate;
    emit roiMinCoordinateChanged();
}

QGeoCoordinate EarthTextureData::roiMaxCoordinate() const
{
    return _roiMaxCoordinate;
}

void EarthTextureData::setRoiMaxCoordinate(const QGeoCoordinate &newRoiMaxCoordinate)
{
    if (_roiMaxCoordinate == newRoiMaxCoordinate)
        return;
    _roiMaxCoordinate = newRoiMaxCoordinate;
    emit roiMaxCoordinateChanged();
}

bool EarthTextureData::textureLoaded() const
{
    return _textureLoaded;
}

int EarthTextureData::zoomLevel() const
{
    return _zoomLevel;
}

void EarthTextureData::setZoomLevel(int newZoomLevel)
{
    if (_zoomLevel == newZoomLevel)
        return;
    _zoomLevel = newZoomLevel;
    emit zoomLevelChanged();
}

OsmParser *EarthTextureData::osmParser() const
{
    return _osmParser;
}

void EarthTextureData::setOsmParser(OsmParser *newOsmParser)
{
    if (_osmParser == newOsmParser){
        return;
    }
    _osmParser = newOsmParser;

    if(_osmParser){
        connect(_osmParser, &OsmParser::mapChanged, this, &EarthTextureData::loadTexture);
    }

    emit osmParserChanged();
}

QSize EarthTextureData::tileCount() const
{
    return _tileCount;
}

void EarthTextureData::setTileCount(const QSize &newTileCount)
{
    if (_tileCount == newTileCount)
        return;
    _tileCount = newTileCount;
    emit tileCountChanged();
}

bool EarthTextureData::textureGeometryDone() const
{
    return _textureGeometryDone;
}

void EarthTextureData::setTextureGeometryDone(bool newTextureGeometryDone)
{
    if (_textureGeometryDone == newTextureGeometryDone)
        return;
    _textureGeometryDone = newTextureGeometryDone;
    emit textureGeometryDoneChanged();
}
