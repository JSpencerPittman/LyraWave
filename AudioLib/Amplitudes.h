#ifndef AMPLITUDES_H
#define AMPLITUDES_H

#include <QObject>
#include <QFile>
#include <qqml.h>

#include "Wav.h"
#include "MoveAvg.hpp"

#define TICKCOUNT 10
#define MOVINGAVG_WINDOWSIZE 3

class Amplitudes : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<double> amplitudes READ amplitudes NOTIFY amplitudeUpdated)
    QML_ELEMENT

public:
    Amplitudes(QObject *parent = nullptr);

    QVector<double> amplitudes();

    Q_INVOKABLE void loadAudioFile(const QString& path);
    Q_INVOKABLE void loadAmplitudes(int refreshRateMS);

    Q_INVOKABLE int sampleRate();
    Q_INVOKABLE int numSamples();
    Q_INVOKABLE double maxSampleValue();

signals:
    void amplitudeUpdated();

private:
    QVector<double> m_qvAmplitude;
    AudioFile<double> m_afAudioFile;
    MovingAverage<double> m_maMovAvg;
    int m_nSampleRate;
    int m_nNumSamples;
    double m_dMaxSample;
};

#endif
