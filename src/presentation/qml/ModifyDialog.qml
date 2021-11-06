import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtMultimedia 5.9
import QtQuick.Dialogs 1.2

import vitaliy.bondar.storyteller 1.0

Dialog {
    id: dialog
    title: "Modify story"

    property var story

    signal removeStory(string uid)

    contentItem: Rectangle {
        implicitWidth: Screen.width * 0.9
        implicitHeight: Screen.height * 0.9
        color: "gray"

        ColumnLayout {
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: buttons.top
                margins: 10
            }

            RowLayout {
                Image {
                    sourceSize.width: 80
                    sourceSize.height: 80
                    source: story.preview
                    fillMode: Image.PreserveAspectCrop
                }

                Label {
                    text: qsTr("Tittle:")
                    verticalAlignment: Text.AlignTop
                }

                TextField {
                    id: tittle
                    Layout.fillWidth : true
                    verticalAlignment: Text.AlignTop

                    property bool textChanged: false

                    onTextChanged: textChanged = true
                }
            }

            ListView {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Layout.fillHeight : true

                clip: true
                spacing: 2
                model: story.storiesAll
                delegate: storyDelegate
            }
        }

        RowLayout {
            id: buttons
            anchors {
                margins: 10
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            Button {
                id: btnOk
                text: "Ok"
                onClicked: {
                    console.log("Dialog accepted")
                    if (tittle.textChanged) story.tittle = tittle.text
                    dialog.accepted()
                }
            }

            Item {Layout.fillWidth : true}

            Button {
                id: btnRemove
                text: "Remove"
                onClicked: {
                    console.log("Dialog accepted")
                    removeStory(story.id)
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


    Component {
        id: storyDelegate

        Rectangle {
            width: parent.width
            height: 40
            radius: 5
            border.color: "black"
            border.width: 1
            color: "transparent"

            Text {
                anchors.fill: parent
                anchors.margins: 10
                text: modelData.tittle
                font.strikeout: modelData.isDeleted
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    modelData.isDeleted = !modelData.isDeleted
                }
            }
        }
    }

    onStoryChanged: {
        tittle.text = story.tittle
    }

    Component.onCompleted: {
        console.log("Dialog load completed")
    }
}
