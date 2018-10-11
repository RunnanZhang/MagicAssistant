import QtQuick 2.7
import QtQuick.Controls 2.0

ListView {
    model: nbaobj
    delegate: Rectangle {
        height: 30
        width: parent.width
        color: "#f0f0f0"
        TextEdit {
            anchors.centerIn: parent
            // state是当前Item的属性关键字，所以可以使用此方法区分.
            text: awayTeam + " " + awayScore + ":" + homeScore + " " + homeTeam + " " + model.modelData.state
            color: isFocusedTeam ? "red" : "black"
            readOnly: true
            wrapMode: Text.WordWrap
            selectByMouse: true
            selectionColor: "#3399ff"
        }
    }

    ScrollBar.vertical: ScrollBar {}
}
