import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle {
    width: 200
    height: 800
    color: "#DDDDDD"

    Rectangle {
        id: toolBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: 20
        color: "gray"
        Rectangle {
            id: btnAdd
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            color: "transparent"
            border.width: 1
            border.color: "white"
            anchors.margins: 1


            Text {
                anchors.fill: parent
                text: "\uf067"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font.family: "FontAwesome"
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: table.requestAdd()
            }
        }
        Rectangle {
            id: lblTotalMultiplier
            anchors.left: btnAdd.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: btnMultiplierLock.left
            color: "transparent"
            border.width: 1
            border.color: "white"
            anchors.margins: 1


            Text {
                anchors.fill: parent
                text: "Total Multiplier : " + constants.totalMultiplier.toFixed(2)
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font.family: "FontAwesome"
                color: "white"
            }
        }
        Rectangle {
            id: btnMultiplierLock
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            color: "transparent"
            border.width: 1
            border.color: "white"
            anchors.margins: 1


            Text {
                anchors.fill: parent
                text: constants.fixedMultiplierTotal ? "\uf023" : "\uf09c"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font.family: "FontAwesome"
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: constants.setFixedMultiplierTotal(!constants.fixedMultiplierTotal)
            }
        }

    }

    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        model: itemmanager.itemList
        clip: true
        delegate: Rectangle {
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right

            Rectangle {
                id: cellName
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom:  bottomLine.top
                anchors.right: cellMultiplier.left

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    text: model.modelData.name
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    font.family: "FontAwesome"
                }
            }
            Rectangle {
                id: cellMultiplier
                anchors.right: cellDelete.left
                anchors.top: parent.top
                anchors.bottom:  bottomLine.top
                width: height

                Text {
                    anchors.fill: parent
                    text: (model.modelData.multiplier).toFixed(2)
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.family: "FontAwesome"
                }
            }
            Rectangle {
                id: cellDelete
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: bottomLine.top
                width: height

                Text {
                    anchors.fill: parent
                    text: "\uf1f8"
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.family: "FontAwesome"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: table.requestRemove(model.index)
                }
            }

            Rectangle {
                id: bottomLine
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                height: 1
                anchors.leftMargin:model.index == listView.count-1 ? 0 :  5
                anchors.rightMargin: model.index == listView.count-1 ? 0 :  5
                color: "silver"

            }
        }

    }

    FontLoader {
        id: fontAwesome
        name: "FontAwesome"
        source: "qrc:/ui/font/fontawesome-webfont.ttf"
    }
}
