#include "widgets.hpp"
#include "application.hpp"

Widget::Widget(Lib *lib, int x, int y, int sx, int sy)
    : _x(x), _y(y), _sx(sx), _sy(sy), _lib(lib)
{
    if (_lib) _lib->register_widget(this);
}

bool Widget::is_selected(int mx, int my) const
{
    return mx >= _x && mx < _x + _sx && my >= _y && my < _y + _sy;
}

void Widget::set_selected(bool s) { _selected = s; }
bool Widget::is_selected_state() const { return _selected; }

void Widget::set_clickable(bool c) { _is_clickable = c; }
bool Widget::is_clickable() const { return _is_clickable; }

void Widget::set_draggable(bool d) { _is_draggable = d; }
bool Widget::is_draggable() const { return _is_draggable; }

void Widget::set_position(int x, int y)
{
    _x = x;
    _y = y;
}

int Widget::x() const { return _x; }
int Widget::y() const { return _y; }
int Widget::sx() const { return _sx; }
int Widget::sy() const { return _sy; }
