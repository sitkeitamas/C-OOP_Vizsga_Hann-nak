#include "sync_app.hpp"

int main()
{
    SyncApp app(800, 600);
    app.event_loop();
    return 0;
}
