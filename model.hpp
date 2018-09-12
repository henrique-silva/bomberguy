#ifndef MODEL_HPP
#define MODEL_HPP

using namespace std;

typedef tuple<int, int> Position;

class Level {
private:
    Position door_pos;
    vector<Position> walls;

public:
    Level(int size, int door_x, int door_y);
    Position get_door_pos();
    void set_door_pos(int x, int y);
    void add_wall(int x, int y);
    vector<Position> get_walls();
};

#endif
