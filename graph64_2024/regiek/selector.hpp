#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include "graphics.hpp"
#include "widget.hpp"
#include "string"

class Selector : public Widget {
protected:
    std::vector<std::string> _options;
    int _selected_index;

public:
    Selector(Window* w, int x, int y, int sx, int sy, const std::vector<std::string>& options);
    void draw() const override;
    void handle(genv::event ev) override;

    int get_selected_index() const;
    std::string get_selected_value() const;

};

#endif // SELECTOR_HPP
