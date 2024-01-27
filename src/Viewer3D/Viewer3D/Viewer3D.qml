import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.Controls

// 3D Viewer modules
import QGroundControl.Viewer3D
import Viewer3D
import Viewer3D.Models3D


Item{
    id: viewer3DBody

    Viewer3DFacts{
        id: _viewer3DManager
    }

    Viewer3DModel{
        id:                     viewer3DWindow
        anchors.fill:           parent
        viewer3DManager:        _viewer3DManager
        //        z:                      viewer3DVisible
    }

    Viewer3DSettingMenu{
        id:                     viewer3DSettingMenu
        viewer3DManager:        _viewer3DManager
        //        z:            QGroundControl.zOrderWidgets
        visible:                false
        opacity:                0.95

        anchors{
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }
}
