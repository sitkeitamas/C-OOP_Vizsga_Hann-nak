#ifndef TEXT_HPP
#define TEXT_HPP


#include "widget.hpp"

class Text : public Widget
{
protected:
    bool _clicked;
    std::string _text;
public:
    Text(Window * w, int x, int y, int sx, int sy, const std::string& text);
    virtual bool is_clicked(int x, int y) const {
    return x >= _x && x <= _x + _size_x &&
           y >= _y && y <= _y + _size_y;};
    virtual void draw() const;
    virtual void handle(genv::event ev);
    const std::string& get_text() const;
};

#endif // TEXT_HPP
