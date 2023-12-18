import QtQuick 6.2
import QtCharts
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import LyraWave
import LW.AudioLib

Item {
    id: ampGraph

    signal stoppingCursorSignal()

    property int refreshRateMS: 10 // How many milliseconds before refreshing the graph
    property int displayMS: 10*1000 // How many milliseconds to show at any given time
    property double delayProp: 0.25

    property int cursorStartMS: 0
    property int cursorTermMS: 120*1000
    property int cursorPosMS: 0


    function play() {
        intervalTimer.start();
    }

    function pause() {
        intervalTimer.stop();
    }

    function reset() {
        setCursor(cursorStartMS);
    }

    function setCursor(posMS) {
        cursorStartMS = posMS
        cursorPosMS = posMS;
        timeAxis.min = posMS / refreshRateMS;
        timeAxis.max = timeAxis.min + (displayMS / refreshRateMS);
    }

    function setCursorTerm(posMS) {
        cursorTermMS = posMS;
    }

    Amplitudes {
        id: amplitudes
    }

    ChartView {
        anchors.fill: parent

        ValuesAxis {
            id: timeAxis
            min: 0
            max: displayMS / refreshRateMS
        }

        ValuesAxis {
            id: amplitudeAxis
            min: 0
            max: 1
        }


        LineSeries {
            id: amplitudePlot
            axisX: timeAxis
            axisY: amplitudeAxis
        }
    }

    Timer {
        id: intervalTimer
        interval: ampGraph.refreshRateMS
        repeat: true
        triggeredOnStart: false
        property int cnt: 0
        onTriggered: shiftRight()

        function shiftRight() {
            if(cursorPosMS >= cursorTermMS) {
                intervalTimer.stop();
                stoppingCursorSignal();
            }
            ++timeAxis.min;
            ++timeAxis.max;
            cursorPosMS += refreshRateMS;
        }
    }

    Component.onCompleted: {
        amplitudePlot.clear();

        const delaySamples = (delayProp * displayMS) / refreshRateMS;
        for(let i = 0; i < delaySamples; ++i)
            amplitudePlot.append(i, 0);

        amplitudes.loadAmplitudes(ampGraph.refreshRateMS);
        // amplitudes.loadMS(0, 1000*60, ampGraph.refreshRateMS);
        for(let j = 0; j < amplitudes.amplitudes.length; ++j)
            amplitudePlot.append(j+delaySamples, amplitudes.amplitudes[j]);

        amplitudeAxis.max = amplitudes.maxSampleValue() * 1.1;

        cursorTermMS = (amplitudes.amplitudes.length) * refreshRateMS;
    }
}
