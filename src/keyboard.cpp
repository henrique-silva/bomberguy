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
    initscr();      /* Start curses mode             */
    raw();                      /* Line buffering disabled     */
    keypad(stdscr, TRUE);    /* We get F1, F2 etc..         */
    noecho();                   /* Don't echo() while we do getch */
    //curs_set(0);                /* Do not display cursor */
    this->running = 1;
    std::thread newthread(kb_thread_f, &(this->buffer), &(this->running));
    (this->kb_thread).swap(newthread);
}

void Keyboard::stop() {
    this->running = 0;
    (this->kb_thread).join();
    endwin();
}

int Keyboard::getchar() {
    int c = 0;
    if (!this->buffer.empty()) {
	c = this->buffer.front();
	this->buffer.pop();
    }
    return c;
}
