#ifndef RTPSESSION_H
#define RTPSESSION_H

#include "liveMedia.hh"
#include "GroupsockHelper.hh"

#include "BasicUsageEnvironment.hh"
#include "speexaudiofilter.h"

struct sessionState_t {
    FramedSource *source;
    RTPSink *sink;
    RTCPInstance *rtcpInstance;
    Groupsock *rtpGroupsock;
    Groupsock *rtcpGroupsock;
};


class RtpSession
{
public:
    RtpSession();
    ~RtpSession();
    void run();
    void create();
    void stop();
    TaskScheduler *scheduler;
    UsageEnvironment *env;
private:
    char const *destinationAddressStr;

    unsigned short rtpPortNum;
    unsigned short rtcpPortNum;
    unsigned char ttl; // low, in case routers don't admin scope
    struct in_addr destinationAddress;

    WAVAudioFileSource *source;
    Port *rtpPort;
    Port *rtcpPort;
    sessionState_t sessionState;
    char inputFileName[255];
    unsigned char payloadFormatCode;
    char const *mimeType;
    unsigned char numChannels;
    unsigned samplingFrequency;
    // Create (and start) a 'RTCP instance' for this RTP sink:
    unsigned estimatedSessionBandwidth; // in kbps; for RTCP b/w share
    unsigned maxCNAMElen;
    unsigned char CNAME[100 + 1];
    Boolean isSSM;

    // just in case
};

#endif // RTPSESSION_H
