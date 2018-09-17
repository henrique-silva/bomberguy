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
    if ( (this->position) >= (this->data.size())) return true;
    else return false;
}

void Sample::load(const char *filename, float volume)
{
    std::string buffer;
    float fdata;
    std::ifstream input_file;
    unsigned int count = 0;

    input_file.open(filename, std::ios_base::in);
    if (!input_file) {
#if DEBUG
        std::cerr << "Arquivo " << filename << " nao encontrado" << std::endl;
#endif
        return;
    }

    while (std::getline(input_file, buffer) ) {
        std::stringstream(buffer) >> fdata;
        (this->data).push_back(fdata*volume);
        count ++;
    }
    this->position = 0;
#if DEBUG
    std::cerr << "Total: " << count << " samples" << std::endl;
#endif
}

unsigned int Sample::get_position()
{
    return this->position;
}

void Sample::set_position(unsigned int pos)
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

std::vector<float> Sample::get_data() {
    return this->data;
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
    float *buffer = (float *) outputBuffer;
    Sample *s;
    s = player->get_data();
    if (s != NULL) {
        std::vector<float> data = s->get_data();
        unsigned int pos = s->get_position();

        // Fill the buffer with samples!
        for (int i=0; (i<framesPerBuffer); i++, pos++) {
            if (pos < data.size())
                buffer[i] = data[pos];
            else
                buffer[i] = 0;
        }
        s->set_position(pos);
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
