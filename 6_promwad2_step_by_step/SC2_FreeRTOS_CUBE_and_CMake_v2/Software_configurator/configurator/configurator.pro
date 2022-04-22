QT += quick multimedia sql testlib network core-private gui-private widgets multimedia serialport
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


 # ~/Qt/5.12.5/gcc_64/bin/lupdate assignment_list.qml firmware.qml groups.qml network.qml main.qml main_menu.qml subscriber.qml subscriber_navigation.qml subscriber_window.qml update_rs232.qml assignment_list_firmware.qml assignment_list_rs232.qml verification.qml programmer_window.qml status_window.qml change_window.qml -ts translator/main_ru.ts

TRANSLATIONS += translator/main_ru.ts

SOURCES += \
#        Lib/BasicUsageEnvironment/BasicHashTable.cpp \
#        Lib/BasicUsageEnvironment/BasicTaskScheduler.cpp \
#        Lib/BasicUsageEnvironment/BasicTaskScheduler0.cpp \
#        Lib/BasicUsageEnvironment/BasicUsageEnvironment.cpp \
#        Lib/BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
#        Lib/BasicUsageEnvironment/DelayQueue.cpp \
#        Lib/SPEEXX/speexx.cpp \
#        Lib/UsageEnvironment/HashTable.cpp \
#        Lib/UsageEnvironment/UsageEnvironment.cpp \
#        Lib/UsageEnvironment/strDup.cpp \
#        Lib/groupsock/GroupEId.cpp \
#        Lib/groupsock/Groupsock.cpp \
#        Lib/groupsock/GroupsockHelper.cpp \
#        Lib/groupsock/IOHandlers.cpp \
#        Lib/groupsock/NetAddress.cpp \
#        Lib/groupsock/NetInterface.cpp \
#        Lib/groupsock/inet.c \
#        Lib/liveMedia/AC3AudioFileServerMediaSubsession.cpp \
#        Lib/liveMedia/AC3AudioRTPSink.cpp \
#        Lib/liveMedia/AC3AudioRTPSource.cpp \
#        Lib/liveMedia/AC3AudioStreamFramer.cpp \
#        Lib/liveMedia/ADTSAudioFileServerMediaSubsession.cpp \
#        Lib/liveMedia/ADTSAudioFileSource.cpp \
#        Lib/liveMedia/AMRAudioFileServerMediaSubsession.cpp \
#        Lib/liveMedia/AMRAudioFileSink.cpp \
#        Lib/liveMedia/AMRAudioFileSource.cpp \
#        Lib/liveMedia/AMRAudioRTPSink.cpp \
#        Lib/liveMedia/AMRAudioRTPSource.cpp \
#        Lib/liveMedia/AMRAudioSource.cpp \
#        Lib/liveMedia/AVIFileSink.cpp \
#        Lib/liveMedia/AudioInputDevice.cpp \
#        Lib/liveMedia/AudioRTPSink.cpp \
#        Lib/liveMedia/Base64.cpp \
#        Lib/liveMedia/BasicUDPSink.cpp \
#        Lib/liveMedia/BasicUDPSource.cpp \
#        Lib/liveMedia/BitVector.cpp \
#        Lib/liveMedia/ByteStreamFileSource.cpp \
#        Lib/liveMedia/ByteStreamMemoryBufferSource.cpp \
#        Lib/liveMedia/ByteStreamMultiFileSource.cpp \
#        Lib/liveMedia/DVVideoFileServerMediaSubsession.cpp \
#        Lib/liveMedia/DVVideoRTPSink.cpp \
#        Lib/liveMedia/DVVideoRTPSource.cpp \
#        Lib/liveMedia/DVVideoStreamFramer.cpp \
#        Lib/liveMedia/DeviceSource.cpp \
#        Lib/liveMedia/DigestAuthentication.cpp \
#        Lib/liveMedia/EBMLNumber.cpp \
#        Lib/liveMedia/FileServerMediaSubsession.cpp \
#        Lib/liveMedia/FileSink.cpp \
#        Lib/liveMedia/FramedFileSource.cpp \
#        Lib/liveMedia/FramedFilter.cpp \
#        Lib/liveMedia/FramedSource.cpp \
#        Lib/liveMedia/GSMAudioRTPSink.cpp \
#        Lib/liveMedia/GenericMediaServer.cpp \
#        Lib/liveMedia/H261VideoRTPSource.cpp \
#        Lib/liveMedia/H263plusVideoFileServerMediaSubsession.cpp \
#        Lib/liveMedia/H263plusVideoRTPSink.cpp \
#        Lib/liveMedia/H263plusVideoRTPSource.cpp \
#        Lib/liveMedia/H263plusVideoStreamFramer.cpp \
#        Lib/liveMedia/H263plusVideoStreamParser.cpp \
#        Lib/liveMedia/H264VideoFileServerMediaSubsession.cpp \
#        Lib/liveMedia/H264VideoFileSink.cpp \
#        Lib/liveMedia/H264VideoRTPSink.cpp \
#        Lib/liveMedia/H264VideoRTPSource.cpp \
#        Lib/liveMedia/H264VideoStreamDiscreteFramer.cpp \
#        Lib/liveMedia/H264VideoStreamFramer.cpp \
#        Lib/liveMedia/H264or5VideoFileSink.cpp \
#        Lib/liveMedia/H264or5VideoRTPSink.cpp \
#        Lib/liveMedia/H264or5VideoStreamDiscreteFramer.cpp \
#        Lib/liveMedia/H264or5VideoStreamFramer.cpp \
#        Lib/liveMedia/H265VideoFileServerMediaSubsession.cpp \
#        Lib/liveMedia/H265VideoFileSink.cpp \
#        Lib/liveMedia/H265VideoRTPSink.cpp \
#        Lib/liveMedia/H265VideoRTPSource.cpp \
#        Lib/liveMedia/H265VideoStreamDiscreteFramer.cpp \
#        Lib/liveMedia/H265VideoStreamFramer.cpp \
#        Lib/liveMedia/InputFile.cpp \
#        Lib/liveMedia/JPEG2000VideoRTPSink.cpp \
#        Lib/liveMedia/JPEG2000VideoRTPSource.cpp \
#        Lib/liveMedia/JPEGVideoRTPSink.cpp \
#        Lib/liveMedia/JPEGVideoRTPSource.cpp \
#        Lib/liveMedia/JPEGVideoSource.cpp \
#        Lib/liveMedia/Locale.cpp \
#        Lib/liveMedia/MP3ADU.cpp \
#        Lib/liveMedia/MP3ADURTPSink.cpp \
#        Lib/liveMedia/MP3ADURTPSource.cpp \
#        Lib/liveMedia/MP3ADUTranscoder.cpp \
#        Lib/liveMedia/MP3ADUdescriptor.cpp \
#        Lib/liveMedia/MP3ADUinterleaving.cpp \
#        Lib/liveMedia/MP3AudioFileServerMediaSubsession.cpp \
#        Lib/liveMedia/MP3AudioMatroskaFileServerMediaSubsession.cpp \
#        Lib/liveMedia/MP3FileSource.cpp \
#        Lib/liveMedia/MP3Internals.cpp \
#        Lib/liveMedia/MP3InternalsHuffman.cpp \
#        Lib/liveMedia/MP3InternalsHuffmanTable.cpp \
#        Lib/liveMedia/MP3StreamState.cpp \
#        Lib/liveMedia/MP3Transcoder.cpp \
#        Lib/liveMedia/MPEG1or2AudioRTPSink.cpp \
#        Lib/liveMedia/MPEG1or2AudioRTPSource.cpp \
#        Lib/liveMedia/MPEG1or2AudioStreamFramer.cpp \
#        Lib/liveMedia/MPEG1or2Demux.cpp \
#        Lib/liveMedia/MPEG1or2DemuxedElementaryStream.cpp \
#        Lib/liveMedia/MPEG1or2DemuxedServerMediaSubsession.cpp \
#        Lib/liveMedia/MPEG1or2FileServerDemux.cpp \
#        Lib/liveMedia/MPEG1or2VideoFileServerMediaSubsession.cpp \
#        Lib/liveMedia/MPEG1or2VideoRTPSink.cpp \
#        Lib/liveMedia/MPEG1or2VideoRTPSource.cpp \
#        Lib/liveMedia/MPEG1or2VideoStreamDiscreteFramer.cpp \
#        Lib/liveMedia/MPEG1or2VideoStreamFramer.cpp \
#        Lib/liveMedia/MPEG2IndexFromTransportStream.cpp \
#        Lib/liveMedia/MPEG2TransportFileServerMediaSubsession.cpp \
#        Lib/liveMedia/MPEG2TransportStreamAccumulator.cpp \
#        Lib/liveMedia/MPEG2TransportStreamFramer.cpp \
#        Lib/liveMedia/MPEG2TransportStreamFromESSource.cpp \
#        Lib/liveMedia/MPEG2TransportStreamFromPESSource.cpp \
#        Lib/liveMedia/MPEG2TransportStreamIndexFile.cpp \
#        Lib/liveMedia/MPEG2TransportStreamMultiplexor.cpp \
#        Lib/liveMedia/MPEG2TransportStreamTrickModeFilter.cpp \
#        Lib/liveMedia/MPEG2TransportUDPServerMediaSubsession.cpp \
#        Lib/liveMedia/MPEG4ESVideoRTPSink.cpp \
#        Lib/liveMedia/MPEG4ESVideoRTPSource.cpp \
#        Lib/liveMedia/MPEG4GenericRTPSink.cpp \
#        Lib/liveMedia/MPEG4GenericRTPSource.cpp \
#        Lib/liveMedia/MPEG4LATMAudioRTPSink.cpp \
#        Lib/liveMedia/MPEG4LATMAudioRTPSource.cpp \
#        Lib/liveMedia/MPEG4VideoFileServerMediaSubsession.cpp \
#        Lib/liveMedia/MPEG4VideoStreamDiscreteFramer.cpp \
#        Lib/liveMedia/MPEG4VideoStreamFramer.cpp \
#        Lib/liveMedia/MPEGVideoStreamFramer.cpp \
#        Lib/liveMedia/MPEGVideoStreamParser.cpp \
#        Lib/liveMedia/MatroskaDemuxedTrack.cpp \
#        Lib/liveMedia/MatroskaFile.cpp \
#        Lib/liveMedia/MatroskaFileParser.cpp \
#        Lib/liveMedia/MatroskaFileServerDemux.cpp \
#        Lib/liveMedia/MatroskaFileServerMediaSubsession.cpp \
#        Lib/liveMedia/Media.cpp \
#        Lib/liveMedia/MediaSession.cpp \
#        Lib/liveMedia/MediaSink.cpp \
#        Lib/liveMedia/MediaSource.cpp \
#        Lib/liveMedia/MultiFramedRTPSink.cpp \
#        Lib/liveMedia/MultiFramedRTPSource.cpp \
#        Lib/liveMedia/OggDemuxedTrack.cpp \
#        Lib/liveMedia/OggFile.cpp \
#        Lib/liveMedia/OggFileParser.cpp \
#        Lib/liveMedia/OggFileServerDemux.cpp \
#        Lib/liveMedia/OggFileServerMediaSubsession.cpp \
#        Lib/liveMedia/OggFileSink.cpp \
#        Lib/liveMedia/OnDemandServerMediaSubsession.cpp \
#        Lib/liveMedia/OutputFile.cpp \
#        Lib/liveMedia/PassiveServerMediaSubsession.cpp \
#        Lib/liveMedia/ProxyServerMediaSession.cpp \
#        Lib/liveMedia/QCELPAudioRTPSource.cpp \
#        Lib/liveMedia/QuickTimeFileSink.cpp \
#        Lib/liveMedia/QuickTimeGenericRTPSource.cpp \
#        Lib/liveMedia/RTCP.cpp \
#        Lib/liveMedia/RTPInterface.cpp \
#        Lib/liveMedia/RTPSink.cpp \
#        Lib/liveMedia/RTPSource.cpp \
#        Lib/liveMedia/RTSPClient.cpp \
#        Lib/liveMedia/RTSPCommon.cpp \
#        Lib/liveMedia/RTSPRegisterSender.cpp \
#        Lib/liveMedia/RTSPServer.cpp \
#        Lib/liveMedia/RTSPServerRegister.cpp \
#        Lib/liveMedia/RTSPServerSupportingHTTPStreaming.cpp \
#        Lib/liveMedia/RawVideoRTPSink.cpp \
#        Lib/liveMedia/RawVideoRTPSource.cpp \
#        Lib/liveMedia/SIPClient.cpp \
#        Lib/liveMedia/ServerMediaSession.cpp \
#        Lib/liveMedia/SimpleRTPSink.cpp \
#        Lib/liveMedia/SimpleRTPSource.cpp \
#        Lib/liveMedia/StreamParser.cpp \
#        Lib/liveMedia/StreamReplicator.cpp \
#        Lib/liveMedia/T140TextRTPSink.cpp \
#        Lib/liveMedia/TCPStreamSink.cpp \
#        Lib/liveMedia/TextRTPSink.cpp \
#        Lib/liveMedia/TheoraVideoRTPSink.cpp \
#        Lib/liveMedia/TheoraVideoRTPSource.cpp \
#        Lib/liveMedia/VP8VideoRTPSink.cpp \
#        Lib/liveMedia/VP8VideoRTPSource.cpp \
#        Lib/liveMedia/VP9VideoRTPSink.cpp \
#        Lib/liveMedia/VP9VideoRTPSource.cpp \
#        Lib/liveMedia/VideoRTPSink.cpp \
#        Lib/liveMedia/VorbisAudioRTPSink.cpp \
#        Lib/liveMedia/VorbisAudioRTPSource.cpp \
#        Lib/liveMedia/WAVAudioFileServerMediaSubsession.cpp \
#        Lib/liveMedia/WAVAudioFileSource.cpp \
#        Lib/liveMedia/ourMD5.cpp \
#        Lib/liveMedia/rtcp_from_spec.c \
#        Lib/liveMedia/speexaudiofilter.cpp \
#        Lib/liveMedia/uLawAudioFilter.cpp \
        Lib/qjson/src/cmdline_tester.cpp \
        Lib/qjson/src/cmdlineparser.cpp \
        Lib/qjson/src/json_parser.cc \
        Lib/qjson/src/json_scanner.cc \
        Lib/qjson/src/json_scanner.cpp \
        Lib/qjson/src/parser.cpp \
        Lib/qjson/src/parserrunnable.cpp \
        Lib/qjson/src/qobjecthelper.cpp \
        Lib/qjson/src/serializer.cpp \
        Lib/qjson/src/serializerrunnable.cpp \
        Lib/aes/qaesencryption.cpp \
