// Alapértelmezett belépési pont: REC/gép listakezelés.
// Más demó: main_icecream.cpp (fagyi), main_sync.cpp (F_sync váz).
#include "rec_gep_app.hpp"

int main()
{
    RecGepApp app(900, 680);
    app.event_loop();
    return 0;
}
