#include "setvalue.hpp"
#include "graphics.hpp"
#include <string>
#include <iostream>
using namespace genv;

SetValue::SetValue(Window * w, int x, int y, int sx, int sy, int min, int max) :
    Widget(w, x, y, sx, sy), _value(35), _min(min), _max(max), _step(1)
{

}

void SetValue::draw() const
{
    gout << move_to(_x, _y) << color(100, 100, 100) << box(_size_x, _size_y);
    gout << move_to(_x + 1, _y + 1) << color(255, 255, 255) << box(_size_x - 22, _size_y - 2);

    gout << move_to(_x + _size_x - 20, _y) << color(224, 224, 224) << box(20, _size_y / 2);
    gout << move_to(_x + _size_x - 20, _y + _size_y / 2) << color(224, 224, 224) << box(20, _size_y / 2);

    gout << move_to(_x + _size_x - 15, _y + 10) << color(0, 0, 0);
    gout << line_to(_x + _size_x - 10, _y + 5);
    gout << line_to(_x + _size_x - 5, _y + 10);
    gout << line_to(_x + _size_x - 15, _y + 10);

    gout << move_to(_x + _size_x - 15, _y + _size_y - 10) << color(0, 0, 0);
    gout << line_to(_x + _size_x - 10, _y + _size_y - 5);
    gout << line_to(_x + _size_x - 5, _y + _size_y - 10);
    gout << line_to(_x + _size_x - 15, _y + _size_y - 10);

    gout << move_to(_x + 8, _y + gout.cdescent() + (_size_y - gout.cascent()) / 3) << color(0, 0, 0) << text(std::to_string(_value));

}

void SetValue::handle(event ev)
{
    if (ev.type == ev_mouse && ev.button == btn_left)
    {
        if (is_selected(ev.pos_x, ev.pos_y))
        {
            if (ev.pos_x >= _x + _size_x - 20 &&
                ev.pos_x <= _x + _size_x &&
                ev.pos_y >= _y &&
                ev.pos_y <= _y + _size_y / 2)
            {
                if (_value + _step <= _max)
                {
                    _value += _step;
                }
            }
            else if (ev.pos_x >= _x + _size_x - 20 &&
                     ev.pos_x <= _x + _size_x &&
                     ev.pos_y >= _y + _size_y / 2 &&
                     ev.pos_y <= _y + _size_y)
            {
                if (_value - _step >= _min)
                {
                    _value -= _step;
                }
            }
        }
    }

}

int SetValue::get_value() const
{
    return _value;
}

void SetValue::set_value(int value, int min, int max, int step) {
    _min = min;
    _max = max;
    _step = step;
    if (value >= _min && value <= _max) {
        _value = value;
    } else if (value < _min) {
        _value = _min;
    } else {
        _value = _max;
    }
}
