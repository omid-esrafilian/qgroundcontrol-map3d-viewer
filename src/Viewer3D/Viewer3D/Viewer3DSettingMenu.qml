import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QGroundControl.Palette
import QGroundControl.ScreenTools
import QGroundControl.Controls

import QGroundControl.Viewer3D

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Rectangle {

    signal mapFileChanged(string file_path)
    signal heightBiasChanged(real height)
    property string windowState: "SETTING_MENU_CLOSE"
    property real default_width: Screen.width * 0.2

    property int leftMarginSpace: ScreenTools.defaultFontPixelWidth

    id: window_body
    clip: true
    color: qgcPal.window

    Viewer3DFacts{
        id: _viewer3DFacts
    }

    QGCPalette {
        id:                 qgcPal
        colorGroupEnabled:  enabled
    }

    QGCLabel {
        id: map_file_label
        Layout.fillWidth:   true
        wrapMode:           Text.WordWrap
        visible:            true
        text: qsTr("3D Map File:")
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: leftMarginSpace
        anchors.topMargin: ScreenTools.defaultFontPixelWidth * 3
    }

    QGCButton {
        id: map_file_btn
        anchors.right: map_file_text_feild.right
        anchors.top: map_file_text_feild.bottom
        anchors.topMargin: ScreenTools.defaultFontPixelWidth * 2
        anchors.rightMargin: ScreenTools.defaultFontPixelWidth

        visible:    true
        text:       qsTr("Select File")

        onClicked: {
            fileDialog.openForLoad()
        }

        QGCFileDialog {
            id:             fileDialog

            nameFilters:    [qsTr("OpenStreetMap files (*.osm)")]
            title:          qsTr("Select map file")
            onAcceptedForLoad: (file) => {
                                   map_file_text_feild.text = file
                                   mapFileChanged(file)
                               }
        }
    }

    QGCTextField {
        id:                 map_file_text_feild
        height:             ScreenTools.defaultFontPixelWidth * 4.5
        unitsLabel:         ""
        showUnits:          false
        visible:            true

        anchors.verticalCenter: map_file_label.verticalCenter
        anchors.right: parent.right
        anchors.left: map_file_label.right
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        readOnly: true

        text: (_viewer3DFacts.qmlBackend)?(_viewer3DFacts.qmlBackend.city_map_path):("nan")
    }

    QGCLabel {
        id: height_bias_label
        Layout.fillWidth:   true
        wrapMode:           Text.WordWrap
        visible:            true
        text: qsTr("Vehicle Height Bias:")
        anchors.left: parent.left
        anchors.top: map_file_btn.bottom
        anchors.leftMargin: leftMarginSpace
        anchors.topMargin: ScreenTools.defaultFontPixelWidth * 2
    }

    QGCTextField {
        id:                 height_bias_textfeild
        width:              ScreenTools.defaultFontPixelWidth * 15
        unitsLabel:         "m"
        showUnits:          true
        numericValuesOnly:  true
        visible:            true

        anchors.verticalCenter: height_bias_label.verticalCenter
        anchors.left:           height_bias_label.right
        anchors.leftMargin:     ScreenTools.defaultFontPixelWidth * 2

        text: (_viewer3DFacts.qmlBackend)?(Number(_viewer3DFacts.qmlBackend.height_bias)):("nan")

        validator: RegularExpressionValidator{
            regularExpression: /(-?\d{1,10})([.]\d{1,6})?$/
        }

        onAccepted:
        {
            focus = false
            heightBiasChanged(parseFloat(text))
        }
    }

    onMapFileChanged: function(file_path){
        console.log(file_path)
        _viewer3DFacts.qmlBackend.city_map_path = file_path
    }

    onHeightBiasChanged: function(height){
        _viewer3DFacts.qmlBackend.height_bias = height
    }

    Behavior on width{
        NumberAnimation{
            easing.type: Easing.InOutQuad;
            duration: 300
        }
    }

    state: windowState

    states: [
        State {
            name: "SETTING_MENU_OPEN"
            PropertyChanges { target: window_body; width: default_width; visible:true}

        },
        State {
            name: "SETTING_MENU_CLOSE"
            PropertyChanges { target: window_body; width: 0}
        }
    ]
}
