#ifndef MAPTILEQUERY_H
#define MAPTILEQUERY_H

#include "qvectornd.h"
#include <QPixmap>
#include <QObject>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPainter>
#include <QDebug>
#include <QGeoCoordinate>

#include "FlightMapSettings.h"

class MapTileQuery : public QObject
{

public:
    typedef struct MapTileContainer_s
    {
        int L = 256; // length of each square image downloaded tile

        std::vector<QPoint> tilesIndexArray;
        int zoomLevel;
        QPoint tileMinIndex;
        QPoint tileMaxIndex;
        int currentTileStat;

        QPoint currentTileIndex;
        QByteArray currentTileData;

        QImage mapTileMerged;
        int mapWidth, mapHeight;
        void init(){
            mapWidth = (tileMaxIndex.x() - tileMinIndex.x() + 1) * L;
            mapHeight = (tileMaxIndex.y() - tileMinIndex.y() + 1) * L;
            mapTileMerged = QImage(mapWidth, mapHeight, QImage::Format_RGB32);
        }

        void setMapTile(){
            QPixmap tmpImage;
            tmpImage.loadFromData(currentTileData);

            QPainter painter(&mapTileMerged);
            painter.drawImage((currentTileIndex.x() - tileMinIndex.x()) * L,
                              (currentTileIndex.y() - tileMinIndex.y()) * L,
                              tmpImage.toImage());
        }

        QByteArray getMapData(){
            QImage tmpImage = mapTileMerged.convertToFormat(QImage::Format_RGBA32FPx4);
            QByteArray tmpImageData =  QByteArray::fromRawData((const char*)tmpImage.bits(), tmpImage.sizeInBytes());
            tmpImageData.data();

            return tmpImageData;
        }

        void clear(){
            tilesIndexArray.clear();
        }
    }MapTileContainer_t;

    typedef struct TileStatistics_s{
        QGeoCoordinate coordinateMin;
        QGeoCoordinate coordinateMax;
        QSize tileCounts;
        int zoomLevel;
    } TileStatistics_t;


    Q_OBJECT
public:
    explicit MapTileQuery(QObject *parent = nullptr);

    void loadMapTiles(int zoomLevel, QPoint tileMinIndex, QPoint tileMaxIndex);
    TileStatistics_t findAndLoadMapTiles(int zoomLevel, QGeoCoordinate coordinate_1, QGeoCoordinate coordinate_2);
    TileStatistics_t adaptiveMapTilesLoader(QGeoCoordinate coordinate_1, QGeoCoordinate coordinate_2);
    int maxTileCount(int zoomLevel, QGeoCoordinate coordinateMin, QGeoCoordinate coordinateMax);
    QByteArray getMapData(){ return _mapToBeLoaded.getMapData();}
    QSize getMapSize(){ return QSize(_mapToBeLoaded.mapWidth, _mapToBeLoaded.mapHeight);}

private:
    QNetworkAccessManager* _networkManager;
    QNetworkReply* _reply;
    int _mapTileLoadStat;
    MapTileContainer_t _mapToBeLoaded;

    FlightMapSettings* _flightMapSettings;
    QString _mapType;

    double valueClip(double n, double _minValue, double _maxValue);
    QPoint latLonToPixelXY(QGeoCoordinate pointCoordinate, int zoomLevel);
    QPoint pixelXYToTileXY(QPoint pixel);
    QPoint tileXYToPixelXY(QPoint tile);
    QGeoCoordinate pixelXYToLatLong(QPoint pixel, int zoomLevel);
    void _loadNextMaptile();
    void httpServerReplyFinished();
    void httpReadyRead();
    void mapTypeChangedEvent(QVariant value);

signals:
    void loadingMapCompleted();
};

#endif // MAPTILEQUERY_H
