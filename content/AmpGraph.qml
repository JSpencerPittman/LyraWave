import QtQuick 6.2
import QtCharts
import QtQuick.Controls
import QtMultimedia
import LyraWave
import LW.AudioLib

Item {
    id: ampGraph
    anchors.fill: parent
    property int refreshRateMS: 10
    property int displayMS: 10*1000
    // property int tickCnt: 20
    // property int timeSpanMS: 1000

    Amplitudes {
        id: amplitudeData
    }

    MediaPlayer {
        id: audioPlayer
        source: "qrc:/Audio/tropicLove.wav"
        audioOutput: AudioOutput {}
    }

    ChartView {
        anchors.fill: parent

        ValuesAxis {
            id: valueAxisX
            min: 0
            max: displayMS / refreshRateMS
        }

        ValuesAxis {
            id: valueAxisY
            min: 0
            max: 1
        }


        LineSeries {
            id: lineSeries
            axisX: valueAxisX
            axisY: valueAxisY
        }
    }

    Timer {
        id: cntTimer
        interval: ampGraph.refreshRateMS
        repeat: true
        triggeredOnStart: false
        property int cnt: 0
        onTriggered: shiftRight()
    }

    Button {
        text: "start"
        onClicked: {
            audioPlayer.play()
            cntTimer.start()
        }
    }

    function shiftRight() {
        ++valueAxisX.min;
        ++valueAxisX.max;
    }

    Component.onCompleted: {
        lineSeries.clear();
        amplitudeData.loadMS(0, 1000*60, 10);
        console.log(amplitudeData.y.length);
        for(let i = 0; i < amplitudeData.y.length; ++i)
            lineSeries.append(i, amplitudeData.y[i]);
    }
}
