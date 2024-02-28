#ifndef MAPTILEFETCHER_H
#define MAPTILEFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class MapTileFetcher : public QObject
{
public:

    typedef struct tileInfo_s{
        int x, y, zoomLevel;
        QByteArray data;
    } tileInfo_t;

    Q_OBJECT
public:
    explicit MapTileFetcher(int zoomLevel, int tileX, int tileY, int mapId, QObject *parent = nullptr);
    ~MapTileFetcher();

private:

    QNetworkAccessManager* _networkManager;
    QNetworkReply* _reply;
    tileInfo_t _tile;

    void requestFinished();

signals:
    void tileDone(tileInfo_t);
};

#endif // MAPTILEFETCHER_H
