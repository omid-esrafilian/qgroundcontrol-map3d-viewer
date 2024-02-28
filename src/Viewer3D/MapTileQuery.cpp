#include "MapTileQuery.h"

#include "QGCApplication.h"
#include "SettingsManager.h"
#include "QGCMapEngine.h"
#include "private/qgeotilespec_p.h"

#define PI                  acos(-1.0f)
#define DEG_TO_RAD          PI/180.0f
#define RAD_TO_DEG          180.0f/PI
#define MAX_TILE_COUNTS     100
#define MAX_ZOOM_LEVEL      20


enum RequestStat{
    STARTED,
    IN_PROGRESS,
    FINISHED,
    ERROR,
};

MapTileQuery::MapTileQuery(QObject *parent)
    : QObject{parent}
{
    _flightMapSettings = qgcApp()->toolbox()->settingsManager()->flightMapSettings();
    mapTypeChangedEvent(_flightMapSettings->mapProvider()->rawValue());
    connect(_flightMapSettings->mapProvider(), &Fact::rawValueChanged, this, &MapTileQuery::mapTypeChangedEvent);
    connect(_flightMapSettings->mapType(), &Fact::rawValueChanged, this, &MapTileQuery::mapTypeChangedEvent);

    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, &QNetworkAccessManager::finished, this, &MapTileQuery::httpServerReplyFinished);
}

void MapTileQuery::loadMapTiles(int zoomLevel, QPoint tileMinIndex, QPoint tileMaxIndex)
{
    _mapTileLoadStat = RequestStat::STARTED;
    _mapToBeLoaded.clear();
    _mapToBeLoaded.zoomLevel = zoomLevel;
    _mapToBeLoaded.tileMinIndex = tileMinIndex;
    _mapToBeLoaded.tileMaxIndex = tileMaxIndex;
    for (int x = tileMinIndex.x(); x <= tileMaxIndex.x(); x++) {
        for (int y = tileMinIndex.y(); y <= tileMaxIndex.y(); y++) {
            _mapToBeLoaded.tilesIndexArray.push_back(QPoint(x, y));
        }
    }
    qDebug() << _mapToBeLoaded.tilesIndexArray.size() << "Tiles to be downloaded!!";
    _mapToBeLoaded.init();
    _loadNextMaptile();
}

MapTileQuery::TileStatistics_t MapTileQuery::findAndLoadMapTiles(int zoomLevel, QGeoCoordinate coordinate_1, QGeoCoordinate coordinate_2)
{
    float lat_1 = coordinate_1.latitude(); float lon_1 = coordinate_1.longitude();
    float lat_2 = coordinate_2.latitude(); float lon_2 = coordinate_2.longitude();

    QGeoCoordinate minCoordinate = QGeoCoordinate(fmax(lat_1, lat_2), fmin(lon_1, lon_2), 0);
    QGeoCoordinate maxCoordinate = QGeoCoordinate(fmin(lat_1, lat_2), fmax(lon_1, lon_2), 0);

    QPoint minPixel = latLonToPixelXY(minCoordinate, zoomLevel);
    QPoint maxPixel = latLonToPixelXY(maxCoordinate, zoomLevel);

    QPoint minTile = pixelXYToTileXY(minPixel);
    QPoint maxTile = pixelXYToTileXY(maxPixel);

    minPixel = tileXYToPixelXY(minTile);
    maxPixel = tileXYToPixelXY(QPoint(maxTile.x() + 1, maxTile.y() + 1));

    minCoordinate = pixelXYToLatLong(minPixel, zoomLevel);
    maxCoordinate = pixelXYToLatLong(maxPixel, zoomLevel);

    // qDebug() << maxCoordinate.latitude() << "," << minCoordinate.longitude() << ";"<< minCoordinate.latitude()<< "," << maxCoordinate.longitude();
    QGeoCoordinate minCoordinate_ = QGeoCoordinate(maxCoordinate.latitude(), minCoordinate.longitude(), 0);
    QGeoCoordinate maxCoordinate_ = QGeoCoordinate(minCoordinate.latitude(), maxCoordinate.longitude(), 0);

    loadMapTiles(zoomLevel, minTile, maxTile);

    TileStatistics_t _output;
    _output.coordinateMin = minCoordinate_;
    _output.coordinateMax = maxCoordinate_;
    _output.tileCounts = QSize(maxTile.x() - minTile.x(), maxTile.y() - minTile.y());
    _output.zoomLevel = zoomLevel;

    return _output;
}

MapTileQuery::TileStatistics_t MapTileQuery::adaptiveMapTilesLoader(QGeoCoordinate coordinate_1, QGeoCoordinate coordinate_2)
{
    int zoomLevel;
    for(zoomLevel=MAX_ZOOM_LEVEL; zoomLevel>0; zoomLevel--){
        if(maxTileCount(zoomLevel, coordinate_1, coordinate_2) < MAX_TILE_COUNTS){
            break;
        }
    }
    return findAndLoadMapTiles(zoomLevel, coordinate_1, coordinate_2);
}

