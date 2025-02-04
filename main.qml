import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.3

Window {
    id: root
    visible: true
    width: 800
    height: 600
    title: qsTr("Wordle Clone")
    color: "#121213"
    visibility: Qt.WindowFullScreen

    Text {
        id: headtext
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 60
        font.pixelSize: 40
        text: "WORDLE CLONE"
        color: "white"
        font.family: "Calibri"
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10


        Item {
            id: boardContainer
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                id: boardLayout
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10
                focus: true

                Keys.onPressed: {
                    // Checking whether inputhandler is defined
                    if (inputHandler)
                        inputHandler.handleKeyPress(event.key, event.text)
                }

                // Game board: 6 lines
                Repeater {
                    model: 6
                    RowLayout {
                        property int rowIndex: index
                        spacing: 7

                        Repeater {
                            model: 5
                            Flipable {
                                id: tileFlipable
                                width: 100
                                height: 100

                                //  First we check whether inputHandler is defined or not, then flip control
                                property bool flipped: (inputHandler && inputHandler.guesses) ?
                                                       (rowIndex < inputHandler.guesses.length) : false
                                rotation: flipped ? 180 : 0
                                transform: Rotation {
                                    origin.x: tileFlipable.width / 2
                                    origin.y: tileFlipable.height / 2
                                    axis.x: 0; axis.y: 1; axis.z: 0
                                    angle: tileFlipable.rotation
                                }
                                Behavior on rotation {
                                    NumberAnimation { duration: 600; easing.type: Easing.InOutQuad }
                                }
                                front: Rectangle {
                                    anchors.fill: parent
                                    color: {
                                        if (inputHandler && typeof inputHandler.currentRow === "number") {
                                            return (rowIndex === inputHandler.currentRow) ?
                                                   inputHandler.currentGuessColors[index] : "#272729";
                                        }
                                        return "#272729";
                                    }
                                    border.color: "#818384"
                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: 24
                                        text: {
                                            if (inputHandler && typeof inputHandler.currentRow === "number") {
                                                return (rowIndex === inputHandler.currentRow) ?
                                                       (inputHandler.inputText.length > index ? inputHandler.inputText[index] : "") : "";
                                            }
                                            return "";
                                        }
                                        color: "white"
                                    }
                                }
                                back: Rectangle {
                                    anchors.fill: parent
                                    color: {
                                        if (inputHandler && inputHandler.guesses && inputHandler.guesses.length > rowIndex) {
                                            return inputHandler.guesses[rowIndex].colors[index];
                                        }
                                        return "#272729";
                                    }
                                    border.color: "#818384"
                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: 24
                                        text: {
                                            if (inputHandler && inputHandler.guesses && inputHandler.guesses.length > rowIndex) {
                                                return inputHandler.guesses[rowIndex].text[index];
                                            }
                                            return "";
                                        }
                                        color: "white"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: gameOverOverlay
        anchors.fill: parent
        visible: (inputHandler && inputHandler.gameOver) ? inputHandler.gameOver : false
        z: 10
        color: "#00000088"
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            bottomPadding: 30
            text: {
                if (inputHandler) {
                    if (inputHandler.gameResult === "win")
                        return "YOU WON!\n The correct answer was: " + inputHandler.solution;
                    else if (inputHandler.gameResult === "lose")
                        return "Game Over! YOU LOST!\n The correct answer was: " + inputHandler.solution;
                }
                return "";
            }
            font.pixelSize: 32
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }
    }

    // Error Overlay
    Rectangle {
        id: errorRectangle
        width: 400
        height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        z: 10
        color: "white"
        visible: (errorMessage.text !== "")
        Text {
            id: errorMessage
            text: ""
            anchors.fill: parent
            visible: (text !== "")
            font.pixelSize: 32
            color: "black"
            padding: 30
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }
    }

    Connections {
        target: inputHandler
        onGuessesChanged: {
            console.log("Guesses updated, count:" + inputHandler.guesses.length)
        }
        onGameOverChanged: {
            console.log("Game Over! Result:" + inputHandler.gameResult);
        }
        onErrorMessageChanged: {
            errorMessage.text = message;
            errorRectangle.visible = true;
            hideTimer.restart();
        }
    }

    Timer {
        id: hideTimer
        interval: 2000
        onTriggered: {
            errorMessage.text = "";
            errorRectangle.visible = false;
        }
    }
}
