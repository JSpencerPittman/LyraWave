# FFMPEG support
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWRESAMPLE REQUIRED libswresample)
pkg_check_modules(AVFILTER REQUIRED libavfilter)
pkg_check_modules(SWSCALE REQUIRED libswscale)

qt_add_qml_module(AudioLib
       URI LW.AudioLib
       VERSION 1.0
       SOURCES
        Amplitudes.h
        Amplitudes.cpp
        MoveAvg.hpp
        AmplitudeReader.h
        AmplitudeReader.cpp
    )

target_link_libraries(AudioLib PRIVATE
    # FFMPEG Support
    ${AVCODEC_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWRESAMPLE_LIBRARIES}
    ${AVFILTER_LIBRARIES}
    ${SWSCALE_LIBRARIES}
)

# FFMPEG support
target_include_directories(AudioLib PRIVATE
    ${AVCODEC_INCLUDE_DIRS}
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${SWRESAMPLE_INCLUDE_DIRS}
    ${AVFILTER_INCLUDE_DIRS}
    ${SWSCALE_INCLUDE_DIRS}
)

# qt_add_resources(LyraWaveApp rsc/sample.qrc)
