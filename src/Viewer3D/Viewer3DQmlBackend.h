#ifndef Viewer3DQmlBackend_H
#define Viewer3DQmlBackend_H

#include <QObject>
#include <qqml.h>
#include <QString>

#include "OsmParser.h"
#include "Viewer3DQmlVariableTypes.h"
#include "Viewer3DMetadata.h"


class Viewer3DMetadata;

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

class Viewer3DQmlBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString osmFilePath MEMBER _osmFilePath NOTIFY cityMapPathChanged)
    Q_PROPERTY(QGeoCoordinate gpsRef READ gpsRef NOTIFY gpsRefChanged)
    Q_PROPERTY(float heightBias MEMBER _heightBias NOTIFY heightBiasChanged)

public:
    explicit Viewer3DQmlBackend(QObject *parent = nullptr);

    Viewer3DMetadata *metadata_loader_thr;
    OsmParser *bld_map_reader_thr;

    void init();

    QGeoCoordinate gpsRef(){return _gpsRef;}
    void setGpsRef(const QGeoCoordinate& gpsRef);

    void initMetadata();
    void initOsmMapLoader();

signals:
    void userPoseChanged();
    void gpsRefChanged();
    void heightBiasChanged();
    void cityMapPathChanged();

private:
    QString _osmFilePath;
    QGeoCoordinate _gpsRef;
    float _heightBias;

protected slots:
    void _gpsRefChangedEvent(QGeoCoordinate newGpsRef);
    void _heightBiasChangedEvent();
    void _cityMapPathChangedEvent();
};

#endif // Viewer3DQmlBackend_H
