#ifndef SPEEXAUDIOFILTER_H
#define SPEEXAUDIOFILTER_H

#ifndef _FRAMED_FILTER_HH
#include "FramedFilter.hh"
#endif
#include "speexx.h"

class SpeexFromPCMAudioSource: public FramedFilter
{
public:
    static SpeexFromPCMAudioSource *
    createNew(UsageEnvironment &env, FramedSource *inputSource);

protected:
    SpeexFromPCMAudioSource(UsageEnvironment &env, FramedSource *inputSource);
    // called only by createNew()
    virtual ~SpeexFromPCMAudioSource();

private:
    // Redefined virtual functions:
    virtual void doGetNextFrame();

private:
    static void afterGettingFrame(void *clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void afterGettingFrame1(unsigned frameSize,
                            unsigned numTruncatedBytes,
                            struct timeval presentationTime,
                            unsigned durationInMicroseconds);

private:
    int fByteOrdering;
    unsigned char *fInputBuffer;
    unsigned fInputBufferSize;
    Speex _speex;
};
#endif // SPEEXAUDIOFILTER_H
