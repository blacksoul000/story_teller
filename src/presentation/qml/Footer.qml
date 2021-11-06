import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtMultimedia 5.9

import vitaliy.bondar.storyteller 1.0

ColumnLayout {
    id: root
    height: 100
    Layout.fillWidth : true

    property var currentStory
    property var currentSample
    property int storyPart
    property int margins: 10

    property int position
    property int duration
    property var playbackState

    property int maxTimeoutMin: 180

    property alias locked: lock.checked
    property alias repeatMode: repeat.mode
    property int repeatNone: 0
    property int repeatOne: 1
    property int repeatAll: 2

    signal backward()
    signal forward()
    signal play()
    signal pause()
    signal stop()
    signal append()
    signal seek(int pos)

    Slider {
        id: slider

        Layout.fillWidth: true
        enabled: root.currentSample != null

        from: 0
        to: root.currentSample ? duration : 0

        onMoved: {
            seek(value)
        }
    }

    RowLayout {
        ColumnLayout {
            spacing: 5
            Text {
                font.pointSize: 16
                text: root.currentSample ? msecToTime(position) : "--:--"
            }
            Text {
                font.pointSize: 16
                text: root.currentSample ? msecToTime(duration) : "--:--"
            }
        }

        Item {Layout.fillWidth : true}

        ToolButton {
            implicitHeight: 60
            implicitWidth: implicitHeight
            enabled: root.storyPart > 0
            visible: !root.locked
            Image {
                anchors.fill: parent
                source: "qrc:/icons/backward.svg"
            }
            onClicked: root.backward()
        }

        ToolButton {
            implicitHeight: 60
            implicitWidth: implicitHeight
            enabled: root.locked
            visible: root.locked
            Image {
                anchors.fill: parent
                source: "qrc:/icons/plus.svg"
            }
            onClicked: root.append()
        }

        Item {Layout.fillWidth : true}

        ToolButton {
            implicitHeight: 60
            implicitWidth: implicitHeight
            enabled: (root.currentSample != undefined)
            visible: !root.locked
            Image {
                anchors.fill: parent
                source: root.playbackState == Audio.PlayingState ? "qrc:/icons/pause.svg" : "qrc:/icons/play.svg"
            }
            onClicked: {
                root.playbackState == Audio.PlayingState ? root.pause() : root.play()
            }
        }

        ToolButton {
            id: repeat
            implicitHeight: 60
            implicitWidth: implicitHeight
            enabled: root.locked
            visible: root.locked

            property int mode: repeatNone

            Image {
                anchors.fill: parent
                source: "qrc:/icons/repeat.svg"
                Text {
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }
                    text: {
                        if (repeat.mode == repeatNone) return "Off"
                        if (repeat.mode == repeatOne) return "1"
                        if (repeat.mode == repeatAll) return "All"
                    }
                }
            }
            onClicked: {
                if (mode == repeatAll) mode = repeatNone
                else ++mode;
            }
        }

        Item {Layout.fillWidth : true}

        ToolButton {
            implicitHeight: 60
            implicitWidth: implicitHeight
            enabled: (root.currentStory != undefined && root.storyPart < root.currentStory.stories.length - 1)
            visible: !root.locked
            Image {
                anchors.fill: parent
                source: "qrc:/icons/forward.svg"
            }
            onClicked: root.forward()
        }

        ToolButton {
            id: timerBtn
            implicitHeight: 60
            implicitWidth: implicitHeight
            enabled: root.locked
            visible: root.locked
            property int timeoutMin: 0
            Image {
                anchors.fill: parent
                source: "qrc:/icons/timer.svg"

                Text {
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }
                    text: timerBtn.timeoutMin == 0 ? "Off" : "" + timerBtn.timeoutMin
                }
            }
            Timer {
                interval: 60000
                running: timerBtn.timeoutMin > 0
                repeat: true
                onTriggered: {
                    --timerBtn.timeoutMin
                    if (timerBtn.timeoutMin == 0) root.stop()
                }
            }
            onClicked: {
                timerBtn.timeoutMin = (timerBtn.timeoutMin + 15 - timerBtn.timeoutMin % 15)
                if (timerBtn.timeoutMin == root.maxTimeoutMin) timerBtn.timeoutMin = 0
            }
        }

        Item {Layout.fillWidth : true}

        DelayButton {
            id: lock
            implicitHeight: 60
            implicitWidth: implicitHeight
            delay: 1000
            Image {
                anchors.fill: parent
                source: "qrc:/icons/locked.svg"
            }
        }
    }

    onPositionChanged: {
        slider.value = position
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
