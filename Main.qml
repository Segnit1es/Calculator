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
        source: ":/OpenSans-SemiBold.ttf"
    }
    font.family: openSans.name

    property bool showSecretMenu: false
    property bool openBracket: true

    Item {
        anchors.fill: parent
        anchors.margins: 0

        Rectangle {
            id: statusBar
            color: "#04BFAD"
            width: parent.width
            height: parent.width * 0.08
            z: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: width * 0.03
                spacing: width * 0.03

                Item { Layout.fillWidth: true }

                Text {
                    id: timeText
                    text: Qt.formatTime(new Date(), "hh:mm")
                    color: "white"
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignRight
                }
            }

            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: timeText.text = Qt.formatTime(new Date(), "hh:mm")
            }
        }

        Rectangle {
            id: topBlock
            width: parent.width
            height: parent.height * 0.3
            color: "#04BFAD"
            radius: parent.width * 0.05
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: parent.height * 0.05 - statusBar.height * 0.7
            z: 0

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: parent.width * 0.05
                spacing: parent.height * 0.02

                Text {
                    id: expressionText
                    text: calc.expression
                    font.pixelSize: 20
                    font.letterSpacing: 0.5
                    color: "#FFFFFF"
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
                    color: "#FFFFFF"
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
                anchors.margins: parent.width * 0.05
                columns: 4
                rowSpacing: parent.height * 0.02
                columnSpacing: parent.width * 0.02

                property int btnSize: 64

                CalcButton { text: "()" ; bg: "#0889A6"; textColor: "#FFFFFF"; onClicked: { if (openBracket) calc.inputParenthesis("("); else calc.inputParenthesis(")"); openBracket = !openBracket } }
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
            spacing: parent.height * 0.03
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
