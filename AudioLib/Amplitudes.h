#ifndef AMPLITUDES_H
#define AMPLITUDES_H

#include <QObject>
#include <QFile>
#include <qqml.h>

#include "Wav.h"

#define TICKCOUNT 30

class Amplitudes : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<double> y READ y NOTIFY yUpdated)
    QML_ELEMENT

public:
    Amplitudes(QObject *parent = nullptr);

    QVector<double> y();

    Q_INVOKABLE void loadMS(int startMS, int endMS);

signals:
    void yUpdated();

private:
    QVector<double> m_yVec;
    AudioFile<double> m_audioFile;
    int m_sampleRate;
    int m_numSamples;
};

#endif
