#pragma once
#include <SOP/SOP_Node.h>
#include <vector>

struct Parms{
    PRM_Template * myTemplateList;
    Parms (){
        PRM_Name     negativeName("nradius", "Negative Radius");
        PRM_Default  fiveDefault(6);     // Default to 5 divisions
        PRM_Default  radiiDefaults[] = {
            PRM_Default(1),      // Outside radius
            PRM_Default(0.3)     // Inside radius
        };

        std::vector<PRM_Template> aaa ={
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

        myTemplateList = aaa.data();
    }

};


class SOP_DualStar : public SOP_Node
    {
    public:
        static OP_Node      *myConstructor(OP_Network*, const char *,
                                           OP_Operator *);

        /// Stores the description of the interface of the SOP in Houdini.
        /// Each parm template refers to a parameter.
        static PRM_Template          myTemplateList[];
        static Parms          parms;

    protected:
        SOP_DualStar(OP_Network *net, const char *name, OP_Operator *op);
        virtual ~SOP_DualStar();


        /// cookMySop does the actual work of the SOP computing, in this
        /// case, a star shape.
        virtual OP_ERROR             cookMySop(OP_Context &context);
        virtual GU_DetailHandle      cookMySopOutput(OP_Context &context, int outputidx, SOP_Node *interest);

        /// Actually build a star in the given geometry
        void    buildStar(GU_Detail *dst,
                          OP_Context &context);

    private:
        /// The following list of accessors simplify evaluating the parameters
        /// of the SOP.
        int     DIVISIONS(fpreal t)     { return evalInt  ("divs", 0, t); }
        fpreal  XRADIUS(fpreal t)   { return evalFloat("rad", 0, t); }
        fpreal  YRADIUS(fpreal t)   { return evalFloat("rad", 1, t); }
        int     NEGRADIUS()         { return evalInt  ("nradius", 0, 0); }
        fpreal  CENTERX(fpreal t)   { return evalFloat("t", 0, t); }
        fpreal  CENTERY(fpreal t)   { return evalFloat("t", 1, t); }
        fpreal  CENTERZ(fpreal t)   { return evalFloat("t", 2, t); }
        int     ORIENT()        { return evalInt  ("orient", 0, 0); }

};
