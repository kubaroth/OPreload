#pragma once
#include <SOP/SOP_Node.h>

#define CR_HOST CR_UNSAFE
#include "cr.h"

const char *plugin = PLUGIN_DIR "/" CR_PLUGIN("guest_plugin");

struct Data{
    void * gdp;
    void * node;
};

namespace HDK_Sample {
    class SOP_DualStar : public SOP_Node
    {
    public:
        static OP_Node      *myConstructor(OP_Network*, const char *,
                                           OP_Operator *);

        /// Stores the description of the interface of the SOP in Houdini.
        /// Each parm template refers to a parameter.
        static PRM_Template          myTemplateList[];

    protected:
        SOP_DualStar(OP_Network *net, const char *name, OP_Operator *op);
        virtual ~SOP_DualStar();


        /// cookMySop does the actual work of the SOP computing, in this
        /// case, a star shape.
        virtual OP_ERROR         cookMySop(OP_Context &context);
        virtual GU_DetailHandle      cookMySopOutput(OP_Context &context, int outputidx, SOP_Node *interest);

        /// Actually build a star in the given geometry
        void    buildStar(GU_Detail *dst,
                          OP_Context &context);

    private:
        cr_plugin ctx;  //hotreload

        /// The following list of accessors simplify evaluating the parameters
        /// of the SOP.
        int         DIVISIONS(fpreal t)     { return evalInt  ("divs", 0, t); }
        fpreal  XRADIUS(fpreal t)   { return evalFloat("rad", 0, t); }
        fpreal  YRADIUS(fpreal t)   { return evalFloat("rad", 1, t); }
        int         NEGRADIUS()         { return evalInt  ("nradius", 0, 0); }
        fpreal  CENTERX(fpreal t)   { return evalFloat("t", 0, t); }
        fpreal  CENTERY(fpreal t)   { return evalFloat("t", 1, t); }
        fpreal  CENTERZ(fpreal t)   { return evalFloat("t", 2, t); }
        int         ORIENT()        { return evalInt  ("orient", 0, 0); }

    };
} // End HDK_Sample namespace
