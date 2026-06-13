#include "statictext.hpp"
#include "graphics.hpp"

using namespace genv;
using namespace std;

StaticText::StaticText(Lib* lib, int x, int y, int sx, int sy,string text, RGB text_color, RGB box_color)
    : Widget(lib, x, y, sx, sy),
      _text(text), _text_color(text_color), _box_color(box_color),
      _default_text_color(text_color), _default_box_color(box_color),
      _visible(true), _watched_value(0), _limit(0)
{
    set_draggable(true);
}

void StaticText::draw() const
{
    if (!_visible) return;

    gout << color(_box_color.r, _box_color.g, _box_color.b) << move_to(_x, _y) << box(_sx, _sy)
         << color(_box_color.r + 18 > 255 ? 255 : _box_color.r + 18,
                  _box_color.g + 18 > 255 ? 255 : _box_color.g + 18,
                  _box_color.b + 18 > 255 ? 255 : _box_color.b + 18)
         << move_to(_x + 2, _y + 2) << box(_sx - 4, _sy - 4)
         << color(_text_color.r, _text_color.g, _text_color.b)
         << move_to(_x + 8, _y + _sy / 2 - 10) << text(_text);
}

void StaticText::handle(event) {}
void StaticText::set_text(string text) { _text = text; }
void StaticText::set_visible(bool visible) { _visible = visible; }
bool StaticText::is_visible() const { return _visible; }
void StaticText::set_colors(RGB text_color, RGB box_color) { _text_color = text_color; _box_color = box_color; }
void StaticText::reset_colors() { _text_color = _default_text_color; _box_color = _default_box_color; }
void StaticText::show_if_below_limit(int value, int limit) { _watched_value = value; _limit = limit; _visible = value < limit; }
void StaticText::show_if_above_limit(int value, int limit) { _watched_value = value; _limit = limit; _visible = value > limit; }
