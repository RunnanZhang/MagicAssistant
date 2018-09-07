import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQml 2.2
import QtQuick 2.4

TextEdit {
    width: 240
    text: nbaobj[1].awayTeam
    font.family: "Helvetica"
    font.pointSize: 20
    color: "blue"
    focus: true
}
