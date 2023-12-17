#ifndef AMPLITUDES_H
#define AMPLITUDES_H

#include <QObject>
#include <QFile>
#include <qqml.h>

#include "Wav.h"
#include "MoveAvg.hpp"

#define TICKCOUNT 10
#define MOVINGAVG_WINDOWSIZE 4

class Amplitudes : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<double> y READ y NOTIFY yUpdated)
    QML_ELEMENT

public:
    Amplitudes(QObject *parent = nullptr);

    QVector<double> y();

    Q_INVOKABLE void loadMS(int startMS, int endMS, int refreshRateMS);

    Q_INVOKABLE void initShift(int startMS, int refreshRateMS, int timeSpanMS);
    Q_INVOKABLE void shift();

    Q_INVOKABLE QVector<double> loadAmplitudes(int startMS, int endMS, int refreshRateMS);

signals:
    void yUpdated();

private:
    QVector<double> m_yVec;
    AudioFile<double> m_audioFile;
    int m_sampleRate;
    int m_numSamples;

    MovingAverage<double> m_shiftMoveAvg;
    int m_shiftStartPos;
    int m_tickCnt;
    int m_shiftJump;
    int m_shiftCount;
};

#endif
