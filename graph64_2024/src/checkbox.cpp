#include "checkbox.hpp"
#include "graphics.hpp"

using namespace genv;

CheckBox::CheckBox(Lib *lib, int x, int y, int sx, int sy)
    : Widget(lib, x, y, sx, sy), _check(false) {}

void CheckBox::draw() const
{
    gout << color(255, 255, 255);
    gout << move_to(_x, _y);
    gout << box(_sx, _sy);

    gout << color(0, 0, 0);
    gout << move_to(_x + 5, _y + 5);
    gout << box(_sx - 10, _sy - 10);

    if (_check)
    {
        gout << color(255, 255, 255);

        gout << move_to(_x + 10, _y + 10);
        gout << line(_sx - 20, _sy - 20);

        gout << move_to(_x + _sx - 10, _y + 10);
        gout << line(-_sx + 20, _sy - 20);
    }
}

void CheckBox::handle(event ev)
{
    if (ev.type == ev_mouse &&
        ev.button == btn_left &&
        is_selected(ev.pos_x, ev.pos_y) &&
        is_clickable())
    {
        _check = !_check;
    }
}

bool CheckBox::is_checked() const
{
    return _check;
}

void CheckBox::set_checked(bool checked)
{
    _check = checked;
}
