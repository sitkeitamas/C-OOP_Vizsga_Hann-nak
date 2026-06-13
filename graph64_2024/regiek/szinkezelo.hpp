#ifndef SZINKEZELO_HPP
#define SZINKEZELO_HPP

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

struct Szin
{
    string r;
    string g;
    string b;
    string nev;
};

class Szinkezelo : public Widget
{
protected:
    Text* nev;
    SetValue* r1;
    SetValue* g1;
    SetValue* b1;
    Button* hozzaad;
    Button* alkalmaz;
    Button* torol1;
    Button* torol2;
    Display* eloter;
    Display* hatter;
    StaticText* szoveg;

    vector<Szin> szin;
    std::vector<Widget*> widgets;

public:
    Szinkezelo(Window* w, int x, int y, int sx, int sy);
    virtual void draw() const ;
    virtual void handle(genv::event ev) ;
    void add_szin();
    void delete_szin_eloter();
    void delete_szin_hatter();
    void use();
    void show_warning(std::string& message);
    virtual void lista_frissites_eloter();
    virtual void lista_frissites_hatter();


};

#endif // SZINKEZELO_HPP
