#include "Amplitudes.h"

Amplitudes::Amplitudes(QObject *parent) : QObject(parent) {
    QFile file(":/Audio/StarWars3.wav");
    file.open(QIODevice::ReadOnly);
    QByteArray dataByteArray = file.readAll();
    std::vector<uint8_t> rawByteVec(dataByteArray.begin(), dataByteArray.end());
    m_audioFile.loadFromMemory(rawByteVec);

    m_sampleRate = m_audioFile.getSampleRate();
    m_numSamples = m_audioFile.getNumSamplesPerChannel();
}

QVector<double> Amplitudes::y() {
    return m_yVec;
}

void Amplitudes::loadMS(int startMS, int endMS) {
    int startPos = (m_sampleRate / 1000) * startMS;
    int endPos = ((m_sampleRate / 1000) * endMS) - 1;

    if(startMS >= endMS) return;
    if(startMS < 0) return;
    if(endPos >= m_numSamples) return;

    int jump = endPos - startPos - 1 / TICKCOUNT;

    m_yVec.clear();
    for(int i = 0; i < TICKCOUNT-1; i++) {
        int idx = startPos + (i * jump);
        m_yVec.push_back(m_audioFile.samples[0][idx]);
    }
    m_yVec.push_back(m_audioFile.samples[0][endPos]);

    emit yUpdated();
}
