#ifndef PLAYBACK_HPP
#define PLAYBACK_HPP

#include "portaudio.h"
#include <vector>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

#define AUDIO_BACKGROUND_MUSIC  "assets/level1.dat"
#define AUDIO_GAMEOVER_MUSIC    "assets/game_over.dat"
#define AUDIO_BOMB_DROP         "assets/sfx/bomb_drop.dat"
#define AUDIO_EXPLOSION         "assets/sfx/explosion.dat"
#define AUDIO_DOOR_DISCOVER     "assets/sfx/door_discover.dat"
#define AUDIO_POWER_UP          "assets/sfx/power_up.dat"


namespace Audio {

    class Sample {
    private:
        std::string name;
        std::vector<float> data;
        unsigned int position;

    public:
        Sample(std::string filename, float volume = 1.0);
        ~Sample();
        void load(std::string filename, float volume = 1.0);
        std::vector<float> get_data();
        unsigned int get_position();
        void set_position(unsigned int pos);
        bool finished();
        std::string get_name();
    };

    class Player {
    private:
        std::vector<Sample *> sample_list;
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
        void play(std::string sample_name);
        Sample * load_sample(std::string sample_name, float volume = 1.0);
        Sample * load_sample_list(std::vector<std::string> name_list);
        Sample * find_sample_by_name(std::string sample_name);
        Sample * get_data();
    };

}
#endif
