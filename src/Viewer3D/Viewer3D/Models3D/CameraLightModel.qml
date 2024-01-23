import QtQuick3D
import QtQuick
import QtQuick.Controls

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Node {
    property alias cam_node_in_rotation: cameraPerspectiveOne.eulerRotation
    property alias cam_node_middle_position: cameraPerspectiveTwo.position
    property alias camera_main: cameraPerspectiveOne

    property real tilt: 0.001
    property real pan: 0.001
    property real r_zoom: 1000


    DirectionalLight {
        eulerRotation.x: 180
    }

    DirectionalLight {
        eulerRotation.x: 0
    }

    DirectionalLight {
        eulerRotation.x: 90
    }

    DirectionalLight {
        eulerRotation.x: 270
    }

    DirectionalLight {
        eulerRotation.y: 90
    }

    DirectionalLight {
        eulerRotation.y: -90
    }

    Node {
        id: camNode
        eulerRotation{
            x:90
        }
        Node {
            id: cameraPerspectiveThree
            Node {
                id: cameraPerspectiveTwo

                position{
                    x: r_zoom * Math.sin(tilt) * Math.cos(pan)
                    z: r_zoom * Math.sin(tilt) * Math.sin(pan)
                    y: r_zoom * Math.cos(tilt)
                }

                PerspectiveCamera {

                    id: cameraPerspectiveOne

                }
            }
        }
    }
}
