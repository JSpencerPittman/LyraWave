// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import LyraWave

Window {
    // width: mainScreen.width
    // height: mainScreen.height
    width: 1500
    height: 600

    visible: true
    title: "LyraWave"

    AmpGraph {
        id: ampgraph
    }

    // Screen01 {
    //     id: mainScreen
    // }

}

