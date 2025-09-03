import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    // интерфейс
    property alias text: label.text
    property color bg: "#BFE9EE"
    property color textColor: "#013A3A"
    property int size: 64

    signal clicked()
    signal released()
    signal pressed()

    width: size
    height: size
    radius: width / 2

    // фон по умолчанию
    color: bg
    border.width: 1
    border.color: Qt.darker(color, 1.2)

    // анимация нажатия
    property bool down: false
    Behavior on scale { NumberAnimation { duration: 80 } }
    scale: down ? 0.97 : 1.0

    // подсветка (на 100 мс)
    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: "#FFD93D"
        visible: flashTimer.running
        z: 1
    }

    Text {
        id: label
        anchors.centerIn: parent
        color: textColor
        font.pixelSize: 20
        font.weight: Font.Bold
        z: 2
    }

    MouseArea {
        anchors.fill: parent
        onPressed: root.down = true
        onReleased: {
            root.down = false
            flashTimer.start()
            root.clicked()
        }
        onCanceled: root.down = false
    }

    Timer {
        id: flashTimer
        interval: 100
        repeat: false
    }
}
