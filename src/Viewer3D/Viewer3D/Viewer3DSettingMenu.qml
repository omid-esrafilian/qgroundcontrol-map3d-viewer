import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Dialogs

import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls          1.0

import Viewer3D.Widgets 1.0

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Rectangle {

    signal mapFileChanged(string file_path)
    signal heightBiasChanged(real height)
    property alias city_map_path_text: map_file_text_feild.text
    property alias bias_height_text: height_bias_textfeild.text

    property int leftMarginSpace: ScreenTools.defaultFontPixelWidth

    id: window_body
    width: Screen.width * 0.2

    color: qgcPal.window

    QGCPalette {
        id:                 qgcPal
        colorGroupEnabled:  enabled
    }

    Text {
        id: map_file_label
        height: ScreenTools.defaultFontPixelWidth * 3
        color: "#ffffff"
        text: qsTr("3D Map File:")
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: ScreenTools.defaultFontPixelWidth * 2
        anchors.leftMargin: leftMarginSpace
        anchors.topMargin: ScreenTools.defaultFontPixelWidth * 3
    }

    MyRoundButton{
        id: map_file_btn
//        x: 450
        width: ScreenTools.defaultFontPixelWidth * 15
        height: ScreenTools.defaultFontPixelWidth * 5
        text: "Select File"
        anchors.right: map_file_text_feild.right
        anchors.top: map_file_text_feild.bottom
        anchors.topMargin: 10
        anchors.rightMargin: 0
        buttonTextColor: "black"
        buttonColor: "#ffffff"
        icon.color: "#ffffff"

        Connections {
            target: map_file_btn
            onClicked:{
                folderDialog.open()
            }
        }

        FileDialog {
            id: folderDialog
            fileMode: FileDialog.OpenFile
//            selectMultiple: false
            onAccepted: {
                var path = selectedFile.toString();
//                var path = folderDialog.fileUrl.toString();
                // remove prefixed "file:///"
                path = path.replace(/^(file:\/{3})/,"");
                // unescape html codes like '%23' for '#'
                var cleanPath = decodeURIComponent(path);

                map_file_text_feild.text = cleanPath
                mapFileChanged(cleanPath)
            }
        }
    }

    TextFieldString {
        id: map_file_text_feild
        height: ScreenTools.defaultFontPixelWidth * 4.5
        anchors.verticalCenter: map_file_label.verticalCenter
        anchors.right: parent.right
        anchors.left: map_file_label.right
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        readOnly: true
        text: city_map_path_text
    }

    Text {
        id: height_bias_label
        height: ScreenTools.defaultFontPixelWidth * 3
        color: "#ffffff"
        text: qsTr("Vehicle Height Bias:")
        anchors.left: parent.left
        anchors.top: map_file_btn.bottom
        font.pixelSize: ScreenTools.defaultFontPixelWidth * 2
        anchors.leftMargin: leftMarginSpace
        anchors.topMargin: ScreenTools.defaultFontPixelWidth * 2
    }

    TextFieldDouble{
        id: height_bias_textfeild

        width: ScreenTools.defaultFontPixelWidth * 15
        height: ScreenTools.defaultFontPixelWidth * 4.5
        anchors.verticalCenter: height_bias_label.verticalCenter
        anchors.left: height_bias_label.right
        anchors.leftMargin: 20
        maximumLength: 6
        unit_text: "m"
        max_limit: 1e10
        min_limit: -1e10
        text: bias_height_text

        onContentChanged: {
            heightBiasChanged(parseFloat(text))
        }
    }
}
