#ifndef AMPLITUDES_H
#define AMPLITUDES_H

#include <QObject>
#include <QFile>
#include <qqml.h>

#include <iostream>

#include "AmplitudeReader.h"
#include "MoveAvg.hpp"

#define MOVINGAVG_WINDOWSIZE 3

class Amplitudes : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<double> amplitudes READ amplitudes NOTIFY amplitudeUpdated)
    QML_ELEMENT

public:
    Amplitudes(QObject *parent = nullptr);

    QVector<double> amplitudes();

    Q_INVOKABLE void load(const QUrl& path, uint32_t amplitudeRecordRateMS);

signals:
    void amplitudeUpdated();

private:
    QVector<double> m_qvAmplitude;
    AmplitudeReader m_arAmplitudeReader;
    MovingAverage<double> m_maMovAvg;
};

#endif
