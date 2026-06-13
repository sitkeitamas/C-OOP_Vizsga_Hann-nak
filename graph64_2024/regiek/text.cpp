#include "text.hpp"
#include "graphics.hpp"

using namespace genv;

Text::Text(Window* w, int x, int y, int sx, int sy, const std::string& text)
: Widget(w, x, y, sx, sy), _clicked(false), _text(text) {}

void Text::draw() const
{
    gout << move_to(_x, _y) << color(100, 100, 100) << box(_size_x, _size_y);
    gout << move_to(_x + 1, _y + 1) << color(255, 255, 255) << box(_size_x - 2, _size_y - 2);

    gout << move_to(_x + 8, _y + gout.cdescent() + (_size_y - gout.cascent())/ 3) << color(0, 0, 0) << text(_text);
}

void Text::handle(event ev)
{
    if (ev.type == ev_mouse &&
        ev.button == btn_left)
    {
        if (is_clicked(ev.pos_x, ev.pos_y))
        {
            _clicked = true;
        }
        else
        {
            _clicked = false;
        }
    }
    else if (ev.type == ev_key && _clicked)
    {
        if (ev.keycode == key_backspace && !_text.empty())
        {
            _text.pop_back();
        }
        else if (ev.keycode >= ' ' && ev.keycode <= 255)
        {
            _text.push_back(ev.keycode);
        }
    }
}


const std::string& Text::get_text() const
{
    return _text;
}
