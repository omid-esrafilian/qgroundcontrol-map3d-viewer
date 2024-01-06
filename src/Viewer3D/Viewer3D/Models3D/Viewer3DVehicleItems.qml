import QtQuick 2.9
import QtQuick3D
import Viewer3DQmlType 1.0

import Viewer3D.Models3D.Drones    1.0
import Viewer3D.Models3D           1.0

import QGroundControl               1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Node {
    id: map3d_item_root
    property var  _backendQml:      null
    property var  _vehicle:      null
    property var  _planMasterController:      null
    property var  _missionController: (_planMasterController)?(_planMasterController.missionController):(null)

    function addMissionItemsToListModel() {
        mission_waypoint_list_model.clear()
        var gps2Local_ = gps2Local
        gps2Local_.gps_ref = backendQml.gps_ref

        for (var i = 1; i < _missionController.visualItems.count; i++) {
            var missionItem = _missionController.visualItems.get(i)
            gps2Local_.coordinate.lat = missionItem.coordinate.latitude
            gps2Local_.coordinate.lon = missionItem.coordinate.longitude
            gps2Local_.coordinate.alt = 0
            mission_waypoint_list_model.append({
                                                   "x": gps2Local_.localCoordinate.x,
                                                   "y": gps2Local_.localCoordinate.y,
                                                   "z": missionItem.altitude.value,
                                                   "isTakeoffItem": missionItem.isTakeoffItem,
                                                   "index": missionItem.sequenceNumber,
                                               })
        }
    }

    function addSegmentToMissionPathModel() {
        mission_path_model.clear()
        var gps2Local_ = gps2Local

        gps2Local_.gps_ref = backendQml.gps_ref
        for (var i = 2; i < _missionController.visualItems.count; i++) {
            var missionItem = _missionController.visualItems.get(i-1)
            gps2Local_.coordinate.lat = missionItem.coordinate.latitude
            gps2Local_.coordinate.lon = missionItem.coordinate.longitude
            gps2Local_.coordinate.alt = 0
            var p1 = Qt.vector3d(gps2Local_.localCoordinate.x, gps2Local_.localCoordinate.y, missionItem.altitude.value)

            missionItem = _missionController.visualItems.get(i)
            gps2Local_.coordinate.lat = missionItem.coordinate.latitude
            gps2Local_.coordinate.lon = missionItem.coordinate.longitude
            gps2Local_.coordinate.alt = 0
            var p2 = Qt.vector3d(gps2Local_.localCoordinate.x, gps2Local_.localCoordinate.y, missionItem.altitude.value)

            mission_path_model.append({
                                          "x_1": p1.x,
                                          "y_1": p1.y,
                                          "z_1": p1.z,
                                          "x_2": p2.x,
                                          "y_2": p2.y,
                                          "z_2": p2.z,
                                      })
        }
    }

    GeoCoordinateType{
        id:gps2Local
    }

    ListModel{
        id: mission_waypoint_list_model
    }

    ListModel{
        id: mission_path_model
    }

    DroneModelDjiF450{
        id: vehicle_3d
        vehicle: _vehicle
        modelScale: Qt.vector3d(0.05, 0.05, 0.05)
        heightBias: _backendQml.height_bias
        gpsRefLat: _backendQml.gps_ref.lat
        gpsRefLon: _backendQml.gps_ref.lon
        gpsRefAlt: _backendQml.gps_ref.alt
    }

    Repeater3D{
        id:way3d_rep
        model: mission_waypoint_list_model

        delegate: Waypoint3DModel{
            opacity: 0.8
            missionItem: model
            heightBias: _backendQml.height_bias
            gpsRefLat: _backendQml.gps_ref.lat
            gpsRefLon: _backendQml.gps_ref.lon
            gpsRefAlt: _backendQml.gps_ref.alt
        }
    }

    Repeater3D{
        id:mission_3d_path
        model: mission_path_model

        delegate: Line3D{
            p_1: Qt.vector3d(model.x_1 * 10, model.y_1 * 10, (model.z_1 + _backendQml.height_bias) * 10)
            p_2: Qt.vector3d(model.x_2 * 10, model.y_2 * 10, (model.z_2 + _backendQml.height_bias) * 10)
            lineWidth:8
            color: "orange"
        }
    }

    Connections {
        target: _missionController
        onVisualItemsChanged: {
            addMissionItemsToListModel()
            addSegmentToMissionPathModel()
        }
    }

    Connections {
        target: _backendQml
        onGpsRefChanged: {
            addMissionItemsToListModel()
            addSegmentToMissionPathModel()
        }
    }
}
