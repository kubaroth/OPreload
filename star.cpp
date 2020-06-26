#include "star.hpp"

#include <GU/GU_Detail.h>
#include <GEO/GEO_PrimPoly.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <OP/OP_AutoLockInputs.h>
#include <PRM/PRM_Include.h>
#include <CH/CH_LocalVariable.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Interrupt.h>
#include <SYS/SYS_Math.h>
#include <limits.h>
#include <OP/OP_Director.h>

#include <UT/UT_Error.h>

#include "reloader.hpp"

using std::cout;
using std::endl;


PRM_Name     negativeName("nradius", "Negative Radius");
PRM_Default  fiveDefault(6);     // Default to 5 divisions
PRM_Default  radiiDefaults[] = {
    PRM_Default(1),      // Outside radius
    PRM_Default(0.3)     // Inside radius
};


PRM_Template
SOP_DualStar::myTemplateList[] = {
    PRM_Template(PRM_INT,                       // Integer parameter.
                 PRM_Template::PRM_EXPORT_TBX,  // Export to top of viewer
                                                // when user selects this node
                 1,                  // One integer in this row/parameter
                 &PRMdivName,        // Name of this parameter - must be static
                 &fiveDefault,       // Default for this parameter - ditto
                 0,                  // Menu for this parameter
                 &PRMdivision2Range  // Valid range
        ),
    PRM_Template(PRM_XYZ,    2, &PRMradiusName, radiiDefaults),
    PRM_Template(PRM_TOGGLE, 1, &negativeName),
    PRM_Template(PRM_XYZ,    3, &PRMcenterName),
    PRM_Template(PRM_ORD,    1, &PRMorientName, 0, &PRMplaneMenu),
    PRM_Template()
};

Parms SOP_DualStar::parms;

OP_Node *
SOP_DualStar::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new SOP_DualStar(net, name, op);
}

SOP_DualStar::SOP_DualStar(OP_Network *net, const char *name, OP_Operator *op)
    : SOP_Node(net, name, op)
{
    // We do not manage our ids, so do not set the flag.
    // ctx.userdata = &data;
    cout << "Main Ctor" <<endl;
}

SOP_DualStar::~SOP_DualStar() {
}

OP_ERROR
SOP_DualStar::cookMySop(OP_Context &context)
{
    
    
    // We must lock our inputs before we try to access their geometry.
    // OP_AutoLockInputs will automatically unlock our inputs when we return.
    // NOTE: Don't call unlockInputs yourself when using this!
    OP_AutoLockInputs inputs(this);
    if (inputs.lock(context) >= UT_ERROR_ABORT)
        return error();

    duplicateSource(0, context);

    using std::cout;
    using std::endl;

    cout << "Update: @@@@@g_table cook " << g_table <<endl;

    // GA_RWHandleV3 n_h(gdp, GA_ATTRIB_POINT, "N");
    // if (!n_h.isValid()){
    //     n_h = GA_RWHandleV3(gdp->addFloatTuple(GA_ATTRIB_POINT,  GEO_STD_ATTRIB_NORMAL, 3));
    // }
     
        
    // Start the interrupt server
    buildStar(gdp, context);

    return error();
}

