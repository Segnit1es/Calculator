import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: app
    visible: true
    width: 360
    height: 720
    title: "Calculator"
    color: "#024873"

    FontLoader {
        id: openSans
        source: "qrc:/fonts/OpenSans-SemiBold.ttf"

    }
    font.family: openSans.name

    property bool showSecretMenu: false

    Item {
        anchors.fill: parent
        anchors.margins: 0

        Rectangle {
            id: topBlock
            width: parent.width
            height: 240
            color: "#00A89D"
            radius: 24
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 12

                Text {
                    id: expressionText
                    text: calc.expression
                    font.pixelSize: 20
                    font.letterSpacing: 0.5
                    color: "#CFF2EE"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                Text {
                    id: resultText
                    text: calc.result
                    font.pixelSize: 50
                    font.letterSpacing: 0.5
                    font.bold: true
                    color: "white"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
            }
        }

        Rectangle {
            id: bottomBlock
            anchors.top: topBlock.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#024873"

            Grid {
                id: keyGrid
                anchors.fill: parent
                anchors.margins: 24
                columns: 4
                rowSpacing: 16
                columnSpacing: 16

                property int btnSize: 64

                CalcButton { text: "()" ; bg: "#0889A6"; textColor: "#FFFFFF" }
                CalcButton { text: "+/-" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onClicked: calc.toggleSign() }
                CalcButton { text: "%" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onClicked: calc.pressPercent() }
                CalcButton { text: "÷" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onClicked: calc.pressOperator("/") }

                CalcButton { text: "7" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "8" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "9" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "×" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onClicked: calc.pressOperator("*") }

                CalcButton { text: "4" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "5" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "6" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "−" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onClicked: calc.pressOperator("-") }

                CalcButton { text: "1" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "2" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "3" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "+" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onClicked: calc.pressOperator("+") }

                CalcButton { text: "C" ; bg: "#F25E5E"; textColor: "#FFFFFF" ; onClicked: calc.clearAll() }
                CalcButton { text: "0" ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "." ; onClicked: calc.inputDot() }
                CalcButton { text: "=" ; bg: "#0889A6"; textColor: "#FFFFFF" ; onPressed: calc.startLongPress() ; onReleased: calc.endLongPress() }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#024873"
        visible: showSecretMenu
        z: 50

        Column {
            anchors.centerIn: parent
            spacing: 20
            Text { text: "Секретное меню"; font.pixelSize: 24; color: "white" }
            Button { text: "Назад"; onClicked: showSecretMenu = false }
        }
    }
    Connections {
        target: calc
        function onShowSecretMenu() {
            app.showSecretMenu = true
        }
    }
}
