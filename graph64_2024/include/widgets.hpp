#ifndef WIDGETS_HPP_INCLUDED
#define WIDGETS_HPP_INCLUDED

#include "graphics.hpp"

class Lib;

class Widget {
protected:
    int _x = 0, _y = 0, _sx = 0, _sy = 0;
    Lib *_lib = nullptr;

    bool _selected = false;
    bool _is_clickable = true;
    bool _is_draggable = false;

public:
    Widget(Lib* lib, int x, int y, int sx, int sy);

    virtual bool is_selected(int mouse_x, int mouse_y) const;
    virtual void draw() const = 0;
    virtual void handle(genv::event ev) = 0;

    void set_selected(bool s);
    bool is_selected_state() const;

    void set_clickable(bool c);
    bool is_clickable() const;

    void set_draggable(bool d);
    bool is_draggable() const;

    void set_position(int x, int y);

    int x() const;
    int y() const;
    int sx() const;
    int sy() const;
};

#endif
