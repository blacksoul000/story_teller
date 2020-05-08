import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtMultimedia 5.9
import QtQuick.Dialogs 1.2

import vitaliy.bondar.storyteller 1.0

Dialog {
    id: dialog
    title: "Add story"

    signal appendUrl(string storyUrl)
    signal appendFile(string storyUrl, string tittle, string preview)

    contentItem: Rectangle {
        implicitWidth: Screen.width * 0.9
        implicitHeight: Screen.height * 0.9
        // border.width: 2
        // border.color: "red"
        color: "gray"

        GridLayout {
            columns: 2
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 10
            }

            RadioButton {
                id: urlWeb
                checked: true
                text: qsTr("Url")
                Layout.columnSpan: 2
            }

            Label {
                text: qsTr("Url:")
                enabled: urlWeb.checked
            }

            TextField {
                id: urlInput
                enabled: urlWeb.checked
                cursorVisible: false
                Layout.fillWidth : true
                onTextChanged: {
                    btnOk.enabled = text.length > 0
                }
            }

            RadioButton {
                id: urlFile
                text: qsTr("File")
                Layout.columnSpan: 2
            }

            Label {
                text: qsTr("Url:")
                enabled: urlFile.checked
            }

            TextField {
                id: fileUrlInput
                enabled: urlFile.checked
                cursorVisible: false
                Layout.fillWidth : true
                onTextChanged: {
                    btnOk.enabled = text.length > 0
                }
            }

            Label {
                text: qsTr("Tittle:")
                enabled: urlFile.checked
            }

            TextField {
                id: tittleInput
                enabled: urlFile.checked
                cursorVisible: false
                Layout.fillWidth : true
            }

            Label {
                text: qsTr("Preview:")
                enabled: urlFile.checked
            }

            TextField {
                id: previewInput
                enabled: urlFile.checked
                cursorVisible: false
                Layout.fillWidth : true
            }
        }

        RowLayout {
            anchors {
                margins: 10
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            Button {
                id: btnOk
                enabled: false
                text: "Ok"
                onClicked: {
                    console.log("Dialog accepted")
                    if (urlWeb.checked) dialog.appendUrl(urlInput.text)
                    if (urlFile.checked) dialog.appendFile(fileUrlInput.text, tittleInput.text, previewInput.text)
                    dialog.accepted()
                }
            }

            Item {Layout.fillWidth : true}

            Button {
                text: "Cancel"
                onClicked: {
                    console.log("Dialog rejected")
                    dialog.rejected()
                }
            }
        }
    }
    Component.onCompleted: {
        console.log("Dialog load completed")
    }
}