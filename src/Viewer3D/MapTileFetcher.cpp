#include "MapTileFetcher.h"

#include "QGCMapEngine.h"


MapTileFetcher::MapTileFetcher(int zoomLevel, int tileX, int tileY, int mapId, QObject *parent)
    : QObject{parent}
{
    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, &QNetworkAccessManager::finished, this, &MapTileFetcher::requestFinished);


    _tile.x = tileX;
    _tile.y = tileY;
    _tile.zoomLevel = zoomLevel;
    _tile.data.clear();
    QNetworkRequest request = getQGCMapEngine()->urlFactory()->getTileURL(mapId, tileX, tileY, zoomLevel, _networkManager);
    _reply = _networkManager->get(request);
}

MapTileFetcher::~MapTileFetcher()
{
    delete _networkManager;
}

void MapTileFetcher::requestFinished()
{
    _tile.data = _reply->readAll();
    disconnect(_networkManager, &QNetworkAccessManager::finished, this, &MapTileFetcher::requestFinished);
    emit tileDone(_tile);
}
