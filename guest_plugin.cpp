#include <iostream>

#include <SOP/SOP_Node.h>
#include <OP/OP_Director.h>

// #include "star.hpp"

#include "cr.h"

using std::cout;
using std::endl;

struct Data{
    void * director;
    void * op;
};

static int     g_failure = 0;
static unsigned int CR_STATE g_version = 0;
// static HostData     *g_data = nullptr; // hold user data kept on host and received from host

CR_EXPORT int cr_main(cr_plugin *ctx, cr_op operation) {
    // assert(ctx);
    // g_data = (HostData *)ctx->userdata;
    g_version = ctx->version;
    g_failure = ctx->failure;

    Data * data = (Data*)ctx->userdata;
    OP_Director *dir = (OP_Director*)data->director;
    OP_Operator *op = (OP_Operator*)data->op;
    
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
            cout << "plugin operator: " << op << endl;

            ///// not working
            // OP_Operator * opp = new OP_Operator(
            //     "hdk_dualstar",                 // Internal name
            //     "Dual Star",                     // UI name
            //     SOP_DualStar::myConstructor,    // How to build the SOP
            //     SOP_DualStar::myTemplateList,   // My parameters
            //     1,                          // Min # of sources
            //     1,                          // Max # of sources
            //     0,      // Local variables
            //     OP_FLAG_GENERATOR,        // Flag it as generator
            //     0,  // labels
            //     2);    // Outputs.
            return 0;
    }

    return 0;
}
