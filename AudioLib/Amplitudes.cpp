#include "Amplitudes.h"

#include <iostream>

Amplitudes::Amplitudes(QObject *parent) : QObject(parent) {
    QFile file(":/Audio/tropicLove.wav");
    file.open(QIODevice::ReadOnly);
    QByteArray dataByteArray = file.readAll();
    std::vector<uint8_t> rawByteVec(dataByteArray.begin(), dataByteArray.end());
    m_audioFile.loadFromMemory(rawByteVec);

    m_sampleRate = m_audioFile.getSampleRate();
    m_numSamples = m_audioFile.getNumSamplesPerChannel();

    m_shiftMoveAvg.setCapacity(MOVINGAVG_WINDOWSIZE);
}

QVector<double> Amplitudes::y() {
    return m_yVec;
}

void Amplitudes::loadMS(int startMS, int endMS, int refreshRateMS) {
    int startPos = (m_sampleRate / 1000) * startMS;
    int endPos = ((m_sampleRate / 1000) * endMS) - 1;

    std::cout << "SP: " << startPos << std::endl;
    std::cout << "EP: " << endPos << std::endl;
    std::cout << "SR: " << m_sampleRate << std::endl;
    std::cout << "NP: " << m_numSamples << std::endl;

    if(startMS >= endMS) return;
    if(startMS < 0) return;
    if(endPos >= m_numSamples) return;

    int jump = (m_sampleRate/1000) * refreshRateMS;
    int samples = ceil((endMS - startMS) / refreshRateMS);

    // std::cout << "DU: " << duration << std::endl;
    std::cout << "JP: " << jump << std::endl;

    m_yVec.clear();
    m_yVec.reserve(samples);
    int idx = startPos;
    while(idx <= endPos) {
        m_yVec.push_back(abs(m_audioFile.samples[0][idx]));
        idx += jump;
    }

    std::cout << "SZ: " << m_yVec.size() << std::endl;

    emit yUpdated();
}

void Amplitudes::initShift(int startMS, int refreshRateMS, int timeSpanMS) {
    m_shiftStartPos = (m_sampleRate / 1000) * startMS;
    m_shiftJump = (m_sampleRate / 1000) * refreshRateMS;
    m_tickCnt = timeSpanMS / refreshRateMS;

    m_shiftMoveAvg.clear();
    m_shiftCount = 0;

    m_yVec.clear();
    while(m_shiftCount < m_tickCnt) {
        int idx = m_shiftStartPos + (m_shiftCount * m_shiftJump);
        double val = m_audioFile.samples[0][idx];
        m_shiftMoveAvg.add(abs(val));
        m_yVec.push_back(m_shiftMoveAvg.averageDouble());
        ++m_shiftCount;
    }

    emit yUpdated();
}

void Amplitudes::shift() {
    m_yVec.pop_front();

    int idx = m_shiftStartPos + (m_shiftCount * m_shiftJump);
    double val = m_audioFile.samples[0][idx];

    m_shiftMoveAvg.add(abs(val));
    m_yVec.push_back(m_shiftMoveAvg.averageDouble());
    ++m_shiftCount;

    emit yUpdated();
}

QVector<double> Amplitudes::loadAmplitudes(int startMS, int endMS, int refreshRateMS) {
    int startPos = (m_sampleRate / 1000) * startMS;
    int endPos = ((m_sampleRate / 1000) * endMS) + startPos - 1;

    int jump = endPos - startPos - 1 / TICKCOUNT;

    m_yVec.clear();
    for(int i = 0; i < TICKCOUNT-1; i++) {
        int idx = startPos + (i * jump);
        m_yVec.push_back(abs(m_audioFile.samples[0][idx]));
    }
    m_yVec.push_back(abs(m_audioFile.samples[0][endPos]));

    emit yUpdated();
}
