#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>
#include <vector>
#include "widget.hpp"
#include "button.hpp"

using namespace std;

class Display : public Widget
{
protected:
    std::vector<std::string> _lines;
    int selected_index = -1;
    int scroll_offset = 0;
    genv::color text_color = genv::color(0, 0, 0);
    genv::color background_color = genv::color(255, 255, 255);

public:
    Display(Window* w, int x, int y, int sx, int sy);
    void draw() const override;
    void handle(genv::event ev) override;
    bool is_selected(int mx, int my) const;
    void set_lines(const std::vector<std::string>& lines);
    int get_selected_index() const;
    void setTextColor(const genv::color& c);
    void setBackgroundColor(const genv::color& c);
    void setPosition(int x, int y);
    void setSize(int width, int height);
};

#endif // DISPLAY_HPP
