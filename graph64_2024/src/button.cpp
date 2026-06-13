#include "button.hpp"
#include "graphics.hpp"

using namespace std;
using namespace genv;

Button::Button(Lib *lib, int x, int y, int sx, int sy, string title, function<void()> f)
    : Widget(lib, x, y, sx, sy), _title(title), _f(f)
{
    set_draggable(true);
}

void Button::draw() const
{
    gout.load_font("LiberationSans-Regular.ttf");
    gout << color(28, 156, 190) << move_to(_x, _y) << box(_sx, _sy)
         << color(84, 205, 220) << move_to(_x + 3, _y + 3) << box(_sx - 6, _sy - 6)
         << color(22, 45, 67) << move_to(_x + 6, _y + 6) << box(_sx - 12, _sy - 12)
         << color(245, 250, 255)
         << move_to((_x + _sx / 2) - gout.twidth(_title) / 2,
                    (_y + _sy / 2) - (gout.cascent() + gout.cdescent()) / 2)
         << text(_title);
}

void Button::handle(event ev)
{
    if (ev.type == ev_mouse && ev.button == btn_left && is_selected(ev.pos_x, ev.pos_y) && is_clickable()) action();
}

void Button::action()
{
    if (_f) _f();
}
