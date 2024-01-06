import QtQuick 2.9
import QtQuick.Controls 2.0

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Item{
    id:textf_feild_body

    property alias maximumLength: text_feild.maximumLength
    property string unit_text: ""
    property alias readOnly: text_feild.readOnly
    property alias text: text_feild.text
    property bool active: true
    property alias textColor: text_feild.color

    width: 50
    height: 20

    TextField{
        id: text_feild
        anchors.fill: parent
        color: textf_feild_body.textColor
        verticalAlignment: Text.AlignVCenter

        readOnly: textf_feild_body.readOnly
        text: textf_feild_body.text

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
    }

    Text {
        id: unit_txt
        text: unit_text

        font{
            pixelSize: parent.height * 0.45
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
