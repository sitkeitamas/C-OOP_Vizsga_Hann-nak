#ifndef TEXT_HPP
#define TEXT_HPP

#include "widgets.hpp"
#include <string>

class Text : public Widget
{
protected:
    std::string _text;
    bool _focused;
    int _max_length;

public:
    Text(Lib* lib, int x, int y, int sx, int sy, int max_length = 30);

    void draw() const override;
    void handle(genv::event ev) override;

    std::string text() const;
    void set_text(const std::string& t);
    void clear();

    bool is_focused() const;
    void set_focused(bool focused);
};

#endif
