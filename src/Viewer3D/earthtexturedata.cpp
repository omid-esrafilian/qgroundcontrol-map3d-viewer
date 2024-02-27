#include "earthtexturedata.h"
#include <QImage>
#include <QUrl>



EarthTextureData::EarthTextureData()
{
}

void EarthTextureData::loadTexture()
{
    _textureLoaded = 0;
    std::pair<QGeoCoordinate, QGeoCoordinate> roiPair = earthTileLoader.findAndLoadMapTiles(_zoomLevel, _osmParser->getMapBoundingBoxCoordinate().first, _osmParser->getMapBoundingBoxCoordinate().second);
    setRoiMinCoordinate(roiPair.first);
    setRoiMaxCoordinate(roiPair.second);
    setRoiMaxCoordinate(roiPair.second);
    // connect(&earthTileLoader, &MapTileImageryLoader::loadingMapCompleted, this, &EarthTextureData::updateEarthTexture);
    connect(&earthTileLoader, &MapTileQuery::loadingMapCompleted, this, &EarthTextureData::updateEarthTexture);
}

void EarthTextureData::updateTexture(QSize image_size, QByteArray image_data)
{
    setSize(image_size);
    setFormat(QQuick3DTextureData::RGBA32F);
    setHasTransparency(false);
    setTextureData(image_data);
}

void EarthTextureData::updateEarthTexture()
{
    setTextureLoaded(true);
    updateTexture(earthTileLoader.getMapSize(), earthTileLoader.getMapData());
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
