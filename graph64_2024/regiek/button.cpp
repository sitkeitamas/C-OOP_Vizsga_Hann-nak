#include "button.hpp"
#include "graphics.hpp"

using namespace std;
using namespace genv;

Button::Button(Window* w, int x, int y, int sx, int sy, const std::string& lbl, std::function<void()> cb)
    : Widget(w, x, y, sx, sy), label(lbl), callback(cb)
{
}

void Button::setCallback(std::function<void()> cb) {
    callback = cb;
}

void Button::draw() const{
    gout << move_to(_x, _y) << color(128, 128, 128) << box(_size_x, _size_y);
    gout << move_to(_x + 5, _y + _size_y / 4) << color(255, 255, 255) << text(label);
}

void Button::handle(event ev)
{
    if (ev.type == ev_mouse && ev.button == btn_left)
    {
        if(is_selected(ev.pos_x, ev.pos_y))
        {
            if (callback) {
                callback();
            }
            /*if (label != "Hozzaad" && label != "Modosit")
                {
                if (label == "Gomb" ) {
                    label = "Nyomva";
                } else {
                    label = "Gomb";
                }

            }*/
            draw();
                gout << refresh;
        }
    }
}
