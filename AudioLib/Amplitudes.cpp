#include "Amplitudes.h"

Amplitudes::Amplitudes(QObject *parent) : QObject(parent) {
    for(int i = 0; i < 10; i++) {
        m_yVec.push_back((double)i);
    }
    m_yVec[3] = -1.0;

    m_audioFile.load("StarWars3.wav");
}

QVector<double> Amplitudes::y() {
    return m_yVec;
}

void Amplitudes::increment() {
    for(int i = 0; i < m_yVec.size(); i++)
        m_yVec[i]++;
    emit yUpdated();
}
