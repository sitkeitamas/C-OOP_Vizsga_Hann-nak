#include "szinkezelo.hpp"
#include <string>

Szinkezelo::Szinkezelo(Window* w, int x, int y, int sx, int sy) : Widget(w, x, y, sx, sy)
{
    nev = new Text(w, 40, 70, 100, 30,std::string (""));
    r1 = new SetValue(w, 180, 70, 100, 30, 35, 50);
    g1 = new SetValue(w, 180, 70, 100, 30, 35, 50);
    b1 = new SetValue(w, 180, 70, 100, 30, 35, 50);
    hozzaad = new Button( w, 40, 130, 240, 30, "Hozzaad", [this]() { add_szin(); });
    alkalmaz = new Button( w, 40, 130, 240, 30, "Alkalmaz", [this]() { use(); });
    torol1 = new Button(w, 40, 170, 240, 30, "Torol", [this]() { delete_szin_eloter(); });
    torol2 = new Button(w, 40, 170, 240, 30, "Torol", [this]() { delete_szin_hatter(); });
    eloter = new Display(w, 40, 220, 240, 150);
    hatter = new Display(w, 40, 220, 240, 150);
    szoveg = new StaticText(w, 40, 380, "Szinkezelo");

    widgets = {nev, r1, g1, b1, hozzaad, alkalmaz, torol1, torol2, eloter, hatter, szoveg};
}

void Szinkezelo::draw() const
{
    for (Widget* w : widgets)
    {
        w->draw();
    }
}
void Szinkezelo::handle(genv::event ev)
{
    for (Widget* w : widgets)
    {
        w->handle(ev);
    }
}
void Szinkezelo::add_szin()
{
    std::string nev_input = nev->get_text();
    int r1_input = r1->get_value();
    int g1_input = g1->get_value();
    int b1_input = b1->get_value();

    string r_input = to_string(r1_input);
    string g_input = to_string(g1_input);
    string b_input = to_string(b1_input);

    if (nev_input.empty()) {
        std::string message = "Név nem lehet üres!";
        show_warning(message);
        return;
    }

    auto it = std::find_if(szin.begin(), szin.end(), [&](const Szin& s) {
        return s.nev == nev_input;
    });

    Szin uj;
    uj.nev = nev_input;
    uj.r = r_input;
    uj.g = g_input;
    uj.b = b_input;
    szin.push_back(uj);

    lista_frissites_eloter();
    lista_frissites_hatter();

}

// - r, g, b ertekei (stringként) + nev -> hozzaad listahoz (csak a név léatszik)

void Szinkezelo::delete_szin_eloter()
{
    int index = eloter->get_selected_index();
    if (index >= 0 && index < szin.size())
    {
        szin.erase(szin.begin() + index);
        lista_frissites_eloter();

    }
    else
    {
        std::string message = "Nincs kijelölt cipõ!";
        show_warning(message);
    }
}

void Szinkezelo::delete_szin_hatter()
{
    int index = hatter->get_selected_index();
    if (index >= 0 && index < szin.size())
    {
        szin.erase(szin.begin() + index);
        lista_frissites_hatter();
    }
    else
    {
        std::string message = "Nincs kijelölt cipõ!";
        show_warning(message);
    }
}

void Szinkezelo::use()
{
    int index1 = eloter->get_selected_index();
    int index2 = hatter->get_selected_index();

    //szoveg->text_color = (r.eloter[index1], g.eloter[index1], b.eloter[index1]);
    //szoveg->background_color = (r.hatter[index2], g.hatter[index2], b.hatter[index2]);

    szoveg->setColor(text, background)

}
// - lista indexet lekerni -> tudjuk hanyadik elem -> annak az elemnek az r, g, b-je => text r, g, b-je

void Szinkezelo::show_warning(std::string& message)
{
    gout << move_to(10, 10) << color(255, 0, 0) << text(message);
    gout << refresh;
}

void Szinkezelo::lista_frissites_eloter()
{
    std::vector<std::string> lines;

    for (const Szin& s : szin)
    {
        std::stringstream ss;
        ss << s.nev << " , ";

        lines.push_back(ss.str());
    }
    eloter->set_lines(lines);
}

void Szinkezelo::lista_frissites_hatter()
{
    std::vector<std::string> lines;

    for (const Szin& s : szin)
    {
        std::stringstream ss;
        ss << s.nev << " , ";

        lines.push_back(ss.str());
    }
    hatter->set_lines(lines);
}
