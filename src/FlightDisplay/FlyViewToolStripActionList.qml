/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQml.Models

import QGroundControl
import QGroundControl.Controls

ToolStripActionList {
    id: _root

    signal displayPreFlightChecklist

    model: [
        ToolStripAction {
            text:           qsTr("Plan")
            iconSource:     "/qmlimages/Plan.svg"
            onTriggered:
            {
                mainWindow.showPlanView()
                map_icon.showFlyMap()
            }
        },
        ToolStripAction {
            id: map_icon
            text:           qsTr("3D View")
            iconSource:     "/qmlimages/Viewer3D/City3DMapIcon.svg"
            onTriggered:{
                if(city_3d_view.z === 0)
                {
                    show3dMap();
                }
                else
                {
                    showFlyMap();
                }
            }

            function show3dMap()
            {
                city_3d_view.z = 1
                map_icon.iconSource =     "/qmlimages/PaperPlane.svg"
                text=           qsTr("Fly")
                city_map_setting_icon.enabled = true
            }

            function showFlyMap()
            {
                city_3d_view.z = 0
                iconSource =     "/qmlimages/Viewer3D/City3DMapIcon.svg"
                text =           qsTr("3D View")
                city_map_setting_icon.enabled = false
                app_setting_menu.state = "SETTING_MENU_CLOSE"
                city_map_setting_icon.checked = false
            }
        },
        ToolStripAction {
            id: city_map_setting_icon
            text:           qsTr("Setting")
            iconSource:     "/qmlimages/Viewer3D/GearIcon.png"
            enabled: false
            onTriggered:{
                app_setting_menu.state = (app_setting_menu.state === "SETTING_MENU_OPEN")?("SETTING_MENU_CLOSE"):("SETTING_MENU_OPEN")
                checked = (app_setting_menu.state === "SETTING_MENU_OPEN")?(true):(false)
            }
        },
        PreFlightCheckListShowAction { onTriggered: displayPreFlightChecklist() },
        GuidedActionTakeoff { },
        GuidedActionLand { },
        GuidedActionRTL { },
        GuidedActionPause { },
        GuidedActionActionList { },
        GuidedActionGripper { }
    ]
}
