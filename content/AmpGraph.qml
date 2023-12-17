import QtQuick 6.2
import QtCharts
import QtQuick.Controls
import QtMultimedia
import LyraWave
import LW.AudioLib

Item {
    id: ampGraph
    anchors.fill: parent
    property int refreshRateMS: 30
    property int tickCnt: 20
    property int timeSpanMS: 1000

    Amplitudes {
        id: amplitudeData
        onYUpdated: updateSeries()
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
            max: ampGraph.tickCnt - 1
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
        onTriggered: amplitudeData.shift();
        // property int rounds: 0
    }

    Button {
        text: "start"
        onClicked: {
            audioPlayer.play()
            cntTimer.start()
        }
    }

    function updateSeries() {
        lineSeries.clear();
        let jump = parseInt(ampGraph.timeSpanMS / (ampGraph.refreshRateMS * ampGraph.tickCnt));
        for(let i = 0; i < ampGraph.tickCnt; ++i)
               lineSeries.append(i, amplitudeData.y[i*jump]);
    }

    Component.onCompleted: {
        amplitudeData.initShift(0, ampGraph.refreshRateMS, ampGraph.timeSpanMS)
        updateSeries();
    }
}
