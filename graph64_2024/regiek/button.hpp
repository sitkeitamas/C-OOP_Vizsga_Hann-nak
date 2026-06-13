#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "widget.hpp"
#include "graphics.hpp"
#include <functional>
#include <string>

using namespace std;

class Button : public Widget
{
protected:
    string label;
    function<void()> callback;
public:
    Button(Window * w, int x, int y, int sx, int sy, const std::string& lbl, std::function<void()> cb = nullptr);
    void draw() const /*override*/;
    virtual void handle(genv::event ev)/* override*/;
    virtual void setCallback(std::function<void()> cb);

};

#endif // BUTTON_HPP
