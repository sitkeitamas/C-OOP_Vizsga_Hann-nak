// Bevihető MVC keret – vizsgán: config.hpp + esetleg row_text / model szabályok.
#include "app.hpp"

int main()
{
    App app(900, 660);
    app.event_loop();
    return 0;
}
