#ifndef CIPO_HPP
#define CIPO_HPP

#include "widget.hpp"
#include "text.hpp"
#include "statictext.hpp"
#include "button.hpp"
#include "setvalue.hpp"
#include "selector.hpp"
#include "display.hpp"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace genv;

struct Cipok
{
    string nev;
    vector<int> meret;
};

class Cipo : public Widget
{
protected:
    Text* nev;
    SetValue* meret;
    Button* hozzaad;
    Button* torol;
    Display* raktar;
    vector<Cipok> cipok;
    std::vector<Widget*> widgets;

public:
    Cipo(Window* w, int x, int y, int sx, int sy);
    virtual void draw() const override;
    virtual void handle(genv::event ev) override;
    void add_cipo();
    void delete_cipo();
    void show_warning(std::string& message);
    virtual void lista_frissites();
};

#endif // CIPO_HPP


/*

text* nev
setvalue* meret
button* hozzáad
button* töröl
display* raktar
vector<Cipok> cipok
std::vector<Widget*> widgets;

draw() const override
handle(genv::event ev) override
add_cipo()
set_cipo(const std::vector<Cipok>& cipok)
show_warning(const std::string& message)
lista_frissit()

*/
