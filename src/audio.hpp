#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <portaudio.h>
#include <sndfile.h>
#include <vector>
#include <string.h>

namespace Audio {

    class Sample {
    private:
	SNDFILE * data;
	SF_INFO info;
        unsigned long position;

    public:
        Sample();
        ~Sample();
        void load(std::string filename);
        SNDFILE * get_data();
	SF_INFO *get_info();
        unsigned long get_position();
        void set_position(unsigned long pos);
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
