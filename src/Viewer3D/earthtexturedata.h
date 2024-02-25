#ifndef EARTHTEXTUREDATA_H
#define EARTHTEXTUREDATA_H

#include <QObject>
#include <QQuick3DTextureData>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "maptileimageryloader.h"
#include "OsmParser.h"


///     @author Omid Esrafilian <esrafilian.omid@gmail.com>


class EarthTextureData : public QQuick3DTextureData
{
    Q_PROPERTY(OsmParser* osmParser READ osmParser WRITE setOsmParser NOTIFY osmParserChanged)
    Q_PROPERTY(QGeoCoordinate roiMinCoordinate READ roiMinCoordinate WRITE setRoiMinCoordinate NOTIFY roiMinCoordinateChanged)
    Q_PROPERTY(QGeoCoordinate roiMaxCoordinate READ roiMaxCoordinate WRITE setRoiMaxCoordinate NOTIFY roiMaxCoordinateChanged)
    Q_PROPERTY(bool textureLoaded READ textureLoaded NOTIFY textureLoadedChanged)
    Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)

    Q_OBJECT
public:
    explicit EarthTextureData();

    Q_INVOKABLE void loadTexture();

    int _height, _width;
    void updateTexture(QSize image_size, QByteArray image_data);

    QGeoCoordinate roiMinCoordinate() const;
    void setRoiMinCoordinate(const QGeoCoordinate &newRoiMinCoordinate);

    QGeoCoordinate roiMaxCoordinate() const;
    void setRoiMaxCoordinate(const QGeoCoordinate &newRoiMaxCoordinate);

    bool textureLoaded() const;

    int zoomLevel() const;
    void setZoomLevel(int newZoomLevel);

    OsmParser *osmParser() const;
    void setOsmParser(OsmParser *newOsmParser);

private:
    QNetworkAccessManager* _manager;
    QNetworkReply* _reply;
    bool _requestFinished;
    QByteArray _ImageHttpData;

    MapTileImageryLoader earthTileLoader;;

    void updateEarthTexture();
    void setTextureLoaded(bool laoded){_textureLoaded = laoded; emit textureLoadedChanged();}

    QGeoCoordinate _roiMinCoordinate;
    QGeoCoordinate _roiMaxCoordinate;

    bool _textureLoaded;

    int _zoomLevel;

    OsmParser *_osmParser = nullptr;

signals:
    void roiMinCoordinateChanged();
    void roiMaxCoordinateChanged();
    void textureLoadedChanged();
    void zoomLevelChanged();
    void osmParserChanged();
};

#endif // EARTHTEXTUREDATA_H
