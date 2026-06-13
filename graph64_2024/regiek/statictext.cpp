#include "statictext.hpp"
#include "graphics.hpp"

using namespace std;
using namespace genv;

StaticText::StaticText(Window * w, int x, int y, const std::string& writing)
: Widget(w, x, y, 50, 30), writing(writing)
{

}

void StaticText::draw() const
{
    if (!visible) return;

    gout << move_to(_x, _y) << background_color << box(_size_x, _size_y);
    gout << move_to(_x + 5, _y + 15) << text_color << text(writing);
}
void StaticText::handle(event ev)
{

}

void StaticText::set_text(const std::string& writing)
{
    this->writing = writing;
}

void StaticText::setColor(const genv::color& text, const genv::color& background)
{
    text_color = text;
    background_color = background;
}

void StaticText::setVisibility(bool v)
{
    visible = v;
}

void StaticText::setPosition(int x, int y)
{
    _x = x;
    _y = y;
}

void StaticText::setSize(int width, int height)
{
    _size_x = width;
    _size_y = height;
}
