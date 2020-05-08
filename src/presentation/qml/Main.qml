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

    property int itemHeight: 60
    property int itemWidth: 60
    property int margins: 10

    property var currentStory
    property var currentSample
    property int storyPart: 0

    signal appendFile(string storyUrl, string tittle, string preview)
    signal appendUrl(string storyUrl)
    signal removeStory(string uid)

    Component {
        id: storyDelegate

        Item {
            width: root.itemWidth
            height: root.itemHeight

            Rectangle {
                id: itemRect
                anchors.fill: parent
                border.width: 2
                border.color: (root.currentStory == modelData) ? "green" : "red"
                radius: 5
                color: "gray"
                Image {
                    anchors.margins: 2
                    anchors.fill: parent
                    source: modelData.preview
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
                onDoubleClicked: {
                    if (root.currentStory == modelData) {
                        player.stop()
                    }
                    root.removeStory(modelData.id)
                }
            }
        }

    }

    Rectangle {
        id: header
        height: 60
        color: "lightgray"
        border.width: 1
        border.color: "black"
        anchors {
            margins: root.margins
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Text {
            anchors.fill: parent
            text: {
                if (!root.currentStory || !root.currentSample) return ""
                console.log(root.currentStory.tittle, root.currentSample.tittle)
                if (root.currentStory.tittle == root.currentSample.tittle) {
                    return root.currentSample.tittle
                } else {
                    return root.currentStory.tittle + "\n" + root.currentSample.tittle
                }
            }
            wrapMode: Text.WordWrap
            maximumLineCount: 3
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ToolButton {
            anchors.right: parent.right
            height: parent.height
            width: height

            Image {
                anchors.fill: parent
                source: "qrc:/icons/settings.svg"
            }

            onClicked: {
                loader.source = "qrc:/qml/AppendDialog.qml"
            }
        }
    }

    Grid {
        anchors {
            margins: root.margins
            top: header.bottom
            left: parent.left
        }

        columns: parent.width / (root.itemWidth + columnSpacing)
        columnSpacing: 10
        rowSpacing: 10

        Repeater {
            model: storyList.stories
            delegate: storyDelegate
        }
    }

    RowLayout {
        id: footer
        height: 40

        anchors {
            margins: root.margins
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        ToolButton {
            height: parent.height
            width: 60
            text: "<==="
            enabled: root.storyPart > 0
            onClicked: {
                --root.storyPart
                root.playSample()
            }
        }

        Item {Layout.fillWidth : true}

        Text {
            text: root.currentSample ? msecToTime(player.position) : "--:--"
        }

        Item {Layout.fillWidth : true}

        ToolButton {
            height: parent.height
            width: height
            enabled: (root.currentSample != undefined)
            Text {
                anchors.centerIn: parent
                text: player.playbackState == Audio.PlayingState ? "||" : ">"
            }
            onClicked: {
                player.playbackState == Audio.PlayingState ? player.pause() : player.play()
            }
        }

        Item {Layout.fillWidth : true}

        Text {
            text: root.currentSample ? msecToTime(player.duration) : "--:--"
        }

        Item {Layout.fillWidth : true}

        ToolButton {
            height: parent.height
            width: 60
            enabled: (root.currentStory != undefined && root.storyPart < root.currentStory.stories.length - 1)
            text: "===>"
            onClicked: {
                ++root.storyPart
                root.playSample()
            }
        }
    }

    Loader {
        id: loader
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

    Audio {
        id: player
        // muted: true  // FIXME
        autoLoad: true
        autoPlay: true
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
            if (status == Audio.EndOfMedia && root.storyPart < root.currentStory.stories.length - 1) {
                ++root.storyPart
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

    function msecToTime(msec) {
        var sec_num = parseInt(Math.round(msec / 1000), 10);
        var hours   = Math.floor(sec_num / 3600);
        var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
        var seconds = sec_num - (hours * 3600) - (minutes * 60);

        var out = ""
        if (hours > 0) {
            if (hours < 10) { out += "0"}
            out += hours + ":";
        }
        if (minutes < 10) {minutes = "0" + minutes;}
        if (seconds < 10) {seconds = "0" + seconds;}
        return out + minutes + ':' + seconds;
    }
}
