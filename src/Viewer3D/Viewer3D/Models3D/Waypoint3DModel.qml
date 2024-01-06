import QtQuick3D
import QtQuick 2.9
import Viewer3DQmlType 1.0

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Node{
    id: body

    property var    missionItem
    property double gpsRefLat: 0
    property double gpsRefLon: 0
    property double gpsRefAlt: 0

    property double heightBias: 0
    property double pose_y: body.missionItem.y * 10
    property double pose_z: (body.missionItem.z + heightBias) * 10
    property double pose_x: body.missionItem.x * 10

    GeoCoordinateType{
        id: gps_to_local

        gps_ref{
            lat: body.gpsRefLat
            lon: body.gpsRefLon
            alt: body.gpsRefAlt
        }

        coordinate{

            lat: body.missionItem.x
            lon: body.missionItem.y
            alt: 0
        }
    }

    position{
        x: body.pose_x
        y: body.pose_y
        z: body.pose_z
    }

    Node{
        eulerRotation{
            x:90
        }
        scale: Qt.vector3d(0.1, 0.1, 0.1)
        Model {
            id: nose
            source: "#Cone"
            materials: [ DefaultMaterial {
                    diffuseColor: (body.missionItem.isTakeoffItem)?("green"):("black")

                }
            ]
        }
    }
}



