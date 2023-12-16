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
            max: 15
        }

        ValuesAxis {
            id: valueAxisY
            min: 0
            max: 15
        }


        LineSeries {
            id: lineSeries
            axisX: valueAxisX
            axisY: valueAxisY
        }
    }

    Button {
        text: "increment"
        onClicked: amplitudeData.increment()
    }

    function updateSeries() {
        lineSeries.clear();
        for(let i = 0; i < amplitudeData.y.length; ++i) {
            lineSeries.append(i, amplitudeData.y[i]);
        }
    }

    Component.onCompleted: updateSeries()
}
