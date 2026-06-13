#ifndef STATICTEXT_HPP
#define STATICTEXT_HPP

#include "widget.hpp"
#include "graphics.hpp"
#include <map>
#include <string>

class StaticText : public Widget
{
protected:
    std::string writing;
    genv::color text_color = genv::color(0, 0, 0);
    genv::color background_color = genv::color(255, 255, 255);
    bool visible = true;
public:
    StaticText(Window * w, int x, int y, const std::string& writing);
    virtual void draw() const;
    virtual void handle(genv::event ev);
    void set_text(const std::string& writing);
    void setColor(const genv::color& text, const genv::color& background);
    void setVisibility(bool v);
    void setPosition(int x, int y);
    void setSize(int width, int height);
};

#endif // STATICTEXT_HPP
