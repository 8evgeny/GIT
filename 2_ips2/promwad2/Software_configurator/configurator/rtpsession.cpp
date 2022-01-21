#include "rtpsession.h"
#include <stdio.h>
#include <stdlib.h>


static void afterPlaying(void *clientData); // forward

RtpSession::RtpSession()
{
    scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);

}

RtpSession::~RtpSession()
{
    delete rtpPort;
    delete rtcpPort;
    // Begin by setting up our usage environment:
}

static void afterPlaying(void *clientData)
{
    RtpSession *session = (RtpSession *) clientData;
    session->stop();

}

void RtpSession::run()
{
    sprintf(inputFileName, "%s", "test.wav");
    // Open the file as a 'MP3 file source':
    source = WAVAudioFileSource::createNew(*env, inputFileName);
    if (source == NULL) {
        *env << "Unable to open file \"" << inputFileName
             << "\" as a MP3 file source\n";
        exit(1);
    }
    sessionState.source = SpeexFromPCMAudioSource::createNew(*env, source);
    //  sessionState.source =  source;
    // Finally, start the streaming:
    *env << "Beginning streaming...\n";
    sessionState.sink->startPlaying(*sessionState.source, afterPlaying, this);
    env->taskScheduler().doEventLoop(); // does not return

}

void RtpSession::create()
{

    destinationAddressStr = "192.168.3.100";
    rtpPortNum = 6666;
    ttl = 1; // low, in case routers don't admin scope
    destinationAddress.s_addr = our_inet_addr(destinationAddressStr);
    rtpPort = new Port(rtpPortNum);
    rtcpPort = new Port(rtcpPortNum);
    sessionState.rtpGroupsock
        = new Groupsock(*env, destinationAddress, *rtpPort, ttl);

    payloadFormatCode = 101;
    mimeType = "audio";
    numChannels = 1;
    isSSM = false;
    samplingFrequency = 44100;
    maxCNAMElen = 100;
    gethostname((char *)CNAME, maxCNAMElen);
    CNAME[maxCNAMElen] = '\0';
    sessionState.sink = SimpleRTPSink::createNew(*env, sessionState.rtpGroupsock,
                        payloadFormatCode, samplingFrequency,
                        "audio", mimeType, numChannels);

}

void RtpSession::stop()
{
    *env << "...done streaming\n";

    sessionState.sink->stopPlaying();

    // End this loop by closing the current source:
    Medium::close(sessionState.source);
}


