#include "playback.hpp"

using namespace Audio;

int mix_and_play (const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
		  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
                  void *userData );

Sample::Sample(std::string filename, float volume)
{
    this->name = filename;
    this->load(filename, volume);
}

Sample::~Sample()
{

}

bool Sample::finished()
{
    return (this->position) >= (this->data.size());
}

void Sample::load(std::string filename, float volume)
{
    std::string buffer;
    float fdata;
    std::ifstream input_file;
    unsigned int count = 0;

    input_file.open(filename, std::ios_base::in);
    if (!input_file) {
        std::cerr << "Arquivo " << filename << " nao encontrado" << std::endl;
        return;
    }

    while (std::getline(input_file, buffer) ) {
        std::stringstream(buffer) >> fdata;
        (this->data).push_back(fdata*volume);
        count ++;
    }
    this->position = 0;

    std::cerr << "Total: " << count << " samples" << std::endl;
}

unsigned int Sample::get_position()
{
    return this->position;
}

void Sample::set_position(unsigned int pos)
{
    this->position = pos;
}

std::vector<float> Sample::get_data()
{
    return this->data;
}

std::string Sample::get_name()
{
    return this->name;
}

Player::Player()
{
    this->playing = false;
    this->audio_sample = NULL;
    this->init();
}

void Player::pause()
{
    this->playing = false;
}

Player::~Player()
{
    for (auto sample : sample_list) {
	delete sample;
    }
}

Sample *Player::get_data()
{
    return this->audio_sample;
}

Sample * Player::load_sample(std::string sample_name, float volume)
{
    Sample * sample = new Sample(sample_name, volume);

    sample_list.push_back(sample);

    return sample;
}

Sample * Player::load_sample_list(std::vector<std::string> name_list)
{
    for (auto name: name_list) {
	load_sample(name);
    }
}

Sample * Player::find_sample_by_name(std::string sample_name)
{
    return *(std::find_if(this->sample_list.begin(), this->sample_list.end(),
                          [sample_name]( Sample* const s) -> bool { return (s->get_name() == sample_name); }));
}

void Player::play(std::string sample_name)
{
    PaError err;
    Sample * sample;

    sample = find_sample_by_name(sample_name);
    if (sample == NULL) {
        sample = load_sample(sample_name);
    }
    this->audio_sample = sample;
    this->audio_sample->set_position(0);

    if (Pa_IsStreamStopped(stream)) {
        err = Pa_StartStream( stream );
        if( err != paNoError ) {
            std::cerr << "Error on Pa_StartStream()" << std::endl;
            return;
        }
    }
}

void Player::play(Sample * sample)
{
    PaError err;
    this->audio_sample = sample;

    if (Pa_IsStreamStopped(stream)) {
        err = Pa_StartStream( stream );
        if( err != paNoError ) {
            std::cerr << "Error on Pa_StartStream()" << std::endl;
            return;
        }
    }
}

void Player::init()
{
    PaError err;

    freopen("/dev/null", "w", stderr);
    err = Pa_Initialize();
    if( err != paNoError ) {
        std::cerr << "Error on Pa_Initialize()" << std::endl;
        return;
    }

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* Default output device. */
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Error: No default output device on Pa_GetDefaultOutputDevice()" << std::endl;
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
                         512,       /* Frames per buffer. */
                         paNoFlag, /* We won't output out of range samples so don't bother clipping them. */
                         mix_and_play,
                         this );

    if( err != paNoError ) {
        std::cerr << "Error on Pa_OpenStream()" << std::endl;
        return;
    }
}

void Player::stop()
{
    PaError err;
    err = Pa_StopStream( stream );
    if( err != paNoError ) {
        std::cerr << "Error on Pa_StopStream()" << std::endl;
        return;
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError ) {
        std::cerr << "Error on Pa_StopStream()" << std::endl;
        return;
    }

    Pa_Terminate();
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
    return paContinue;
}