#        Lib/aes/aestest.cpp \
#        Program/database.cpp \
#        Program/model.cpp \
        appcore.cpp \
#        audioplayer.cpp \
#        audiorecoder.cpp \
        assignment_list.cpp \
        firmware.cpp \
        groups.cpp \
        main.cpp \
#        rtpsession.cpp \
 \        #directory.cpp  file.cpp  dialogPlugin.cpp
    network.cpp \
    programmer.cpp \
    receiver.cpp \
        subscriber.cpp \
    terminal.cpp \
    verification.cpp

RESOURCES += qml.qrc \
            icons/GIT_en.png \
            icons/git-logo.png \
            qtquickcontrols2.conf

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/GIT_en.png \
    icons/GIT_en.svg \
    icons/git-logo.png \
    qtquickcontrols2.conf

INCLUDEPATH += \
#        Lib/BasicUsageEnvironment/include/ \
#        Lib/UsageEnvironment/include/ \
#        Lib/liveMedia/ \
#        Lib/liveMedia/include/ \
#        Lib/groupsock/include/ \
#        Lib/SPEEXX/ \
        Lib/qjson/include/ \
        Lib/aes/

LIBS += -lboost_filesystem -lboost_system
#-lmysqlclient

# QMAKE_CXXFLAGS -= -Wmismatched-tags
#   !contains(QT_ARCH, x86_64){
#                 message("Compiling for 32bit system")
#                  OS_TYPE = linux32
# }else{
#                 message("Compiling for 64bit system")
#                 OS_TYPE = linux64
# }


