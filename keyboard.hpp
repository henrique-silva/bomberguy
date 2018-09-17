#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <queue>
#include <thread>
#include <ncurses.h>

class Keyboard  {
  private:
    std::queue<int> buffer;
    int running;
    std::thread kb_thread;

  public:
    Keyboard();
    ~Keyboard();
    void stop();
    void init();
    int getchar();
};

void kb_thread_f(std::queue<int> *keybuffer, int *control);

#endif
