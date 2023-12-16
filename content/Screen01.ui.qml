/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtCharts
import LyraWave
import LW.AudioLib

Rectangle {
    id: rectangle
    width: 1000
    height: 500

    Amplitudes {
        id: amps
    }

    ChartView {
        anchors.fill: parent
        LineSeries {
            id: lineSeries
        }
    }

    function updateSeries() {

    }

    // ChartView {
    //     title: "Line"
    //     anchors.fill: parent
    //     antialiasing: true

    //     LineSeries {
    //         name: "LineSeries"
    //         XYPoint { x: 0; y: 0 }
    //         XYPoint { x: 1.1; y: 2.1 }
    //         XYPoint { x: 1.9; y: 3.3 }
    //         XYPoint { x: 2.1; y: 2.1 }
    //         XYPoint { x: 2.9; y: 4.9 }
    //         XYPoint { x: 3.4; y: 3.0 }
    //         XYPoint { x: 4.1; y: 3.3 }
    //     }
    // }
}
