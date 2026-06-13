#include "widget.hpp"
#include "window.hpp"
#include "graphics.hpp"

using namespace genv;

Widget::Widget(Window * w, int x, int y, int sx, int sy) :
    _window(w), _x(x), _y(y), _size_x(sx), _size_y(sy)
{
    if(_window)
    {
        _window->add_widget(this);
    }
}

bool Widget::is_selected(int mouse_x, int mouse_y)
{
    return mouse_x>_x && mouse_x<_x+_size_x &&
    mouse_y>_y && mouse_y<_y+_size_y;
}
