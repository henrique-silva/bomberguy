#include "keyboard.hpp"

/*********************** Keyboard Class methods ************************/
void kb_thread_f(std::queue<int> *keybuffer, int *control)
{
    int c;
    while ((*control) == 1) {
        c = getch();
        if (c!=ERR) {
	    keybuffer->push(c);
	}
        std::this_thread::sleep_for (std::chrono::milliseconds(20));
    }
    return;
}

Keyboard::Keyboard() {
}

Keyboard::~Keyboard() {
    this->stop();
}

void Keyboard::init() {
    /* NCurses must be initialized by now!! */
    this->running = 1;
    std::thread newthread(kb_thread_f, &(this->buffer), &(this->running));
    (this->kb_thread).swap(newthread);
}

void Keyboard::stop() {
    this->running = 0;
    (this->kb_thread).join();
}

int Keyboard::getchar() {
    int c = 0;
    if (!this->buffer.empty()) {
	c = this->buffer.front();
	//cout << "Popping char " << c << " from queue\n";
	this->buffer.pop();
    }
    return c;
}
