import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: app
    visible: true
    width: 492
    height: 865
    title: "Calculator"
    color: "#024873"

    FontLoader {
        id: openSans
        source: "qrc:/fonts/fonts/OpenSans-SemiBold.ttf"
    }
    font.family: openSans.name

    property bool showSecretMenu: false
    property bool openBracket: true

    // Относительные размеры, вычисляемые на основе размеров окна
    property real relativeWidth: width / 358
    property real relativeHeight: height / 634

    property real fontFactor: height / 865

    function relX(value) { return value * relativeWidth; }
    function relY(value) { return value * relativeHeight; }

    Item {
        anchors.fill: parent
        anchors.margins: 0

        Rectangle {
            id: statusBar
            color: "#04BFAD"
            width: parent.width
            height: relY(26)
            z: 1

            Image {
                id: icon
                anchors.centerIn: parent
                source: "qrc:/img/img/icon.png"
                width: parent.width
                height: parent.height
            }
        }

        Rectangle {
            id: topBlock
            width: parent.width
            height: relY(153)
            color: "#04BFAD"
            radius: parent.width * 0.05
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: parent.height * 0.05 - statusBar.height * 0.7
            z: 0

            Text {
                id: expressionText
                text: calc.expression
                font.pixelSize: 20 * fontFactor
                font.letterSpacing: 0.5 * fontFactor
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideRight
                anchors {
                    top: parent.top
                    topMargin: relY(46)
                    left: parent.left
                    right: parent.right
                    rightMargin: relX(45)
                }
            }

            Text {
                id: resultText
                text: calc.result
                font.pixelSize: 50 * fontFactor
                font.letterSpacing: 0.5 * fontFactor
                font.bold: true
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideRight
                anchors {
                    top: expressionText.bottom
                    topMargin: relY(26)
                    bottom: parent.bottom
                    bottomMargin: relY(26)
                    left: parent.left
                    right: parent.right
                    rightMargin: relX(45)
                }
            }
        }

        Rectangle {
            id: bottomBlock
            anchors.top: topBlock.bottom
            anchors.topMargin: relY(25)
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: relY(40)
            color: "#024873"

            Grid {
                id: keyGrid
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: relX(23)
                    rightMargin: relX(23)
                }
                columns: 4
                spacing: relX(20)

                property real btnSize: Math.min(
                    (width - (spacing * (columns - 1))) / columns,
                    (height - (spacing * (rows - 1))) / rows
                )
                property int rows: 5

                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "()"
                    onClicked: {
                        if (openBracket) calc.inputParenthesis("(");
                        else calc.inputParenthesis(")");
                        openBracket = !openBracket;
                    }
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "+/-"
                    onClicked: calc.toggleSign()
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "%"
                    onClicked: calc.pressPercent()
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "÷"
                    onClicked: calc.pressOperator("/")
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }

                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "7"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "8"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "9"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "×"
                    onClicked: calc.pressOperator("*")
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }

                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "4"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "5"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "6"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "−"
                    onClicked: calc.pressOperator("-")
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }

                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "1"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "2"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "3"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "+"
                    onClicked: calc.pressOperator("+")
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }

                ClearButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "C"
                    onClicked: calc.clearAll()
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "0"
                    onClicked: calc.inputDigit(text)
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                DigitButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "."
                    onClicked: calc.inputDot()
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
                FuncButton {
                    width: keyGrid.btnSize
                    height: keyGrid.btnSize
                    text: "="
                    onPressed: calc.startLongPress()
                    onReleased: calc.endLongPress()
                    fontFamily: openSans.name
                    fontFactor: app.fontFactor
                }
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
            Text {
                text: "Секретное меню"
                font.pixelSize: 24
                color: "white"
            }
            Button {
                text: "Назад"
                onClicked: showSecretMenu = false
            }
        }
    }

    Connections {
        target: calc
        function onShowSecretMenu() {
            app.showSecretMenu = true
        }
    }
}
