#include "text.hpp"
#include "graphics.hpp"

using namespace genv;
using namespace std;

Text::Text(Lib* lib, int x, int y, int sx, int sy, int max_length)
    : Widget(lib, x, y, sx, sy), _text(""), _focused(false), _max_length(max_length)
{
    set_draggable(true);
}

void Text::draw() const
{
    gout << color(91, 104, 143) << move_to(_x, _y) << box(_sx, _sy)
         << color(_focused ? 49 : 38, _focused ? 60 : 43, _focused ? 92 : 64)
         << move_to(_x + 3, _y + 3) << box(_sx - 6, _sy - 6)
         << color(245, 247, 255) << move_to(_x + 8, _y + _sy / 2 -10) << genv::text(_text);

    if (_focused)
    {
        int cursor_x = _x + 8 + gout.twidth(_text);
        gout << color(255, 210, 95) << move_to(cursor_x, _y + 7) << line(0, _sy - 14);
    }
}

void Text::handle(event ev)
{
    if (!is_clickable()) return;
    if (ev.type == ev_mouse && ev.button == btn_left) _focused = is_selected(ev.pos_x, ev.pos_y);
    if (!_focused) return;

    if (ev.type == ev_key && ev.keycode == key_backspace)
    {
        if (!_text.empty()) _text.pop_back();
    }
    if (ev.type == ev_key && ev.keycode >= 32 && ev.keycode <= 255 && (int)_text.size() < _max_length)
    {
        _text += char(ev.keycode);
    }
}

string Text::text() const { return _text; }
void Text::set_text(const string& t) { _text = t; }
void Text::clear() { _text.clear(); }
bool Text::is_focused() const { return _focused; }
void Text::set_focused(bool focused) { _focused = focused; }
