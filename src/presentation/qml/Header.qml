import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import vitaliy.bondar.storyteller 1.0

Rectangle {
    id: root      
    color: "lightgray"
    height: 60
    border.width: 1
    border.color: "black"

    property var currentStory
    property var currentSample
    property int margins: 10

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
}