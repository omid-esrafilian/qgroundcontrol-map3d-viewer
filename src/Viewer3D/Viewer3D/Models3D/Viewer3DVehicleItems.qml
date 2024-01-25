import QtQuick
import QtQuick3D
import QtPositioning

import Viewer3D.Models3D.Drones
import Viewer3D.Models3D
import QGroundControl.Viewer3D

import QGroundControl
import QGroundControl.Controllers
import QGroundControl.Controls
import QGroundControl.FlightDisplay
import QGroundControl.FlightMap
import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.Vehicle

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Node {
    id: map3d_item_root
    property var  _backendQml:                  null
    property var  _vehicle:                     null
    property var  _planMasterController:        null
    property var  _missionController:           (_planMasterController)?(_planMasterController.missionController):(null)

    function addMissionItemsToListModel() {
        mission_waypoint_list_model.clear()
        var gps2Local_ = gps2Local
        gps2Local_.gpsRef = _backendQml.gpsRef

        for (var i = 1; i < _missionController.visualItems.count; i++) {
            var missionItem = _missionController.visualItems.get(i)
            gps2Local_.coordinate = missionItem.coordinate
            gps2Local_.coordinate.altitude = 0
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

        gps2Local_.gpsRef = _backendQml.gpsRef
        for (var i = 2; i < _missionController.visualItems.count; i++) {
            var missionItem = _missionController.visualItems.get(i-1)
            gps2Local_.coordinate = missionItem.coordinate
            gps2Local_.coordinate.altitude = 0
            var p1 = Qt.vector3d(gps2Local_.localCoordinate.x, gps2Local_.localCoordinate.y, missionItem.altitude.value)

            missionItem = _missionController.visualItems.get(i)
            gps2Local_.coordinate = missionItem.coordinate
            gps2Local_.coordinate.altitude = 0
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
        heightBias: _backendQml.heightBias
        gpsRef: _backendQml.gpsRef
    }

    Repeater3D{
        id:way3d_rep
        model: mission_waypoint_list_model

        delegate: Waypoint3DModel{
            opacity: 0.8
            missionItem: model
            heightBias: _backendQml.heightBias
        }
    }

    Repeater3D{
        id:mission_3d_path
        model: mission_path_model

        delegate: Line3D{
            p_1: Qt.vector3d(model.x_1 * 10, model.y_1 * 10, (model.z_1 + _backendQml.heightBias) * 10)
            p_2: Qt.vector3d(model.x_2 * 10, model.y_2 * 10, (model.z_2 + _backendQml.heightBias) * 10)
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
