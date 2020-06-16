#include <iostream>

#include <SOP/SOP_Node.h>

#include "cr.h"

using std::cout;
using std::endl;

static int     g_failure = 0;
static unsigned int CR_STATE g_version = 0;
// static HostData     *g_data = nullptr; // hold user data kept on host and received from host

int add(int a){
    cout << "plugin..."<<endl;

    return a + 1;
}

CR_EXPORT int cr_main(cr_plugin *ctx, cr_op operation) {
    // assert(ctx);
    // g_data = (HostData *)ctx->userdata;
    g_version = ctx->version;
    g_failure = ctx->failure;

    // GU_Detail &gdp = ctx->userdata->gdp;
    int a=0;
    switch (operation) {
        case CR_LOAD:
            // some action
            return 0;
        case CR_UNLOAD:
            // if needed, save stuff to pass over to next instance
            return 0;
        case CR_CLOSE:
            // some action
            return 0;
        case CR_STEP:
            add(1);
            return 0;
    }

    return 0;
}
