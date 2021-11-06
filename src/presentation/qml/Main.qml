import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtMultimedia 5.9

import vitaliy.bondar.storyteller 1.0

Item {
    id: root
    width: Screen.width
    height: Screen.height

    property int numCols: 5
    property int itemHeight: (Screen.width - 2 * margins - (numCols - 1) * grid.columnSpacing) / numCols
    property int itemWidth: itemHeight
    property int margins: 10

    property var currentStory
    property var currentSample
    property int storyPart: 0

    property bool controlsEnabled: footer.locked

    signal appendFile(string storyUrl, string tittle, string preview)
    signal appendUrl(string storyUrl)
    signal removeStory(string uid)

    Component {
        id: storyGroupDelegate

        Item {
            width: root.itemWidth
            height: root.itemHeight

            Rectangle {
                id: itemRect
                anchors.fill: parent
                border.width: 3
                border.color: (root.currentStory == modelData) ? "green" : "red"
                radius: 5
                color: "transparent"
                Image {
                    anchors.margins: 3
                    anchors.fill: parent
                    source: modelData.preview
                    fillMode: Image.PreserveAspectCrop
                }

                SequentialAnimation on scale {
                    running: (root.currentStory == modelData)
                    loops: Animation.Infinite
                    PropertyAnimation { to: 0.7; duration: 1500 }
                    PropertyAnimation { to: 1.0; duration: 1500 }
                    onRunningChanged: if (!running) itemRect.scale = 1.0
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (root.currentStory == modelData) {
                        root.currentStory = undefined
                    }
                    else {
                        root.currentStory = modelData
                    }
                }
                onPressAndHold: {
                    if (!controlsEnabled) return;
                    modifyLoader.story = modelData
                    modifyLoader.source = "qrc:/qml/ModifyDialog.qml"
                }
            }
        }
    }

    Header {
        id: header

        anchors {
            margins: root.margins
            top: parent.top
            left: parent.left
            right: parent.right
        }

        currentStory: root.currentStory
        currentSample: root.currentSample
        margins: root.margins
    }

    Flickable {
        id: flick
        anchors {
            margins: root.margins
            top: header.bottom
            bottom: footer.top
            left: parent.left
            right: parent.right
        }

        flickableDirection: Flickable.VerticalFlick

        contentHeight: grid.height
        contentWidth: width
        clip: true

        Grid {
            id: grid
            columns: root.numCols
            columnSpacing: 10
            rowSpacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Repeater {
                model: storyList.stories
                delegate: storyGroupDelegate
            }
        }
    }

    Footer {
        id: footer

        anchors {
            margins: root.margins
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        currentStory: root.currentStory
        currentSample: root.currentSample
        storyPart: root.storyPart
        margins: root.margins

        position: player.position
        duration: player.duration
        playbackState: player.playbackState

        onBackward: {
            --root.storyPart
            root.playSample()
        }

        onForward: {
            ++root.storyPart
            root.playSample()
        }

        onPlay: {
            player.play()
        }

        onPause: {
            player.pause()
        }

        onStop: {
            root.currentStory = undefined
        }

        onAppend: {
            appendLoader.source = "qrc:/qml/AppendDialog.qml"
        }

        onSeek: {
            player.seek(pos)
        }
    }

    Loader {
        id: appendLoader
        onLoaded: {
           item.appendUrl.connect(root.appendUrl)
           item.appendFile.connect(root.appendFile)
           item.accepted.connect(onRejected)
           item.rejected.connect(onRejected)
           item.open()
        }

        function onRejected() {
            console.log("Main onRejected")
            item.close()
            source = ""
        }
    }

    Loader {
        id: modifyLoader
        property var story

        onLoaded: {
           item.removeStory.connect(onRemoveStory)
           item.accepted.connect(onAccepted)
           item.rejected.connect(onRejected)
           item.story = modifyLoader.story
           item.open()
        }

        function onRemoveStory(uid) {
            console.log("onRemoveStory")
            if (root.currentStory && root.currentStory.id == uid) {
                player.stop()
            }
            root.removeStory(uid)
        }

        function onRejected() {
            console.log("Main onRejected")
            item.close()
            source = ""
        }

        function onAccepted() {
            console.log("Main onAccepted")
            onRejected()
        }
    }

    Audio {
        id: player
        autoLoad: true
        autoPlay: true
        loops: footer.repeatMode == footer.repeatOne ? Audio.Infinite : 1
        onSourceChanged: {
            console.log("onSourceChanged")
        }
        onPlaying: {
            console.log("onPlaying", playbackRate)
        }
        onPaused: {
            console.log("onPaused")
        }
        onStopped: {
            console.log("onStopped", status)
            if (status == Audio.EndOfMedia) {
                if (footer.repeatMode == footer.repeatOne) {
                    return  // see loops property
                } else if (root.storyPart < root.currentStory.stories.length - 1) {
                    ++root.storyPart
                } else if (footer.repeatMode == footer.repeatRepeatAll) {
                    root.storyPart = 0
                } else {
                    return
                }

                root.playSample()
            }
        }
    }

    onCurrentStoryChanged: {
        root.storyPart = 0
        if (!root.currentStory) {
            player.stop()
            player.source = ""
            root.currentSample = undefined
            return
        }

        playSample();
    }

    function playSample() {
        root.currentSample = root.currentStory.stories[root.storyPart]
        player.source = root.currentSample.url
    }
}
