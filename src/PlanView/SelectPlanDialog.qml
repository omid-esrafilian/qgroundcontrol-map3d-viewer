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
import QGroundControl.Controllers
import QGroundControl.FactSystem
import QGroundControl.FactControls
import QGroundControl.ScreenTools

QGCPopupDialog {
    id:         root
    title:      qsTr("Select Plan")
    buttons:    Dialog.Cancel

    property var planMasterController

    property real _margin:              ScreenTools.defaultFontPixelWidth
    property real _sectionLeftMargin:   ScreenTools.defaultFontPixelWidth * 2
    property var  _appSettings:         QGroundControl.settingsManager.appSettings

    onRejected: ApplicationWindow.window.popView()

    ColumnLayout {
        spacing: ScreenTools.defaultFontPixelHeight / 2

        QGCLabel {
            text: qsTr("Create Plan")
        }

        GridLayout {
            Layout.leftMargin:  _sectionLeftMargin
            columns:            2
            columnSpacing:      _margin
            rowSpacing:         _margin
            Layout.fillWidth:   true

            Repeater {
                model: planMasterController.planCreators

                Rectangle {
                    id:     button
                    width:  ScreenTools.defaultFontPixelHeight * 7
                    height: planCreatorNameLabel.y + planCreatorNameLabel.height
                    color:  button.pressed || button.highlighted ? qgcPal.buttonHighlight : qgcPal.button

                    property bool highlighted: mouseArea.containsMouse
                    property bool pressed:     mouseArea.pressed

                    Image {
                        id:                 planCreatorImage
                        anchors.left:       parent.left
                        anchors.right:      parent.right
                        source:             object.imageResource
                        sourceSize.width:   width
                        fillMode:           Image.PreserveAspectFit
                        mipmap:             true
                    }

                    QGCLabel {
                        id:                     planCreatorNameLabel
                        anchors.top:            planCreatorImage.bottom
                        anchors.left:           parent.left
                        anchors.right:          parent.right
                        horizontalAlignment:    Text.AlignHCenter
                        text:                   object.name
                        color:                  button.pressed || button.highlighted ? qgcPal.buttonHighlightText : qgcPal.buttonText
                    }

                    QGCMouseArea {
                        id:                 mouseArea
                        anchors.fill:       parent
                        hoverEnabled:       true
                        preventStealing:    true

                        onClicked:          {
                            object.createPlan(_mapCenter())
                            root.close()
                        }

                        function _mapCenter() {
                            var centerPoint = Qt.point(editorMap.centerViewport.left + (editorMap.centerViewport.width / 2), editorMap.centerViewport.top + (editorMap.centerViewport.height / 2))
                            return editorMap.toCoordinate(centerPoint, false /* clipToViewPort */)
                        }
                    }
                }
            }
        }

        QGCLabel {
            text: qsTr("Open Plan")
        }

        QGCLabel {
            Layout.leftMargin:  _sectionLeftMargin
            text:               qsTr("No saved plans found.")
            visible:            fileRepeater.model.count === 0
        }

        Flow {
            Layout.fillWidth:   true

            Repeater {
                id:                 fileRepeater
                Layout.leftMargin:  _sectionLeftMargin
                model:              fileController.getFiles(_appSettings.missionSavePath, "*." + planMasterController.fileExtension)

                QGCButton {
                    text: modelData
                    onClicked: {
                        planMasterController.loadFromFile(_appSettings.missionSavePath + "/" + modelData)
                        planMasterController.fitViewportToItems()
                        planMasterController.missionController.setCurrentPlanViewSeqNum(0, true)
                        root.close()
                    }
                }
            }
        }
    }

    QGCFileDialogController { id: fileController }
}
