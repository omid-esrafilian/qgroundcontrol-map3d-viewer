#include "Viewer3DTileReply.h"

#include "QGCMapEngine.h"


MapTileFetcher::MapTileFetcher(int zoomLevel, int tileX, int tileY, int mapId, QObject *parent)
    : QObject{parent}
{
    _timeoutCounter = 0;
    _timeoutTimer = new QTimer(this);
    _networkManager = new QNetworkAccessManager(this);
    _networkManager->setTransferTimeout(5000);
    // connect(_networkManager, &QNetworkAccessManager::finished, this, &MapTileFetcher::requestFinished);

    _tile.x = tileX;
    _tile.y = tileY;
    _tile.zoomLevel = zoomLevel;
    _tile.mapId = mapId;
    _tile.data.clear();
    _mapId = mapId;
    prepareDownload();

    _timeoutTimer->start(6000);
    connect(_timeoutTimer, &QTimer::timeout, this, &MapTileFetcher::timeoutTimerEvent);
}

MapTileFetcher::~MapTileFetcher()
{
    delete _networkManager;
    delete _timeoutTimer;
}

void MapTileFetcher::prepareDownload()
{
    QNetworkRequest request = getQGCMapEngine()->urlFactory()->getTileURL(_mapId, _tile.x, _tile.y, _tile.zoomLevel, _networkManager);
    _reply = _networkManager->get(request);
    connect(_reply, &QNetworkReply::finished, this, &MapTileFetcher::requestFinished);
    connect(_reply, &QNetworkReply::errorOccurred, this, &MapTileFetcher::requestError);
}

void MapTileFetcher::requestFinished()
{
    _tile.data = _reply->readAll();
    // disconnect(_networkManager, &QNetworkAccessManager::finished, this, &MapTileFetcher::requestFinished);
    _timeoutTimer->stop();
    disconnect(_reply, &QNetworkReply::finished, this, &MapTileFetcher::requestFinished);
    disconnect(_reply, &QNetworkReply::errorOccurred, this, &MapTileFetcher::requestError);
    disconnect(_timeoutTimer, &QTimer::timeout, this, &MapTileFetcher::timeoutTimerEvent);
    emit tileDone(_tile);
}

void MapTileFetcher::requestError()
{
    emit tileError(_tile);
    disconnect(_reply, &QNetworkReply::finished, this, &MapTileFetcher::requestFinished);
    disconnect(_reply, &QNetworkReply::errorOccurred, this, &MapTileFetcher::requestError);
}

void MapTileFetcher::timeoutTimerEvent()
{
    if(_timeoutCounter > 3){
        disconnect(_reply, &QNetworkReply::finished, this, &MapTileFetcher::requestFinished);
        disconnect(_reply, &QNetworkReply::errorOccurred, this, &MapTileFetcher::requestError);
        disconnect(_timeoutTimer, &QTimer::timeout, this, &MapTileFetcher::timeoutTimerEvent);
        emit tileGiveUp(_tile);
        _timeoutTimer->stop();
    }else if(_tile.data.isEmpty()){
        emit tileError(_tile);
        prepareDownload();
        _timeoutCounter++;
    }
}
