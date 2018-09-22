#include "RtAudio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

#include "audio.hpp"

#define DEBUG 0

using namespace Audio;

Sample::Sample()
{

}

Sample::~Sample()
{

}

bool Sample::finished()
{
    return ((this->position) >= (this->info.frames));
}

void Sample::load(std::string filename)
{
    this->info.format = 0;
    this->data = sf_open(filename.c_str(), SFM_READ, &this->info);
    this->position = 0;
}

SNDFILE *Sample::get_data()
{
    return this->data;
}

SF_INFO *Sample::get_info()
{
    return &this->info;
}
unsigned long Sample::get_position()
{
    return this->position;
}

void Sample::set_position(unsigned long pos)
{
    this->position = pos;
}

Player::Player()
{
    this->playing = false;
}

void Player::pause()
{
    this->playing = false;
}

Player::~Player()
{

}

Sample *Player::get_data() {
    return this->audio_sample;
}

void Player::play(Sample *audiosample)
{
    audiosample->set_position(0);
    this->audio_sample = audiosample;
}

int mix_and_play (const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData )

{
    Player *player = (Player*) userData;
    int *buffer = (int *) outputBuffer;
    Sample *s;

    unsigned long framesLeft = framesPerBuffer;
    unsigned long framesRead;

    s = player->get_data();
    if (s != NULL) {
        SNDFILE *data = s->get_data();
        unsigned long pos = s->get_position();
	SF_INFO *info = s->get_info();

        sf_seek(data, pos*info->channels, SEEK_SET);


        if (framesLeft > (info->frames - pos)) {
            framesRead = (unsigned int) (info->frames - pos);
	    //playbackEnded = true;
	    framesLeft = framesRead;
	    s->set_position(info->frames);
        } else {
            framesRead = framesLeft;
            pos += framesRead;
	    s->set_position(pos);
        }

        sf_readf_int(data, buffer, framesRead);
    }
    return 0;
}

void Player::init()
{
    PaError err;

    /* Pa_Initialize throws a bunch of log messages in stderr, so just ignore them here to avoid spamming on the screen */
    freopen("/dev/null","w",stderr);
    err = Pa_Initialize();

    if( err != paNoError ) {
#if DEBUG
        std::cerr << "Error on Pa_Initialize()" << std::endl;
#endif
        return;
    }

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* Default output device. */
    if (outputParameters.device == paNoDevice) {
#if DEBUG
        std::cerr << "Error: No default output device on Pa_GetDefaultOutputDevice()" << std::endl;
#endif
        return;
    }

    outputParameters.channelCount = 1;                     /* Mono output. */
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream( &stream,
                         NULL,      /* No input. */
                         &outputParameters,
                         44100,
                         256,       /* Frames per buffer. */
                         paClipOff, /* We won't output out of range samples so don't bother clipping them. */
                         mix_and_play,
                         this );

    if( err != paNoError ) {
#if DEBUG
        std::cerr << "Error on Pa_OpenStream()" << std::endl;
#endif
        return;
    }

    err = Pa_StartStream( stream );
    if( err != paNoError ) {
#if DEBUG
        std::cerr << "Error on Pa_StartStream()" << std::endl;
#endif
        return;
    }
}

void Player::stop()
{
    PaError err;
    err = Pa_StopStream( stream );
    if( err != paNoError ) {
#if DEBUG
        std::cerr << "Error on Pa_StopStream()" << std::endl;
#endif
        return;
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError ) {
#if DEBUG
        std::cerr << "Error on Pa_StopStream()" << std::endl;
#endif
        return;
    }

    Pa_Terminate();
}
