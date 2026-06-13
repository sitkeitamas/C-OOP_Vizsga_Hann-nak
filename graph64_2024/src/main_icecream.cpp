#include "icecream_app.hpp"

int main()
{
    IceCreamApp app(900, 320);
    app.event_loop();
    return 0;
}
