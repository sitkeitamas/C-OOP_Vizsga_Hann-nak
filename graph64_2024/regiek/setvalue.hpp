#ifndef SETVALUE_HPP
#define SETVALUE_HPP

#include "widget.hpp"
#include "graphics.hpp"

class SetValue : public Widget
{
protected:
    int _value, _min, _max, _step;
public:
    SetValue(Window * w, int x, int y, int sx, int sy, int min, int max);
    virtual void draw() const ;
    virtual void handle(genv::event ev) ;
    int get_value() const;
    virtual void set_value(int value, int min, int max, int step=1);

};
#endif // SETVALUE_HPP
