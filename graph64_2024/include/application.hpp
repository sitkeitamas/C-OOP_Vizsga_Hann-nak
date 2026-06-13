#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "widgets.hpp"
#include <vector>

class Widget;

class Lib {
protected:
    std::vector<Widget*> _widgets;
    int _XX, _YY;

    Widget* _dragged_widget = nullptr;
    int _drag_offset_x = 0;
    int _drag_offset_y = 0;
    bool _ctrl_down = false;

public:
    Lib(int XX, int YY);

    virtual void event_loop();
    virtual void register_widget(Widget* widget);
};

#endif
