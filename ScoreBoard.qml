import QtQuick 2.7

ListView {
    model: nbaobj
    delegate: Rectangle {
        height: 30
        width: parent.width
        color: "#f0f0f0"
        Text {
            anchors.centerIn: parent
            text: awayTeam + " " + awayScore + ":" + homeScore + " " + homeTeam + " " + state
            color: isFocusedTeam ? "red" : "black"
        }
    }
}
