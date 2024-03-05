#ifndef VIEWER3DTERRAINTEXTURE_H
#define VIEWER3DTERRAINTEXTURE_H

#include <QObject>
#include <QQuick3DTextureData>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "Viewer3DTileQuery.h"
#include "OsmParser.h"
#include "FlightMapSettings.h"

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>


class Viewer3DTerrainTexture : public QQuick3DTextureData
{
    Q_PROPERTY(OsmParser* osmParser READ osmParser WRITE setOsmParser NOTIFY osmParserChanged)
    Q_PROPERTY(QGeoCoordinate roiMinCoordinate READ roiMinCoordinate WRITE setRoiMinCoordinate NOTIFY roiMinCoordinateChanged)
    Q_PROPERTY(QGeoCoordinate roiMaxCoordinate READ roiMaxCoordinate WRITE setRoiMaxCoordinate NOTIFY roiMaxCoordinateChanged)
    Q_PROPERTY(QSize tileCount READ tileCount NOTIFY tileCountChanged)
    Q_PROPERTY(bool textureLoaded READ textureLoaded NOTIFY textureLoadedChanged)
    Q_PROPERTY(bool textureGeometryDone READ textureGeometryDone NOTIFY textureGeometryDoneChanged)
    Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)

    Q_OBJECT
public:
    explicit Viewer3DTerrainTexture();

    Q_INVOKABLE void loadTexture();

    int _height, _width;

    QGeoCoordinate roiMinCoordinate() const;
    void setRoiMinCoordinate(const QGeoCoordinate &newRoiMinCoordinate);

    QGeoCoordinate roiMaxCoordinate() const;
    void setRoiMaxCoordinate(const QGeoCoordinate &newRoiMaxCoordinate);

    bool textureLoaded() const;

    int zoomLevel() const;
    void setZoomLevel(int newZoomLevel);

    OsmParser *osmParser() const;
    void setOsmParser(OsmParser *newOsmParser);

    QSize tileCount() const;
    void setTileCount(const QSize &newTileCount);

    bool textureGeometryDone() const;
    void setTextureGeometryDone(bool newTextureGeometryDone);

private:

    MapTileQuery _terrainTileLoader;
    FlightMapSettings* _flightMapSettings;
    QString _mapType;
    int _mapId;

    void updateEarthTexture();
    void setTextureLoaded(bool laoded){_textureLoaded = laoded; emit textureLoadedChanged();}
    void mapTypeChangedEvent(void);

    QGeoCoordinate _roiMinCoordinate;
    QGeoCoordinate _roiMaxCoordinate;

    bool _textureLoaded;

    int _zoomLevel;

    OsmParser *_osmParser = nullptr;

    QSize _tileCount;

    bool _textureGeometryDone;

signals:
    void roiMinCoordinateChanged();
    void roiMaxCoordinateChanged();
    void textureLoadedChanged();
    void zoomLevelChanged();
    void osmParserChanged();
    void tileCountChanged();
    void textureGeometryDoneChanged();
    void mapProviderIdChanged();
};

#endif // VIEWER3DTERRAINTEXTURE_H