void
SOP_DualStar::buildStar(GU_Detail *dst, OP_Context &context)
{
    fpreal now = context.getTime();
    int divisions  = DIVISIONS(now)*2;  // We need twice our divisions of points

    int plane     = ORIENT();
    int negradius = NEGRADIUS();
    float tx      = CENTERX(now);
    float ty      = CENTERY(now);
    float tz      = CENTERZ(now);

    int xcoord, ycoord, zcoord;
    switch (plane)
        {
        case 0:         // XY Plane
            xcoord = 0;
            ycoord = 1;
            zcoord = 2;
            break;
        case 1:         // YZ Plane
            xcoord = 1;
            ycoord = 2;
            zcoord = 0;
            break;
        case 2:         // XZ Plane
            xcoord = 0;
            ycoord = 2;
            zcoord = 1;
            break;
        }

    if (divisions < 4)
        {
            // With the range restriction we have on the divisions, this
            // is actually impossible, but it shows how to add an error
            // message or warning to the SOP.
            addWarning(SOP_MESSAGE, "Invalid divisions");
            divisions = 4;
        }

    // Start the interrupt server
    UT_AutoInterrupt boss("Building Star");
    if (boss.wasInterrupted())
        {
            return;
        }

    // Build a polygon
    GEO_PrimPoly *poly = GEO_PrimPoly::build(dst, divisions, GU_POLY_CLOSED);
    float tinc = M_PI*2 / (float)divisions;

    // Now, set all the points of the polygon
    for (int i = 0; i < divisions; i++)
        {
            // Check to see if the user has interrupted us...
            if (boss.wasInterrupted())
                break;

            // Since we expect the local variables to be used in specifying
            // the radii, we have to evaluate the channels INSIDE the loop
            // through the points...

            float tmp = (float)i * tinc;
            float rad = (i & 1) ? XRADIUS(now) : YRADIUS(now);
            if (!negradius && rad < 0)
                rad = 0;

            UT_Vector3 pos;
            pos(xcoord) = SYScos(tmp) * rad + tx;
            pos(ycoord) = SYSsin(tmp) * rad + ty;
            pos(zcoord) = 0 + tz;

            GA_Offset ptoff = poly->getPointOffset(i);
            dst->setPos3(ptoff, pos);
        }

}

GU_DetailHandle
SOP_DualStar::cookMySopOutput(OP_Context &context, int outputidx, SOP_Node *interests)
{
    GU_DetailHandle result;
    GU_Detail       *dst = new GU_Detail();

    result.allocateAndSet(dst);

    buildStar(dst, context);

    return result;
}


///
/// newSopOperator is the hook that Houdini grabs from this dll
/// and invokes to register the SOP.  In this case we add ourselves
/// to the specified operator table.
///
void
newSopOperator(OP_OperatorTable *table)
{

    PRM_Name     * negativeName = new PRM_Name ("nradius", "Negative Radius");
    PRM_Name     * negativeName2 = new PRM_Name ("nradius2", "Negative Radius2");
    // PRM_Default  fiveDefault(6);     // Default to 5 divisions
    PRM_Default  radiiDefaults[] = {
        PRM_Default(1),      // Outside radius
        PRM_Default(0.3)     // Inside radius
    };

    std::vector<PRM_Template> * vvv = new std::vector<PRM_Template>();
    auto p1 = PRM_Template(PRM_TOGGLE, 1, negativeName);
    auto p2 = PRM_Template(PRM_TOGGLE, 1, negativeName2);
    auto pend = PRM_Template();
    vvv->push_back(p1);
    vvv->push_back(p2);
    vvv->push_back(pend);
    
    OP_Operator * op = new OP_Operator(
        "hdk_dualstar",                 // Internal name
        "Dual Star",                    // UI name
        SOP_DualStar::myConstructor,    // How to build the SOP
        /* SOP_DualStar::myTemplateList,   */
/*SOP_DualStar::parms.myTemplateList,*/
        vvv->data(),
        1,                          // Min # of sources
        1,                          // Max # of sources
        0,                          // Local variables
        OP_FLAG_GENERATOR,          // Flag it as generator
        0,                          // labels
        2);                         // Outputs.
   
    std::cout << "Addding new operator: " << "HDK star" << std::endl;
    table->addOperator(op);
    g_op = op;

    VersionCounter& counter = VersionCounter::getInstance();
    cout << " dso ver:" << counter.dso_version << endl;
    
    if (counter.dso_version == 0){
        OP_Director * dir = OPgetDirector();
        const char * abc =  "abc";
        cout << "setting callback " << endl;
        dir->setSaveCallback(reload_callback, (void*)abc);

        g_table = table;
    }

    counter.dso_version++;
        
}

