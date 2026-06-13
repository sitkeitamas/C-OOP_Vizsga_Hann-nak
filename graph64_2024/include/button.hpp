#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>
#include <string>
#include "widgets.hpp"

class Button: public Widget {
protected:
    std::string _title;
    std::function<void()> _f;
public:
    Button(Lib*, int, int, int, int, std::string, std::function<void()>);
    void draw() const override;
    void handle(genv::event) override;
    void action();
};

#endif
