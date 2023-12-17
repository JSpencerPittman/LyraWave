qt_add_qml_module(AudioLib
       URI LW.AudioLib
       VERSION 1.0
       SOURCES
        Amplitudes.h
        Amplitudes.cpp
        Wav.h
        MoveAvg.hpp
       RESOURCES
        rsc/sample.qrc)

# qt_add_resources(LyraWaveApp rsc/sample.qrc)
