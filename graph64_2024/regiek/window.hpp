#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "widget.hpp"
#include "text.hpp"
#include "statictext.hpp"
#include "button.hpp"
#include "setvalue.hpp"
#include "selector.hpp"
#include "display.hpp"

#include <vector>
#include <string>
#include <map>

class Window
{
    int _x, _y;
    std::map<std::string, int> storage;
    int height, width;

public:

    std::vector<Widget *> widget;

    Window(int X, int Y);
    void add_widget(Widget * w);
    void event_loop();
};

#endif // WINDOW_HPP
