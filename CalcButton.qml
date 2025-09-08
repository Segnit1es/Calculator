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

    property string fontFamily: ""
    property real fontFactor: 1.0

    property url source: ""
    property bool useImage: source != ""

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
        visible: !useImage
        anchors.centerIn: parent
        color: flashTimer.running ? altText : primaryText
        font.pixelSize: 24 * fontFactor
        font.letterSpacing: 1 * fontFactor
        font.family: fontFamily
        z: 2
    }

    Image {
        id: icon
        anchors.centerIn: parent
        visible: useImage
        source: root.source
        width: parent.width * 0.6 * fontFactor
        height: parent.height * 0.6 * fontFactor
        fillMode: Image.PreserveAspectFit
        z: 3
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
