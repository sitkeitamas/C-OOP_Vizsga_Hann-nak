#include "application.hpp"
#include "dropdown.hpp"      // open-dropdown prioritás dispatch-hez
#include "graphics.hpp"
#include "widgets.hpp"
#include "text.hpp"

using namespace genv;

// ============================================================
// APPLICATION.CPP – JAVÍTOTT VERZIÓ
//
// Fő javítás: nyitott Dropdown prioritást kap minden egéreset
// előtt. Ha a Dropdown nyitva van és máshova kattintasz, a
// Dropdown handleje fut le (bezárja magát) és más widget NEM
// kap eseményt. Ez a valós dropdown viselkedés.
// ============================================================

Lib::Lib(int XX, int YY)
    : _XX(XX), _YY(YY)
{
    gout.open(XX, YY);
}

void Lib::register_widget(Widget *widget)
{
    _widgets.push_back(widget);
}

void Lib::event_loop()
{
    // Első kirajzolás
    gout << color(26, 28, 44) << move_to(0, 0) << box(_XX, _YY);
    for (Widget *w : _widgets) w->draw();
    gout << refresh;

    event ev;
    while (gin >> ev)
    {
        // ===== CTRL ÁLLAPOT =====
        if (ev.type == ev_key)
        {
            if (ev.keycode == key_lctrl || ev.keycode == key_rctrl)    _ctrl_down = true;
            if (ev.keycode == -key_lctrl || ev.keycode == -key_rctrl)  _ctrl_down = false;
        }

        // ===== CTRL + EGÉR: HÚZÁS =====
        if (ev.type == ev_mouse && ev.button == btn_left && _ctrl_down)
        {
            for (int i = (int)_widgets.size() - 1; i >= 0; --i)
            {
                Widget *w = _widgets[i];
                // Nyitott dropdown ne indítson húzást – előbb zárjon be
                Dropdown *dd = dynamic_cast<Dropdown*>(w);
                if (dd && dd->is_opened()) continue;

                if (w->is_selected(ev.pos_x, ev.pos_y) && w->is_draggable())
                {
                    _dragged_widget = w;
                    _drag_offset_x  = ev.pos_x - w->x();
                    _drag_offset_y  = ev.pos_y - w->y();
                    break;
                }
            }
        }

        if (ev.type == ev_mouse && _dragged_widget != nullptr && _ctrl_down)
        {
            _dragged_widget->set_position(ev.pos_x - _drag_offset_x,
                                          ev.pos_y - _drag_offset_y);
        }

        if ((ev.type == ev_mouse && ev.button == -btn_left) || !_ctrl_down)
        {
            _dragged_widget = nullptr;
        }

        // ===== HÁTTÉR TÖRLÉSE =====
        gout << color(26, 28, 44) << move_to(0, 0) << box(_XX, _YY);

        if (_dragged_widget == nullptr)
        {
            if (ev.type == ev_mouse && ev.button == btn_left)
            {
                for (Widget *w : _widgets)
                {
                    Text *t = dynamic_cast<Text*>(w);
                    if (t) t->set_focused(false);
                }
            }

            bool handled = false;

            // ===== FIX: NYITOTT DROPDOWN PRIORITÁS =====
            // Ha van nyitott dropdown, az kap MINDEN egéreseményt –
            // más widget NEM kap eseményt amíg a dropdown nyitva van.
            // A dropdown handleje bezárja magát ha máshova kattintanak.
            if (ev.type == ev_mouse)
            {
                for (Widget *w : _widgets)
                {
                    Dropdown *dd = dynamic_cast<Dropdown*>(w);
                    if (dd && dd->is_opened() && dd->is_clickable())
                    {
                        dd->handle(ev);
                        handled = true;
                        break;
                    }
                }
            }

            // ===== NORMÁL DISPATCH (ha nincs nyitott dropdown) =====
            if (!handled)
            {
                for (int i = (int)_widgets.size() - 1; i >= 0; --i)
                {
                    Widget *w = _widgets[i];
                    if (ev.type == ev_mouse)
                    {
                        if (w->is_selected(ev.pos_x, ev.pos_y) && w->is_clickable())
                        {
                            w->handle(ev);
                            break;
                        }
                    }
                    else
                    {
                        w->handle(ev);
                    }
                }
            }
        }

        // ===== ÚJRARAJZOLÁS =====
        for (Widget *w : _widgets) w->draw();
        gout << refresh;
    }
}
