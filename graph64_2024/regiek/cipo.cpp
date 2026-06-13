#include "cipo.hpp"
#include <iostream>


Cipo::Cipo(Window* w, int x, int y, int sx, int sy) : Widget(w, x, y, sx, sy)
{
    nev = new Text(w, 40, 70, 100, 30,std::string (""));
    meret = new SetValue(w, 180, 70, 100, 30, 35, 50);
    hozzaad = new Button( w, 40, 130, 240, 30, "Hozzaadas", [this]() { add_cipo(); });
    torol = new Button(w, 40, 170, 240, 30, "Torol", [this]() { delete_cipo(); });
    raktar = new Display(w, 40, 220, 240, 150);
    vector<Cipok> cipok;
    widgets = {nev, meret, hozzaad, torol, raktar};

}

void Cipo::draw() const
{
    for (Widget* w : widgets) {
        w->draw();
    }
}

void Cipo::handle(genv::event ev)
{
    for (Widget* w : widgets) {
        w->handle(ev);
    }
}

void Cipo::add_cipo()
{
    std::string nev_input = nev->get_text();
    int meret_input = meret->get_value();
    if (nev_input.empty()) {
        std::string message = "Név nem lehet üres!";
        show_warning(message);
        return;
    }

    auto it = std::find_if(cipok.begin(), cipok.end(), [&](const Cipok& c) {
        return c.nev == nev_input;
    });

    if (it != cipok.end()) {
        if (std::find(it->meret.begin(), it->meret.end(), meret_input) == it->meret.end()) {
            it->meret.push_back(meret_input);
            std::sort(it->meret.begin(), it->meret.end());
        }
    } else {
        Cipok uj;
        uj.nev = nev_input;
        uj.meret.push_back(meret_input);
        cipok.push_back(uj);
    }

    lista_frissites();
}

void Cipo::delete_cipo()
{
    int index = raktar->get_selected_index();
    if (index >= 0 && index < cipok.size())
    {
        cipok.erase(cipok.begin() + index);
        lista_frissites();
    }
    else
    {
        std::string message = "Nincs kijelölt cipõ!";
        show_warning(message);
    }
}


void Cipo::show_warning(std::string& message)
{
    gout << move_to(10, 10) << color(255, 0, 0) << text(message);
    gout << refresh;
}

void Cipo::lista_frissites()
{
    std::vector<std::string> lines;

    for (const Cipok& c : cipok)
    {
        std::stringstream ss;
        ss << c.nev << " : ";
        for (size_t i = 0; i < c.meret.size(); ++i)
        {
            ss << c.meret[i];
            if (i != c.meret.size() - 1)
            {
                ss << ", ";
            }
        }
        lines.push_back(ss.str());
    }
    raktar->set_lines(lines);
}
