#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "widgets.hpp"

class CheckBox: public Widget
{
protected:
    bool _check;
public:
    CheckBox(Lib*, int, int, int, int);

    virtual void draw() const override;
    virtual void handle(genv::event) override;

    bool is_checked() const;
    void set_checked(bool checked);
};

#endif // CHECKBOX_HPP
