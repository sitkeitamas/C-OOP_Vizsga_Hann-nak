#include "window.hpp"

int main()
{
    Window window(800, 600);
    window.event_loop();
    return 0;
}
