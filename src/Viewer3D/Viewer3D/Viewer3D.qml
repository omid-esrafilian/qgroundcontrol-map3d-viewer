import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.Controls

// 3D Viewer modules
import QGroundControl.Viewer3D
import Viewer3D.Models3D


Item{
    id: viewer3DBody
    property bool viewer3DOpen: false
    property bool settingMenuOpen: false

    Viewer3DFacts{
        id: _viewer3DManager
    }

    Loader{
        id: view3DLoader
        anchors.fill: parent

        onLoaded: {
            item.viewer3DManager = _viewer3DManager
        }
    }

    onViewer3DOpenChanged: {
        view3DLoader.source = "Models3D/Viewer3DModel.qml"
        if(viewer3DOpen){
            viewer3DBody.z = 1
        }else{
            viewer3DBody.z = 0
        }
    }

    onSettingMenuOpenChanged:{
        if(settingMenuOpen === true){
            popupWindow.open()
        }
    }

    Popup{
        id: popupWindow

        anchors{
            centerIn: Overlay.overlay
        }

        width: Screen.width * 0.25
        height: Screen.width * 0.3
        focus: true
        background: Rectangle{
            anchors.fill: parent
            color:        qgcPal.window
        }

        QGCPalette {
            id:                 qgcPal
            colorGroupEnabled:  enabled
        }

        Viewer3DSettingMenu{
            id:                     viewer3DSettingMenu
            viewer3DManager:        _viewer3DManager
            visible:                true
            opacity:                0.95

            anchors.fill: parent
        }

        Connections{
            target: viewer3DSettingMenu
            onCloseBtnClicked:{
                settingMenuOpen = false
                popupWindow.close()
            }
        }

        closePolicy: Popup.NoAutoClose
    }
}
