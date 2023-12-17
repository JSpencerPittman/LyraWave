import QtQuick 6.2
import QtCharts
import QtQuick.Controls
import LyraWave
import LW.AudioLib

Item {
    anchors.fill: parent

    Amplitudes {
        id: amplitudeData
        onYUpdated: updateSeries()
    }

    ChartView {
        anchors.fill: parent
        // antialiasing: true

        ValuesAxis {
            id: valueAxisX
            min: 0
            max: 30
        }

        ValuesAxis {
            id: valueAxisY
            min: -1
            max: 1
        }


        LineSeries {
            id: lineSeries
            axisX: valueAxisX
            axisY: valueAxisY
        }
    }

    Button {
        text: "shift"
        id: shiftButton
        property int count: 0
        onClicked: shift()
    }

    function shift() {
        let start = (shiftButton.count * 100);
        amplitudeData.loadMS(start, start+100);
        shiftButton.count++;
    }

    function updateSeries() {
        lineSeries.clear();
        for(let i = 0; i < amplitudeData.y.length; ++i) {
            lineSeries.append(i, amplitudeData.y[i]);
        }
    }

    Component.onCompleted: updateSeries()
}
