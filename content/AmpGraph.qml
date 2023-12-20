import QtQuick 6.2
import QtCharts
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import LyraWave
import LW.AudioLib

Item {
    id: amplitudeGraph

    signal stoppingCursorSignal()

    property int refreshIntervalMS: 10 // Milliseconds in between each refresh
    property int timespanMS: 1*1000 // Total time shown at any point on the graph
    property double presentOffsetProp: 0.25 // What fraction from the left is the present

    property int cursorStartMS: 0 // Starting location of the cursor
    property int cursorCurrentPosMS: 0 // Cursor's current position
    property int cursorTermMS: 0 // Where the cursor stops moving

    property int sampleIdx: 0

    function play() {
        refreshTimer.start();
    }

    function pause() {
        refreshTimer.stop();
    }

    function reset() {
        setCursor(cursorStartMS);
    }

    function setCursor(posMS) {
        cursorStartMS = posMS
        cursorCurrentPosMS = posMS;
        timeAxis.min = posMS - (timespanMS * presentOffsetProp);
        timeAxis.max = timeAxis.min + timespanMS;
        loadFromCursorStartPos();
    }

    function setCursorTerm(posMS) {
        cursorTermMS = posMS;
    }

    function loadAudioFile(path) {
        timeAxis.min = -timespanMS*presentOffsetProp;
        timeAxis.max = timespanMS*(1-presentOffsetProp);
        amplitudes.load(path, refreshIntervalMS);

        cursorCurrentPosMS = 0;
        cursorStartMS = 0;
        cursorTermMS = amplitudes.amplitudes.length * refreshIntervalMS;
        sampleIdx = 0;

        loadFromCursorStartPos();
    }

    property int nextSample: 0
    property int previousSample: 0

    function loadFromCursorStartPos() {
        amplitudePlot.clear();
        let initialSamples = (timespanMS / refreshIntervalMS);
        let offsetSamples = presentOffsetProp * initialSamples;
        let futureSamples = initialSamples * 2;

        let presentSampleIdx = cursorStartMS / refreshIntervalMS;

        let lower = Math.max(presentSampleIdx - offsetSamples, 0);
        let higher = Math.min(presentSampleIdx + initialSamples, amplitudes.amplitudes.length-1);

        for(let i = lower; i <= higher; ++i) {
            amplitudePlot.append(i*refreshIntervalMS, amplitudes.amplitudes[i]);
        }
        sampleIdx = higher+1;

        nextSample = higher+1;
        previousSample = lower;
    }

    Amplitudes {
        id: amplitudes
    }

    Timer {
        id: refreshTimer
        interval: amplitudeGraph.refreshIntervalMS
        repeat: true
        triggeredOnStart: false
        onTriggered: shiftRight()

        function shiftRight() {
            if(nextSample < amplitudes.amplitudes.length) {
                amplitudePlot.append(nextSample*refreshIntervalMS, amplitudes.amplitudes[nextSample]);
                ++nextSample;
                if(cursorCurrentPosMS > 2 * timespanMS) {
                    amplitudePlot.remove(0);
                }
            }

            if(cursorCurrentPosMS >= cursorTermMS) {
                refreshTimer.stop();
                stoppingCursorSignal();
            }

            cursorCurrentPosMS += amplitudeGraph.refreshIntervalMS;
            timeAxis.min += refreshIntervalMS;
            timeAxis.max += refreshIntervalMS;
        }

        function shiftRight2() {
            if(sampleIdx < amplitudes.amplitudes.length) {
                amplitudePlot.append(sampleIdx*refreshIntervalMS, amplitudes.amplitudes[sampleIdx]);
                sampleIdx++;
            }
            if(cursorCurrentPosMS >= cursorTermMS) {
                refreshTimer.stop();
                stoppingCursorSignal();
            }

            cursorCurrentPosMS += amplitudeGraph.refreshIntervalMS;
            timeAxis.min += refreshIntervalMS;
            timeAxis.max += refreshIntervalMS;
        }
    }

    ChartView {
        anchors.fill: parent

        ValuesAxis {
            id: timeAxis
            min: -timespanMS*presentOffsetProp
            max: timespanMS*(1-presentOffsetProp)
        }

        ValuesAxis {
            id: amplitudeAxis
            min: 0
            max: 2**15
        }


        LineSeries {
            id: amplitudePlot
            axisX: timeAxis
            axisY: amplitudeAxis
        }
    }

    // Component.onCompleted: {
    //     amplitudePlot.clear();

    //     const delaySamples = (delayProp * displayMS) / refreshRateMS;
    //     for(let i = 0; i < delaySamples; ++i)
    //         amplitudePlot.append(i, 0);

    //     amplitudes.loadAmplitudes(ampGraph.refreshRateMS);
    //     // amplitudes.loadMS(0, 1000*60, ampGraph.refreshRateMS);
    //     for(let j = 0; j < amplitudes.amplitudes.length; ++j)
    //         amplitudePlot.append(j+delaySamples, amplitudes.amplitudes[j]);

    //     amplitudeAxis.max = amplitudes.maxSampleValue() * 1.1;

    //     cursorTermMS = (amplitudes.amplitudes.length) * refreshRateMS;
    // }
}
