import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property alias text: label.text

    property color primaryBg: "#B0D1D8"
    property color primaryText: "#024873"
    property color altBg: "#FFD93D"
    property color altText: "#024873"

    property int size: 64

    signal clicked()
    signal released()
    signal pressed()

    width: size
    height: size
    radius: width / 2

    color: primaryBg
    border.width: 1
    border.color: Qt.darker(primaryBg, 1.2)

    property bool down: false
    Behavior on scale { NumberAnimation { duration: 80 } }
    scale: down ? 0.97 : 1.0

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: altBg
        visible: flashTimer.running
        z: 1
    }

    Text {
        id: label
        anchors.centerIn: parent
        color: flashTimer.running ? altText : primaryText
        font.pixelSize: 24
        font.letterSpacing: 1
        font.weight: Font.Bold
        z: 2
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            root.down = true
            root.pressed()
        }
        onReleased: {
            root.down = false
            flashTimer.start()
            root.released()
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