int MapTileQuery::maxTileCount(int zoomLevel, QGeoCoordinate coordinateMin, QGeoCoordinate coordinateMax)
{
    double mapSize = powf(2, zoomLevel);
    double latResolution = 180.0 / mapSize;
    double lonResolution = 360.0 / mapSize;

    double latLen = coordinateMax.latitude() - coordinateMin.latitude();
    double lonLen = coordinateMax.longitude() - coordinateMin.longitude();

    int tileXCount = ceil(lonLen/lonResolution);
    int tileYCount = ceil(latLen/latResolution);

    return tileXCount * tileYCount;
}

double MapTileQuery::valueClip(double n, double _minValue, double _maxValue)
{
    return fmin(fmax(n, _minValue), _maxValue);
}

QPoint MapTileQuery::latLonToPixelXY(QGeoCoordinate pointCoordinate, int zoomLevel)
{
    double MinLatitude = -85.05112878;
    double MaxLatitude = 85.05112878;
    double MinLongitude = -180.0f;
    double MaxLongitude = 180.0f;

    double latitude = valueClip(pointCoordinate.latitude(), MinLatitude, MaxLatitude);
    double longitude = valueClip(pointCoordinate.longitude(), MinLongitude, MaxLongitude);

    double x = (longitude + 180) / (360);
    double y = 0;
    if(fabs(latitude) < MaxLatitude){
        double sinLatitude = sin(latitude * DEG_TO_RAD);
        y = 0.5 - log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * PI);
    }else{
        y = (90 - latitude) / 180;
    }

    double mapSize = powf(2, zoomLevel) * 256.0f;
    int pixelX = (int) valueClip(x * mapSize + 0.5, 0, mapSize - 1);
    int pixelY = (int) valueClip(y * mapSize + 0.5, 0, mapSize - 1);

    return QPoint(pixelX, pixelY);
}

QPoint MapTileQuery::pixelXYToTileXY(QPoint pixel)
{
    return  QPoint(pixel.x() / 256, pixel.y() / 256);
}

QPoint MapTileQuery::tileXYToPixelXY(QPoint tile)
{
    return QPoint(tile.x() * 256, tile.y() * 256);
}

QGeoCoordinate MapTileQuery::pixelXYToLatLong(QPoint pixel, int zoomLevel)
{
    double mapSize = powf(2, zoomLevel) * 256.0f;
    double x = (valueClip(pixel.x(), 0, mapSize - 1) / mapSize) - 0.5;
    double y = 0;
    if(pixel.y() <mapSize - 1 && pixel.y() > 0){
        y = 0.5 - (valueClip(pixel.y(), 0, mapSize - 1) / mapSize);
    }else{
        y = (pixel.y() >= mapSize - 1)?(-1):(1);
    }

    double latitude = 90.0f - 360.0f * atan(exp(-y * 2 * PI)) / PI;
    double longitude = 360 * x;
    return QGeoCoordinate(latitude, longitude, 0);
}

void MapTileQuery::_loadNextMaptile()
{
    if(_mapToBeLoaded.tilesIndexArray.size() > 0){
        _mapTileLoadStat = RequestStat::IN_PROGRESS;
        _mapToBeLoaded.currentTileStat = RequestStat::STARTED;

        _mapToBeLoaded.currentTileIndex = _mapToBeLoaded.tilesIndexArray.back();
        _mapToBeLoaded.currentTileData.clear();

        QGeoTileSpec spec;
        spec.setX(_mapToBeLoaded.currentTileIndex.x());
        spec.setY(_mapToBeLoaded.currentTileIndex.y());
        spec.setZoom(_mapToBeLoaded.zoomLevel);
        spec.setMapId(getQGCMapEngine()->urlFactory()->getQtMapIdFromProviderType(_mapType));

        QNetworkRequest request = getQGCMapEngine()->urlFactory()->getTileURL(spec.mapId(), spec.x(), spec.y(), spec.zoom(), _networkManager);
        _reply = _networkManager->get(request);
        connect(_reply, &QIODevice::readyRead, this, &MapTileQuery::httpReadyRead);
    }else if(_mapTileLoadStat == RequestStat::IN_PROGRESS){
        _mapTileLoadStat = RequestStat::FINISHED;
        qDebug() << "All tiles downloaded ";
        emit loadingMapCompleted();
    }
}

void MapTileQuery::httpServerReplyFinished()
{
    if(_mapToBeLoaded.currentTileStat == RequestStat::IN_PROGRESS){
        _mapToBeLoaded.currentTileStat = RequestStat::FINISHED;

        _mapToBeLoaded.setMapTile();
        // qDebug() << "Tile index " << _mapToBeLoaded.currentTileIndex << " Downloaded";

        disconnect(_reply, &QIODevice::readyRead, this, &MapTileQuery::httpReadyRead);
        _mapToBeLoaded.tilesIndexArray.pop_back();
        _loadNextMaptile();
    }
}

void MapTileQuery::httpReadyRead()
{
    if(_mapToBeLoaded.currentTileStat == RequestStat::STARTED || _mapToBeLoaded.currentTileStat == RequestStat::IN_PROGRESS){
        _mapToBeLoaded.currentTileStat = RequestStat::IN_PROGRESS;
        _mapToBeLoaded.currentTileData += _reply->readAll();
    }
}

void MapTileQuery::mapTypeChangedEvent(QVariant value)
{
    _mapType.clear();
    _mapType = _flightMapSettings->mapProvider()->rawValue().toString() + QString(" ");
    _mapType += _flightMapSettings->mapType()->rawValue().toString();
}
