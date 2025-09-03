import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3



ApplicationWindow {
    id: app
    visible: true
    width: 360
    height: 720
    title: "Calculator"

    FontLoader {
        id: openSans
        source: "qrc:/fonts/OpenSans-SemiBold.ttf"
    }
    font.family: openSans.name

    property bool showSecretMenu: false
    property string currentOperator: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12
        Rectangle {
            Layout.preferredHeight: 170
            Layout.fillWidth: true
            radius: 12
            color: "#00A89D"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 6
                Text {
                    id: expressionText
                    text: calc.expression
                    font.pixelSize: 16
                    color: "#CFF2EE"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                Text {
                    id: resultText
                    text: calc.result
                    font.pixelSize: 44
                    font.bold: true
                    color: "white"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 12
            color: "#0A3540"

            Grid {
                id: keyGrid
                anchors.fill: parent
                anchors.margins: 18
                columns: 4
                rowSpacing: 14
                columnSpacing: 14

                property int btnSize: 64

                CalcButton { text: "()" ; bg: "#0D6B73"; textColor: "white"; size: btnSize }
                CalcButton { text: "+/-" ; bg: "#0D6B73"; textColor: "white"; size: btnSize ; onClicked: calc.toggleSign() }
                CalcButton { text: "%" ; bg: "#0D6B73"; textColor: "white"; size: btnSize ; onClicked: calc.pressPercent() }
                CalcButton { text: "÷" ; bg: "#0D6B73"; textColor: "white" ; size: btnSize ; onClicked: calc.pressOperator("/") }

                CalcButton { text: "7" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "8" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "9" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "×" ; bg: "#0D6B73"; textColor: "white" ; size: btnSize ; onClicked: calc.pressOperator("*") }

                CalcButton { text: "4" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "5" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "6" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "−" ; bg: "#0D6B73"; textColor: "white" ; size: btnSize ; onClicked: calc.pressOperator("-") }

                CalcButton { text: "1" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "2" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "3" ; bg: "#BFE9EE"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "+" ; bg: "#0D6B73"; textColor: "white" ; size: btnSize ; onClicked: calc.pressOperator("+") }

                CalcButton { text: "C" ; bg: "#F25E5E"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.clearAll() }
                CalcButton { text: "0" ; bg: "#CEDFE3"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputDigit(text) }
                CalcButton { text: "." ; bg: "#CEDFE3"; textColor: "#013A3A" ; size: btnSize ; onClicked: calc.inputPoint() }
                Item { width: btnSize; height: btnSize }
                CalcButton {
                    text: "=" ; bg: "#0D6B73"; textColor: "white" ; onPressed: calc.startLongPress() ; onReleased: calc.endLongPress()
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#0A3540"
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
            showSecretMenu = true
        }
    }
}
