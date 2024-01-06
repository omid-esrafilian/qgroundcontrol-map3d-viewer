import QtQuick
import QtQuick.Controls 2.0

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Item{
    id: textf_feild_body
    property alias maximumLength: text_feild.maximumLength
    property string unit_text: ""
    property real max_limit: 1e10
    property real min_limit: -1e10
    property alias readOnly: text_feild.readOnly
    property alias text: text_feild.text
    property bool active: true
    property alias textColor: text_feild.color

    signal contentChanged(string text_val)

    width: 50
    height: 20

    TextField{
        id: text_feild
        anchors.fill: parent
        color: textf_feild_body.textColor
        readOnly: textf_feild_body.readOnly
        text: textf_feild_body.text
        verticalAlignment: Text.AlignVCenter

        property string oldText
        property bool cancelling

        font{
            pixelSize: parent.height * 0.45
        }
        focus: false
        selectByMouse: active

        onAccepted:
        {
            text_feild.focus = false
            oldText = text
            contentChanged(text)
        }

        Keys.onEscapePressed: {
            focus = false
            cancelling = true
            text = oldText
            oldText = ""
        }

        onActiveFocusChanged: {
            // When we first gain focus, save the old text and select everything for clearing.
            if (activeFocus) {
                oldText = text
                if(active)
                    selectAll()
            }
        }


        selectionColor: "gray"
        selectedTextColor: "#ffffff"
// Map3DQt6
        validator: RegularExpressionValidator{
            regularExpression: /(-?\d{1,10})([.]\d{1,6})?$/
        }
    }

    Text {
        id: unit_txt
        text: unit_text

        font{
            pixelSize: parent.height * 0.5
        }
        color: "#202020"

        anchors
        {
            right: text_feild.right
            rightMargin:4
            verticalCenter: parent.verticalCenter
        }
    }
}