HEADERS += \
#    Lib/BasicUsageEnvironment/include/BasicHashTable.hh \
#    Lib/BasicUsageEnvironment/include/BasicUsageEnvironment.hh \
#    Lib/BasicUsageEnvironment/include/BasicUsageEnvironment0.hh \
#    Lib/BasicUsageEnvironment/include/BasicUsageEnvironment_version.hh \
#    Lib/BasicUsageEnvironment/include/DelayQueue.hh \
#    Lib/BasicUsageEnvironment/include/HandlerSet.hh \
#    Lib/SPEEXX/speexx.h \
#    Lib/UsageEnvironment/include/Boolean.hh \
#    Lib/UsageEnvironment/include/HashTable.hh \
#    Lib/UsageEnvironment/include/UsageEnvironment.hh \
#    Lib/UsageEnvironment/include/UsageEnvironment_version.hh \
#    Lib/UsageEnvironment/include/strDup.hh \
#    Lib/groupsock/include/GroupEId.hh \
#    Lib/groupsock/include/Groupsock.hh \
#    Lib/groupsock/include/GroupsockHelper.hh \
#    Lib/groupsock/include/IOHandlers.hh \
#    Lib/groupsock/include/NetAddress.hh \
#    Lib/groupsock/include/NetCommon.h \
#    Lib/groupsock/include/NetInterface.hh \
#    Lib/groupsock/include/TunnelEncaps.hh \
#    Lib/groupsock/include/groupsock_version.hh \
#    Lib/liveMedia/EBMLNumber.hh \
#    Lib/liveMedia/H263plusVideoStreamParser.hh \
#    Lib/liveMedia/MP3ADUdescriptor.hh \
#    Lib/liveMedia/MP3AudioMatroskaFileServerMediaSubsession.hh \
#    Lib/liveMedia/MP3Internals.hh \
#    Lib/liveMedia/MP3InternalsHuffman.hh \
#    Lib/liveMedia/MP3StreamState.hh \
#    Lib/liveMedia/MPEGVideoStreamParser.hh \
#    Lib/liveMedia/MatroskaDemuxedTrack.hh \
#    Lib/liveMedia/MatroskaFileParser.hh \
#    Lib/liveMedia/MatroskaFileServerMediaSubsession.hh \
#    Lib/liveMedia/OggDemuxedTrack.hh \
#    Lib/liveMedia/OggFileParser.hh \
#    Lib/liveMedia/OggFileServerMediaSubsession.hh \
#    Lib/liveMedia/StreamParser.hh \
#    Lib/liveMedia/include/AC3AudioFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/AC3AudioRTPSink.hh \
#    Lib/liveMedia/include/AC3AudioRTPSource.hh \
#    Lib/liveMedia/include/AC3AudioStreamFramer.hh \
#    Lib/liveMedia/include/ADTSAudioFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/ADTSAudioFileSource.hh \
#    Lib/liveMedia/include/AMRAudioFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/AMRAudioFileSink.hh \
#    Lib/liveMedia/include/AMRAudioFileSource.hh \
#    Lib/liveMedia/include/AMRAudioRTPSink.hh \
#    Lib/liveMedia/include/AMRAudioRTPSource.hh \
#    Lib/liveMedia/include/AMRAudioSource.hh \
#    Lib/liveMedia/include/AVIFileSink.hh \
#    Lib/liveMedia/include/AudioInputDevice.hh \
#    Lib/liveMedia/include/AudioRTPSink.hh \
#    Lib/liveMedia/include/Base64.hh \
#    Lib/liveMedia/include/BasicUDPSink.hh \
#    Lib/liveMedia/include/BasicUDPSource.hh \
#    Lib/liveMedia/include/BitVector.hh \
#    Lib/liveMedia/include/ByteStreamFileSource.hh \
#    Lib/liveMedia/include/ByteStreamMemoryBufferSource.hh \
#    Lib/liveMedia/include/ByteStreamMultiFileSource.hh \
#    Lib/liveMedia/include/DVVideoFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/DVVideoRTPSink.hh \
#    Lib/liveMedia/include/DVVideoRTPSource.hh \
#    Lib/liveMedia/include/DVVideoStreamFramer.hh \
#    Lib/liveMedia/include/DeviceSource.hh \
#    Lib/liveMedia/include/DigestAuthentication.hh \
#    Lib/liveMedia/include/FileServerMediaSubsession.hh \
#    Lib/liveMedia/include/FileSink.hh \
#    Lib/liveMedia/include/FramedFileSource.hh \
#    Lib/liveMedia/include/FramedFilter.hh \
#    Lib/liveMedia/include/FramedSource.hh \
#    Lib/liveMedia/include/GSMAudioRTPSink.hh \
#    Lib/liveMedia/include/GenericMediaServer.hh \
#    Lib/liveMedia/include/H261VideoRTPSource.hh \
#    Lib/liveMedia/include/H263plusVideoFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/H263plusVideoRTPSink.hh \
#    Lib/liveMedia/include/H263plusVideoRTPSource.hh \
#    Lib/liveMedia/include/H263plusVideoStreamFramer.hh \
#    Lib/liveMedia/include/H264VideoFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/H264VideoFileSink.hh \
#    Lib/liveMedia/include/H264VideoRTPSink.hh \
#    Lib/liveMedia/include/H264VideoRTPSource.hh \
#    Lib/liveMedia/include/H264VideoStreamDiscreteFramer.hh \
#    Lib/liveMedia/include/H264VideoStreamFramer.hh \
#    Lib/liveMedia/include/H264or5VideoFileSink.hh \
#    Lib/liveMedia/include/H264or5VideoRTPSink.hh \
#    Lib/liveMedia/include/H264or5VideoStreamDiscreteFramer.hh \
#    Lib/liveMedia/include/H264or5VideoStreamFramer.hh \
#    Lib/liveMedia/include/H265VideoFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/H265VideoFileSink.hh \
#    Lib/liveMedia/include/H265VideoRTPSink.hh \
#    Lib/liveMedia/include/H265VideoRTPSource.hh \
#    Lib/liveMedia/include/H265VideoStreamDiscreteFramer.hh \
#    Lib/liveMedia/include/H265VideoStreamFramer.hh \
#    Lib/liveMedia/include/InputFile.hh \
#    Lib/liveMedia/include/JPEG2000VideoRTPSink.hh \
#    Lib/liveMedia/include/JPEG2000VideoRTPSource.hh \
#    Lib/liveMedia/include/JPEGVideoRTPSink.hh \
#    Lib/liveMedia/include/JPEGVideoRTPSource.hh \
#    Lib/liveMedia/include/JPEGVideoSource.hh \
#    Lib/liveMedia/include/Locale.hh \
#    Lib/liveMedia/include/MP3ADU.hh \
#    Lib/liveMedia/include/MP3ADURTPSink.hh \
#    Lib/liveMedia/include/MP3ADURTPSource.hh \
#    Lib/liveMedia/include/MP3ADUTranscoder.hh \
#    Lib/liveMedia/include/MP3ADUinterleaving.hh \
#    Lib/liveMedia/include/MP3AudioFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/MP3FileSource.hh \
#    Lib/liveMedia/include/MP3Transcoder.hh \
#    Lib/liveMedia/include/MPEG1or2AudioRTPSink.hh \
#    Lib/liveMedia/include/MPEG1or2AudioRTPSource.hh \
#    Lib/liveMedia/include/MPEG1or2AudioStreamFramer.hh \
#    Lib/liveMedia/include/MPEG1or2Demux.hh \
#    Lib/liveMedia/include/MPEG1or2DemuxedElementaryStream.hh \
#    Lib/liveMedia/include/MPEG1or2DemuxedServerMediaSubsession.hh \
#    Lib/liveMedia/include/MPEG1or2FileServerDemux.hh \
#    Lib/liveMedia/include/MPEG1or2VideoFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/MPEG1or2VideoRTPSink.hh \
#    Lib/liveMedia/include/MPEG1or2VideoRTPSource.hh \
#    Lib/liveMedia/include/MPEG1or2VideoStreamDiscreteFramer.hh \
#    Lib/liveMedia/include/MPEG1or2VideoStreamFramer.hh \
#    Lib/liveMedia/include/MPEG2IndexFromTransportStream.hh \
#    Lib/liveMedia/include/MPEG2TransportFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamAccumulator.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamFramer.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamFromESSource.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamFromPESSource.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamIndexFile.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamMultiplexor.hh \
#    Lib/liveMedia/include/MPEG2TransportStreamTrickModeFilter.hh \
#    Lib/liveMedia/include/MPEG2TransportUDPServerMediaSubsession.hh \
#    Lib/liveMedia/include/MPEG4ESVideoRTPSink.hh \
#    Lib/liveMedia/include/MPEG4ESVideoRTPSource.hh \
#    Lib/liveMedia/include/MPEG4GenericRTPSink.hh \
#    Lib/liveMedia/include/MPEG4GenericRTPSource.hh \
#    Lib/liveMedia/include/MPEG4LATMAudioRTPSink.hh \
#    Lib/liveMedia/include/MPEG4LATMAudioRTPSource.hh \
#    Lib/liveMedia/include/MPEG4VideoFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/MPEG4VideoStreamDiscreteFramer.hh \
#    Lib/liveMedia/include/MPEG4VideoStreamFramer.hh \
#    Lib/liveMedia/include/MPEGVideoStreamFramer.hh \
#    Lib/liveMedia/include/MatroskaFile.hh \
#    Lib/liveMedia/include/MatroskaFileServerDemux.hh \
#    Lib/liveMedia/include/Media.hh \
#    Lib/liveMedia/include/MediaSession.hh \
#    Lib/liveMedia/include/MediaSink.hh \
#    Lib/liveMedia/include/MediaSource.hh \
#    Lib/liveMedia/include/MediaTranscodingTable.hh \
#    Lib/liveMedia/include/MultiFramedRTPSink.hh \
#    Lib/liveMedia/include/MultiFramedRTPSource.hh \
#    Lib/liveMedia/include/OggFile.hh \
#    Lib/liveMedia/include/OggFileServerDemux.hh \
#    Lib/liveMedia/include/OggFileSink.hh \
#    Lib/liveMedia/include/OnDemandServerMediaSubsession.hh \
#    Lib/liveMedia/include/OutputFile.hh \
#    Lib/liveMedia/include/PassiveServerMediaSubsession.hh \
#    Lib/liveMedia/include/ProxyServerMediaSession.hh \
#    Lib/liveMedia/include/QCELPAudioRTPSource.hh \
#    Lib/liveMedia/include/QuickTimeFileSink.hh \
#    Lib/liveMedia/include/QuickTimeGenericRTPSource.hh \
#    Lib/liveMedia/include/RTCP.hh \
#    Lib/liveMedia/include/RTPInterface.hh \
#    Lib/liveMedia/include/RTPSink.hh \
#    Lib/liveMedia/include/RTPSource.hh \
#    Lib/liveMedia/include/RTSPClient.hh \
#    Lib/liveMedia/include/RTSPCommon.hh \
#    Lib/liveMedia/include/RTSPRegisterSender.hh \
#    Lib/liveMedia/include/RTSPServer.hh \
#    Lib/liveMedia/include/RTSPServerSupportingHTTPStreaming.hh \
#    Lib/liveMedia/include/RawVideoRTPSink.hh \
#    Lib/liveMedia/include/RawVideoRTPSource.hh \
#    Lib/liveMedia/include/SIPClient.hh \
#    Lib/liveMedia/include/ServerMediaSession.hh \
#    Lib/liveMedia/include/SimpleRTPSink.hh \
#    Lib/liveMedia/include/SimpleRTPSource.hh \
#    Lib/liveMedia/include/StreamReplicator.hh \
#    Lib/liveMedia/include/T140TextRTPSink.hh \
#    Lib/liveMedia/include/TCPStreamSink.hh \
#    Lib/liveMedia/include/TextRTPSink.hh \
#    Lib/liveMedia/include/TheoraVideoRTPSink.hh \
#    Lib/liveMedia/include/TheoraVideoRTPSource.hh \
#    Lib/liveMedia/include/VP8VideoRTPSink.hh \
#    Lib/liveMedia/include/VP8VideoRTPSource.hh \
#    Lib/liveMedia/include/VP9VideoRTPSink.hh \
#    Lib/liveMedia/include/VP9VideoRTPSource.hh \
#    Lib/liveMedia/include/VideoRTPSink.hh \
#    Lib/liveMedia/include/VorbisAudioRTPSink.hh \
#    Lib/liveMedia/include/VorbisAudioRTPSource.hh \
#    Lib/liveMedia/include/WAVAudioFileServerMediaSubsession.hh \
#    Lib/liveMedia/include/WAVAudioFileSource.hh \
#    Lib/liveMedia/include/liveMedia.hh \
#    Lib/liveMedia/include/liveMedia_version.hh \
#    Lib/liveMedia/include/ourMD5.hh \
#    Lib/liveMedia/include/uLawAudioFilter.hh \
#    Lib/liveMedia/rtcp_from_spec.h \
#    Lib/liveMedia/speexaudiofilter.h \
    Lib/qjson/include/QJson/Parser \
    Lib/qjson/include/QJson/QObjectHelper \
    Lib/qjson/include/QJson/Serializer \
    Lib/qjson/src/FlexLexer.h \
    Lib/qjson/src/cmdlineparser.h \
    Lib/qjson/src/json_parser.hh \
    Lib/qjson/src/json_scanner.h \
    Lib/qjson/src/location.hh \
    Lib/qjson/src/parser.h \
    Lib/qjson/src/parser_p.h \
    Lib/qjson/src/parserrunnable.h \
    Lib/qjson/src/position.hh \
    Lib/qjson/src/qjson_debug.h \
    Lib/qjson/src/qjson_export.h \
    Lib/qjson/src/qobjecthelper.h \
    Lib/qjson/src/serializer.h \
    Lib/qjson/src/serializerrunnable.h \
    Lib/qjson/src/stack.hh \
    Lib/aes/qaesencryption.h \
#    Lib/aes/aestest.h \
#    Program/database.h \
#    Program/model.h \
    RuLang.h \
    RuTranslator.h \
    appcore.h \
    receiver.h
#    asio_compatibility.hpp \
#    audioplayer.h \
#    audiorecorder.h \
#    client_http.hpp \
#    client_https.hpp \
#    crypto.hpp \
#    rtpsession.h \
#    server_http.hpp \
#    server_https.hpp \
#    status_code.hpp \
 #    utility.hpp \



