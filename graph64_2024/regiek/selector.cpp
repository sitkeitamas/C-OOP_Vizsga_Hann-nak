#include "selector.hpp"
#include "graphics.hpp"
#include <string>
#include <iostream>

using namespace genv;
using namespace std;

Selector::Selector(Window* w, int x, int y, int sx, int sy, const std::vector<std::string>& options)
    : Widget(w, x, y, sx, sy), _options(options), _selected_index(-1)
{}

void Selector::draw() const
{
    gout << move_to(_x, _y)
         << color(200, 200, 200)
         << box(_size_x, _size_y)
         << color(0, 0, 0);

    int item_height = 20;
    for (size_t i = 0; i < _options.size(); ++i) {
        int item_y = _y + i* item_height;
        if ((int)i == _selected_index) {
            gout << move_to(_x, item_y)
                 << color(100, 100, 255)
                 << box(_size_x, item_height);
            gout << color(255, 255, 255);
        } else {
            gout << color(0, 0, 0);
        }
        gout << move_to(_x + 5, item_y + 5)
             << text(_options[i]);
    }
}

void Selector::handle(event ev)
{
    if (ev.type == ev_mouse && ev.button == btn_left) {
        int relative_y = ev.pos_y - _y;
        int item_height = 20;
        int index = relative_y / item_height;
        if (index >= 0 && index < (int)_options.size()) {
            _selected_index = index;
        }
    }
}

int Selector::get_selected_index() const
{
    return _selected_index;
}

std::string Selector::get_selected_value() const
{
    if (_selected_index >= 0 && _selected_index < (int)_options.size()) {
        return _options[_selected_index];
    }
    return "";
}
