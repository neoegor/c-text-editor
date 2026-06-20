#include "app.h"

int main(int argc, const char* argv[]) {
    App app;
    app_init(&app, argc, argv);
    app_run(&app);
    app_free(&app);

    return 0;
}
