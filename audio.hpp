#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "portaudio.h"
#include <vector>
#include <string.h>

namespace Audio {

    class Sample {
    private:
        std::vector<float> data;
        unsigned int position;

    public:
        Sample();
        ~Sample();
        void load(const char *filename, float volume=1.0);
        std::vector<float> get_data();
        unsigned int get_position();
        void set_position(unsigned int pos);
        bool finished();

    };


    class Player {
    private:
        Sample *audio_sample;
        bool playing;

        PaStreamParameters  outputParameters;
        PaStream*           stream;
        PaError             err;
        PaTime              streamOpened;


    public:
        Player();
        ~Player();

        void init();
        void pause();
        void stop();
        void play(Sample *audiosample);
        Sample *get_data();
    };
}
#endif
