#include <iostream>

#include <SOP/SOP_Node.h>

#include "cr.h"

using std::cout;
using std::endl;

static int     g_failure = 0;
static unsigned int CR_STATE g_version = 0;
// static HostData     *g_data = nullptr; // hold user data kept on host and received from host

void cook(GU_Detail * gdp){
    cout <<"guess points" << gdp->getNumPoints() <<endl;

    // Uncomment, rebulid and disable/enable the SOP to add a point attribute at runtime.
    
    // GA_RWHandleV3 n_h(gdp, GA_ATTRIB_POINT, "N");
    // if (!n_h.isValid()){
    //     n_h = GA_RWHandleV3(gdp->addFloatTuple(GA_ATTRIB_POINT,  GEO_STD_ATTRIB_NORMAL, 3));
    // }

}

CR_EXPORT int cr_main(cr_plugin *ctx, cr_op operation) {
    // assert(ctx);
    // g_data = (HostData *)ctx->userdata;
    g_version = ctx->version;
    g_failure = ctx->failure;

    GU_Detail *gdp = (GU_Detail*)ctx->userdata;

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
            cook(gdp);
            return 0;
    }

    return 0;
}
