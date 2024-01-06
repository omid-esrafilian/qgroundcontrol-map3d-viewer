import QtQuick3D
import QtQuick
import QtQuick.Controls 2.0
import Viewer3DQmlType 1.0
import QtQuick.Window   2.2

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

View3D {
    id: topRightView

    property var  backendQml:      null
    property var  missionController
    readonly property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle


    camera: standAloneScene.camera_main
    importScene: CameraLightModel{
        id: standAloneScene
    }

//    renderMode: View3D.Inline

    environment: SceneEnvironment {
        clearColor: "white"
        backgroundMode: SceneEnvironment.Color
    }

    Model {
        id: city_map_model
        visible: true
        scale: Qt.vector3d(10, 10, 10)
        geometry: CityMapGeometry {
            id: city_map_geometry
            model_name: "city_map"
            city_map: backendQml.city_map_path
            bld_map_reader: Viewer3DOsmReader
        }

        materials: [ DefaultMaterial {
                diffuseColor: "gray"
            }
        ]

    }

    Repeater3D{
        model: QGroundControl.multiVehicleManager.vehicles

        delegate: Viewer3DVehicleItems{
            _vehicle: object
            _backendQml: topRightView.backendQml
            _planMasterController: masterController

            PlanMasterController {
                id: masterController
                Component.onCompleted: startStaticActiveVehicle(object)
            }
        }
    }

    MouseArea{
        property real transferSpeed: 1
        property real rotationSpeed: 0.1
        property real zoomSpeed: 0.3

        property point lastPos;
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons;


        onPressed: (mouse)=> {
            lastPos = Qt.point(mouse.x, mouse.y);
        }

        onPositionChanged: (mouse)=> {
            let roll = standAloneScene.cam_node_in_rotation.x * (3.1415/180)
            let pitch = standAloneScene.cam_node_in_rotation.y * (3.1415/180)
            let yaw = standAloneScene.cam_node_in_rotation.z * (3.1415/180)

            if (mouse.buttons === Qt.LeftButton) { // Left button for translate
                let dx_l = (mouse.x - lastPos.x) * transferSpeed
                let dy_l = (mouse.y - lastPos.y) * transferSpeed

                //Note: Rotation Matrix is computed as: R = R(-pitch) * R(roll)
                // Then the corerxt tramslation is: d = R * [dx_l; dy_l; dz_l]

                let dx = dx_l * Math.cos(pitch) - dy_l * Math.sin(pitch) * Math.sin(roll)
                let dy =  dy_l * Math.cos(roll)
                let dz = dx_l * Math.sin(pitch) + dy_l * Math.cos(pitch) * Math.sin(roll)

                standAloneScene.cam_node_middle_position.x -= dx
                standAloneScene.cam_node_middle_position.y += dy
                standAloneScene.cam_node_middle_position.z += dz
            }else if (mouse.buttons === Qt.RightButton){ // Right button for rotation

                let rotation_vec = Qt.vector2d(mouse.y - lastPos.y, mouse.x - lastPos.x);

                let dx_l = rotation_vec.x * rotationSpeed
                let dy_l = rotation_vec.y * rotationSpeed

                standAloneScene.cam_node_in_rotation.x += dx_l
                standAloneScene.cam_node_in_rotation.y += dy_l

            }
            lastPos = Qt.point(mouse.x, mouse.y)

        }

        onWheel: (wheel)=> {
            let dz_l = -wheel.angleDelta.y * zoomSpeed

            let roll = standAloneScene.cam_node_in_rotation.x * (3.1415/180)
            let pitch = standAloneScene.cam_node_in_rotation.y * (3.1415/180)

            let dx = -dz_l * Math.cos(roll) * Math.sin(pitch)
            let dy =  -dz_l * Math.sin(roll)
            let dz = dz_l * Math.cos(pitch) * Math.cos(roll)


            standAloneScene.cam_node_middle_position.x -= dx
            standAloneScene.cam_node_middle_position.y += dy
            standAloneScene.cam_node_middle_position.z += dz

        }
    }
}
