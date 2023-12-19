#include "amplitudereader.h"

AmplitudeReader::AmplitudeReader()
    : m_uiAmplitudeSampleIntervalMS(0) {}

AmplitudeReader::AmplitudeReader(const std::string path, const uint32_t amplitudeSampleIntervalMS)
    : m_sPath(path), m_uiAmplitudeSampleIntervalMS(amplitudeSampleIntervalMS) {}

std::string AmplitudeReader::path() {
    return m_sPath;
}

uint32_t AmplitudeReader::amplitudeRecordRateMS() {
    return m_uiAmplitudeSampleIntervalMS;
}

void AmplitudeReader::setPath(const std::string path) {
    m_sPath = path;
}

void AmplitudeReader::setAmplitudeRecordRateMS(const uint32_t amplitudeSampleIntervalMS) {
    m_uiAmplitudeSampleIntervalMS = amplitudeSampleIntervalMS;
}

std::vector<double> *AmplitudeReader::collectAmplitudes() {
    try {
        // Verify a path has been specified
        if(m_sPath.empty())
            throw std::invalid_argument("Parameter 'path' is missing");
        // Verify the given path exists
        if(!std::filesystem::exists(std::filesystem::path(m_sPath)))
            throw std::invalid_argument("Specified path doesn't exist");

    } catch(const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }


    // Format context is the main structure used in lavf for muxing and demuxing.
    // It exports all of the information about the file being read or written.
    AVFormatContext *formatContext = nullptr;
    // Stream of the media file containing audio
    AVStream *audioStream = nullptr;
    // Array containing collected amplitudes
    std::vector<double> *collectedAmplitudes = nullptr;

    try {
        int resOfOperation; // result of lavf functions

        // Allocate the formatContext
        formatContext = avformat_alloc_context();
        if(formatContext == nullptr)
            throw AVException("format context could not be allocated");

        // Open the media file
        resOfOperation = avformat_open_input(&formatContext, m_sPath.c_str(), NULL, NULL);
        if (resOfOperation < 0)
            throw AVException::decodeErrNum(resOfOperation);

        // Retrieve information about each stream in the media file
        // This allows us to identify which stream is for audio instead of video, subtitles, etc.
        resOfOperation = avformat_find_stream_info(formatContext, NULL);
        if (resOfOperation < 0)
            throw AVException::decodeErrNum(resOfOperation);

        // Identify the audio stream
        int audioStreamIndex = -1;
        for(unsigned int i = 0; i < formatContext->nb_streams; ++i)
            if(formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audioStreamIndex = i;
                break;
            }
        if(audioStreamIndex == -1)
            throw AVException("Media file has no audio stream");

        audioStream = formatContext->streams[audioStreamIndex];

        // The type of decoder to use (mp3, wav)
        AVCodec *codec = avcodec_find_decoder(audioStream->codecpar->codec_id);
        if(codec == nullptr)
            throw AVException("No available decoder (AVCodec)");

        // The codecContext explains how to apply a codec (decoder) to a specific media file
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        if(codecContext == nullptr)
            throw AVException("Could not allocate an AVCodecContext");

        // Fill the codec context with information about the media file
        resOfOperation = avcodec_parameters_to_context(codecContext, audioStream->codecpar);
        if(resOfOperation < 0)
            throw AVException::decodeErrNum(resOfOperation);

        // Initialize the codec context to use the identified codec
        resOfOperation = avcodec_open2(codecContext, codec, NULL);
        if(resOfOperation < 0)
            throw AVException::decodeErrNum(resOfOperation);

        // A stream consists of a flow of packets which is the compressed data
        // For audio data specifically it typically contains multiple compressed frames.
        AVPacket *packet = av_packet_alloc();
        if(packet == nullptr)
            throw AVException("Could not allocate an AVPacket");

        // A frame contains the raw decoded audio data
        AVFrame *frame = av_frame_alloc();
        if(frame == nullptr)
            throw AVException("Could not allocate an AVFrame");

        // Sampling rate (Hz)
        m_uiSampleFreqHz = audioStream->codecpar->sample_rate;
        // How many samples to jump between each recorded amplitude
        //  If the user didn't specify an interval we will assume to collect every amplitude
        uint32_t sampleJump;
        if(m_uiAmplitudeSampleIntervalMS)
            sampleJump = (m_uiSampleFreqHz/1000) * m_uiAmplitudeSampleIntervalMS;
        else
            sampleJump = 1;
        // Current frame in the interval [0, sampleJump]
        uint32_t cyclicCounter = 0;
        m_uiNBSamples = 0;

        collectedAmplitudes->reserve(estimateNBCollectedAmplitudes(
            audioStream->duration,
            audioStream->time_base,
            sampleJump,
            m_uiSampleFreqHz));

        // This actually reads packets instead of singular frames
        // While packets are available load them into packet
        while(av_read_frame(formatContext, packet) == 0) {
            // Verify the packets from the audio stream
            if(packet->stream_index == audioStreamIndex) {
                // Send the raw packet data to the decoder
                resOfOperation = avcodec_send_packet(codecContext, packet);
                if(resOfOperation != 0 && resOfOperation != AVERROR_EOF)
                    throw AVException::decodeErrNum(resOfOperation);

                // Return decoded output from the decoder to the frame
                resOfOperation = avcodec_receive_frame(codecContext, frame);
                while(resOfOperation == 0) {
                    double *samples = (double*)frame->data[0];
                    for(int i = 0; i < frame->nb_samples; ++i) {
                        ++m_uiNBSamples;
                        if(cyclicCounter % sampleJump == 0) {
                            // record amplitude
                            collectedAmplitudes->push_back(samples[i]);
                            cyclicCounter = 0;
                        }
                        ++cyclicCounter;
                    }

                    resOfOperation = avcodec_receive_frame(codecContext, frame);
                }
            }

            // Wipe the packet clear
            av_packet_unref(packet);
        }


        // Release the packet and frame structs
        av_packet_free(&packet);
        av_frame_free(&frame);

        // Release the codec context
        avcodec_free_context(&codecContext);


        // Release the format context
        avformat_close_input(&formatContext);

    } catch(const AVException& e) {
        std::cout << "AVException: " << e.what() << std::endl;
        return nullptr;
    }

    return collectedAmplitudes;

}

int64_t estimateNBCollectedAmplitudes(int64_t duration, AVRational timeBase,
                                             int64_t sampleJump, int64_t sampleFreqHZ) {
    int64_t estimate = duration;
    estimate *= (int64_t)timeBase.num;
    estimate /= (int64_t)timeBase.den;
    estimate *= sampleFreqHZ;
    estimate /= sampleJump;
    return estimate;
}

AmplitudeReader::AVException::AVException(const std::string &message)
    : m_sMessage(message) {}

const char* AmplitudeReader::AVException::what() const noexcept {
    return m_sMessage.c_str();
}

AmplitudeReader::AVException AmplitudeReader::AVException::decodeErrNum(int errnum) {
    char* errbuf = new char[AV_ERROR_MAX_STRING_SIZE];
    int res = av_strerror(errnum, errbuf, AV_ERROR_MAX_STRING_SIZE);
    if(res == 0) {
        std::string errMessage(errbuf);
        delete [] errbuf;
        return AVException(errMessage);
    } else {
        delete [] errbuf;
        return AVException(std::to_string(errnum));
    }
}
