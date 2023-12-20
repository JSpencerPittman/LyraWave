#ifndef AMPLITUDE_READER_H
#define AMPLITUDE_READER_H

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    #include <libavutil/rational.h>
}

#include <exception>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <vector>

class AmplitudeReader
{
public:
    AmplitudeReader();
    AmplitudeReader(const std::string path, const uint32_t amplitudeRecordRateMS);


    std::string path();
    uint32_t amplitudeRecordRateMS();

    void setPath(const std::string path);
    void setAmplitudeRecordRateMS(const uint32_t amplitudeRecordRateMS);

    std::vector<int16_t> *collectAmplitudes();


    class AVException : public std::exception {
    public:
        AVException(const std::string &message);

        const char* what() const noexcept override;

        static AVException decodeErrNum(int errnum);

    private:
        std::string m_sMessage;
    };

private:
    std::string m_sPath;
    uint32_t m_uiAmplitudeSampleIntervalMS;

    uint32_t m_uiSampleFreqHz;
    uint32_t m_uiNBSamples;
};

#endif
