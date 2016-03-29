import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

Item {
     width: 200; height:200
     Rectangle {
         color: "green"
         width: 100; height: 100
         objectName: "rect"
         Rectangle {
             color: "blue"
             x: 100; y: 100; width: 100; height: 100
         }
     }
}
