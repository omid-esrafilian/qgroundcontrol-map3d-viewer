/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QGroundControl
import QGroundControl.Controls
import QGroundControl.Palette
import QGroundControl.MultiVehicleManager
import QGroundControl.ScreenTools
import QGroundControl.Controllers

Rectangle {
    id:     _root
    width:  parent.width
    height: ScreenTools.toolbarHeight
    color:  qgcPal.toolbarBackground

    property var planMasterController

    property alias layerIndex: layerCombo.layerIndex

    property var    _activeVehicle:             QGroundControl.multiVehicleManager.activeVehicle
    property real   _margins:                   ScreenTools.defaultFontPixelWidth
    property var    _appSettings:                QGroundControl.settingsManager.appSettings

    function checkReadyForSave() {
        var unableToSave = qsTr("Unable to save")
        var readyForSaveState = planMasterController.readyForSaveState()
        if (readyForSaveState == VisualMissionItem.NotReadyForSaveData) {
            ApplicationWindow.window.showMessageDialog(unableToSave, qsTr("Plan has incomplete items. Complete all items and save again."))
            return false
        } else if (readyForSaveState == VisualMissionItem.NotReadyForSaveTerrain) {
            ApplicationWindow.window.showMessageDialog(unableToSave, qsTr("Plan is waiting on terrain data from server for correct altitude values."))
            return false
        }
        return true
    }

    RowLayout {
        anchors.top:        parent.top
        anchors.bottom:     parent.bottom
        anchors.leftMargin: _margins
        anchors.left:       parent.left
        spacing:            ScreenTools.defaultFontPixelWidth

        QGCButton {
            text:       qsTr("Save")

            onClicked: {
                if (!checkReadyForSave()) {
                    return
                }
                if (planMasterController.currentPlanFile === "") {
                    saveDialog.openForSave()
                    planMasterController.saveToSelectedFile()
                } else {
                    planMasterController.saveToCurrent()
                }
            }
        }
    }

    RowLayout {
        anchors.top:            parent.top
        anchors.bottom:         parent.bottom
        anchors.rightMargin:    _margins
        anchors.right:          parent.right
        spacing:                ScreenTools.defaultFontPixelWidth

        QGCComboBox {
            id:             layerCombo
            model:          [ qsTr("Mission"), qsTr("Fence"), qsTr("Rally") ]
            currentIndex:   0
            visible:        QGroundControl.corePlugin.options.enablePlanViewSelector

            property int layerIndex: Math.max(currentIndex, 0)
        }
    }

    QGCFileDialog {
        id:             saveDialog
        folder:         _appSettings.missionSavePath
        nameFilters:    planMasterController.saveNameFilters

        onAcceptedForSave: (file) => {
            planMasterController.saveToFile(file)
            close()
        }
    }
}
