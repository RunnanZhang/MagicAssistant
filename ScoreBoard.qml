import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

Item {
    id: buttonrow
    width: 250; height: 50
    RowLayout {
        TextField {
            id: txtvalue1
            text: nbaobj.teamscore[0].homeTeam // get the value by MyCppObject::value1
        }
        TextField {
            id: txtvalue2
            text: "23232" // the value by MyCppObject::value2
        }
    }
}
