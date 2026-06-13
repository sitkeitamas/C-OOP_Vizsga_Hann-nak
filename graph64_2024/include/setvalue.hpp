#ifndef SETVALUE_HPP
#define SETVALUE_HPP

#include "widgets.hpp"

class SetValue : public Widget
{
protected:
    int _value;
    int _min;
    int _max;

public:
    SetValue(Lib* lib, int x, int y, int sx, int sy, int min, int max);

    void draw() const override;
    void handle(genv::event ev) override;

    void increase();
    void decrease();

    int value() const;
    void set_value(int value);
};

#endif
