#include "window.hpp"
#include "graphics.hpp"


using namespace std;
using namespace genv;

Window::Window(int X, int Y) : _x(X), _y(Y)
{
    gout.open(X, Y);
    gout << font("LiberationSans-Regular.ttf", 12);
}



void Window::add_widget(Widget * w)
{
    widget.push_back(w);
}

void Window::event_loop()
{
    event ev;
    while (gin >> ev and ev.button != key_escape)
    {
        gout << color(255, 255, 255) << move_to(0, 0) << box(_x, _y); // clear

        if (!widget.empty())
        {
            widget[0]->handle(ev);
            widget[0]->draw();
        }

        gout << refresh;
    }
}
