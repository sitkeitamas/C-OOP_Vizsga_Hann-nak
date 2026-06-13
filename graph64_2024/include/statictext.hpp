#ifndef STATICTEXT_HPP
#define STATICTEXT_HPP

#include "widgets.hpp"
#include <string>

struct RGB
{
    int r, g, b;
};

class StaticText : public Widget
{
protected:
    std::string _text;

    RGB _text_color;
    RGB _box_color;

    RGB _default_text_color;
    RGB _default_box_color;

    bool _visible;
    int _watched_value;
    int _limit;

public:
    StaticText(Lib* lib, int x, int y, int sx, int sy,
               std::string text,
               RGB text_color = {255,255,255},
               RGB box_color = {0,0,0});

    void draw() const override;
    void handle(genv::event ev) override;

    void set_text(std::string text);

    void set_visible(bool visible);
    bool is_visible() const;

    void set_colors(RGB text_color, RGB box_color);
    void reset_colors();

    void show_if_below_limit(int value, int limit);
    void show_if_above_limit(int value, int limit);
};

#endif
