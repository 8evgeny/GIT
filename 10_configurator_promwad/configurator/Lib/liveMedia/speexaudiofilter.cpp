#include "speexaudiofilter.h"
#include "speexx.h"


SpeexFromPCMAudioSource *SpeexFromPCMAudioSource
::createNew(UsageEnvironment &env, FramedSource *inputSource)
{
    return new SpeexFromPCMAudioSource(env, inputSource);
}

SpeexFromPCMAudioSource
::SpeexFromPCMAudioSource(UsageEnvironment &env, FramedSource *inputSource)
    : FramedFilter(env, inputSource),
      fInputBuffer(NULL), fInputBufferSize(0)
{
    _speex.initializeEncoder();
}

SpeexFromPCMAudioSource::~SpeexFromPCMAudioSource()
{
    delete[] fInputBuffer;
}

void SpeexFromPCMAudioSource::doGetNextFrame()
{
    unsigned bytesToRead = fMaxSize * 2;
    if (bytesToRead > fInputBufferSize) {
        delete[] fInputBuffer;
        fInputBuffer = new unsigned char[bytesToRead];
        fInputBufferSize = bytesToRead;
    }

    fInputSource->getNextFrame(fInputBuffer, bytesToRead,
                               afterGettingFrame, this,
                               FramedSource::handleClosure, this);
}

void SpeexFromPCMAudioSource
::afterGettingFrame(void *clientData, unsigned frameSize,
                    unsigned numTruncatedBytes,
                    struct timeval presentationTime,
                    unsigned durationInMicroseconds)
{
    SpeexFromPCMAudioSource *source = (SpeexFromPCMAudioSource *)clientData;
    source->afterGettingFrame1(frameSize, numTruncatedBytes,
                               presentationTime, durationInMicroseconds);
}


void SpeexFromPCMAudioSource
::afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes,
                     struct timeval presentationTime,
                     unsigned durationInMicroseconds)
{
    int uu = 0;
    int nbBYtes;
    nbBYtes = _speex.encodingVoice((spx_int16_t *)fInputBuffer, (char *)fTo, (unsigned int) frameSize);
    fFrameSize = nbBYtes ;
    fNumTruncatedBytes = numTruncatedBytes;
    fPresentationTime = presentationTime;
    fDurationInMicroseconds = durationInMicroseconds;
    afterGetting(this);
}
