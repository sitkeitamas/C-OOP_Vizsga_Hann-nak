#include "display.hpp"
#include "graphics.hpp"

using namespace genv;

Display::Display(Window * w, int x, int y, int sx, int sy) :
    Widget(w, x, y, sx, sy)
{

}

void Display::draw() const
{
    gout << move_to(_x, _y) << color(100, 100, 100) << box(_size_x, _size_y); // border
gout << move_to(_x + 1, _y + 1) << background_color << box(_size_x - 2, _size_y - 2);


    int y_offset = 22;
    int max_lines = _size_y / y_offset;

    size_t line_limit = std::min(_lines.size(), static_cast<size_t>(scroll_offset + max_lines));


    for (size_t i = scroll_offset; i < line_limit; ++i)
    {
        int y_pos = _y - 10 + y_offset * (i - scroll_offset) + y_offset;

        if ((int)i == selected_index)
        {
            gout << move_to(_x + 1, y_pos + 15 - y_offset + 1)
                 << color(200, 200, 255)
                 << box(_size_x - 2, y_offset - 2);
        }

        gout << move_to(_x + 10, y_pos)
     << text_color
     << text(_lines[i]);

    }
    gout << color(150, 150, 150);

    if (scroll_offset > 0)
    {
        gout << move_to(_x + _size_x - 15, _y + 5)
             << text("^");
    }

    if (scroll_offset + max_lines < (int)_lines.size())
    {
        gout << move_to(_x + _size_x - 15, _y + _size_y - 10)
             << text("v");
    }
}

void Display::handle(event ev)
{
    if (ev.type == ev_mouse && is_selected(ev.pos_x, ev.pos_y))
    {
        int y_offset = 22;
        int visible_lines = _size_y / y_offset;

        if (ev.button == btn_wheelup)
        {
            if (scroll_offset > 0)
                scroll_offset--;
        }
        else if (ev.button == btn_wheeldown)
        {
            if ((int)_lines.size() > visible_lines && scroll_offset < (int)_lines.size() - visible_lines)
                scroll_offset++;
        }
        else if (ev.button == btn_left)
        {
            int relative_y = ev.pos_y - _y;
            int clicked_index = scroll_offset + (relative_y / y_offset);

            if (clicked_index >= 0 && clicked_index < (int)_lines.size())
            {
                selected_index = clicked_index;
            }
        }
    }
}

bool Display::is_selected(int mx, int my) const
{
    return (mx > _x && mx < _x + _size_x &&
             my > _y && my < _y + _size_y);
}


void Display::set_lines(const std::vector<std::string>& lines)
{
    _lines = lines;
    scroll_offset = 0;
    if (selected_index >= (int)_lines.size())
    {
        selected_index = -1;
    }
}

int Display::get_selected_index() const
{
    return selected_index;
}

void Display::setTextColor(const genv::color& c)
{
    text_color = c;
}

void Display::setBackgroundColor(const genv::color& c)
{
    background_color = c;
}

void Display::setPosition(int x, int y)
{
    _x = x;
    _y = y;
}

void Display::setSize(int width, int height)
{
    _size_x = width;
    _size_y = height;
}
