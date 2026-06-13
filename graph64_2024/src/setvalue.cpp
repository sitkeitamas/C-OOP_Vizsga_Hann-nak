#include "setvalue.hpp"
#include "graphics.hpp"

using namespace genv;

SetValue::SetValue(Lib* lib, int x, int y, int sx, int sy, int min, int max)
    : Widget(lib, x, y, sx, sy), _value(min), _min(min), _max(max)
{
    set_draggable(true);
}

void SetValue::draw() const
{
    gout << color(91,104,143) << move_to(_x,_y) << box(_sx,_sy)
         << color(38,43,64) << move_to(_x+3,_y+3) << box(_sx-6,_sy-6)
         << color(255,210,95) << move_to(_x+12, _y+_sy/2-12) << text("-")
         << color(245,247,255) << move_to(_x+_sx/2-6, _y+_sy/2-10) << text(std::to_string(_value))
         << color(255,210,95) << move_to(_x+_sx-18, _y+_sy/2-10) << text("+");
}

void SetValue::handle(event ev)
{
    if (!is_clickable()) return;

    if (ev.type == ev_mouse && is_selected(ev.pos_x, ev.pos_y))
    {
        if (ev.button == btn_wheelup)
        {
            increase();
        }
        else if (ev.button == btn_wheeldown)
        {
            decrease();
        }
        else if (ev.button == btn_left)
        {
            int relative_x = ev.pos_x - _x;

            if (relative_x < _sx / 3)
            {
                decrease();   // "-" oldal
            }
            else if (relative_x > 2 * _sx / 3)
            {
                increase();   // "+" oldal
            }
        }
    }
}

void SetValue::increase() { if (_value < _max) _value++; }
void SetValue::decrease() { if (_value > _min) _value--; }
int SetValue::value() const { return _value; }

void SetValue::set_value(int value)
{
    if (value < _min) _value = _min;
    else if (value > _max) _value = _max;
    else _value = value;
}
