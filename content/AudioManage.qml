import QtQuick 6.2
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs
import QtCore
import LyraWave

Item {
    id: audioManage
    anchors.fill: parent

    property int cursorStartMS: 0
    property int cursorTermMS: 60*1000

    property string audioFilePath: "qrc:/Audio/tropicLove.mp3"

    MediaPlayer {
        id: audioPlayer
        source: audioFilePath
        audioOutput: AudioOutput {}
    }

    RowLayout {
        anchors.fill: parent

        AmpGraph {
            id: ampgraph
            Layout.fillHeight: true
            Layout.fillWidth: true
            onStoppingCursorSignal: audioPlayer.pause()
        }

        ColumnLayout {
            Button {
                text: "play"
                onClicked: {
                    ampgraph.play();
                    audioPlayer.play();
                }
            }

            Button {
                text: "pause"
                onClicked: {
                    ampgraph.pause();
                    audioPlayer.pause();
                }
            }

            Button {
                text: "reset"
                onClicked: {
                    ampgraph.reset();
                    audioPlayer.setPosition(cursorStartMS);
                }
            }

            Button {
                text: "load"
            }
        }

        ColumnLayout {
            Button {
                text: "select"
                onClicked: {
                    ampgraph.pause();
                    audioPlayer.pause();
                    ampgraph.setCursor(audioManage.cursorStartMS)
                    ampgraph.setCursorTerm(audioManage.cursorTermMS)
                    audioPlayer.setPosition(cursorStartMS)
                }
            }
            TextField {
                id: playStartTextField
                inputMethodHints: Qt.ImhDigitsOnly
                onTextChanged: audioManage.cursorStartMS = playStartTextField.text
            }
            TextField {
                id: playEndTextField
                inputMethodHints: Qt.ImhDigitsOnly
                onTextChanged: audioManage.cursorTermMS = playEndTextField.text
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an audio file (wav/mp3)"
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            audioManage.audioFilePath = selectedFile
        }
    }
}
