#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "graphics.hpp"

class Window;
class Widget
{
protected:
    Window * _window;
    int _x, _y, _size_x, _size_y;

public:
    Widget(Window * w, int x, int y, int sx, int sy);
     bool is_selected(int mouse_x, int mouse_y);
    virtual void draw() const =0;
    virtual void handle(genv::event ev) = 0;
};
#endif // WIDGET_HPP
