#include "sysdm.h"
#include "renvadp.h"
#include "../src/syst.h"
#include "../src/env.h"


SystDm::SystDm(const string &aType, const string &aName, MEnv *aEnv) : Syst(aType, aName, aEnv) {
    // Initialize distributed model system
}

SystDm::~SystDm() {
    // Cleanup resources
}

MNode* SystDm::mutAddElem(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx)
{
    // Custom implementation for distributed model system
    // Given from Node, modified to create Primary proxy instead of real parent
    // TODO: Add distributed model specific logic here

    MNode* res = nullptr;
    bool isDm = true;
    string sparent = aMut.Attr(ENa_Parent);
    string sname = aMut.Attr(ENa_Id);

    assert(!sname.empty());
    if (isDm) {
        do {
            // Distributed model
            // Create adapter to owned remote environment
            //MIface* renvai = Provider()->createIfi(string(OwdRenvAdp::idStr()), sname, mEnv);
            //MOwdRenvAdp* renva = renvai ? renvai->lIft<MOwdRenvAdp>() : nullptr;
            MOwdRenvAdp* renva = new OwdRenvAdp(sname, mEnv);
            if (!renva) {
                LOGN2(EErr) << "Failed creating remote env adapter";
                break;
            }
            bool cres = renva->createRenv("", getUriS(), aMut);
            if (!cres) {
                LOGN2(EErr) << "Failed creating remote environment";
                break;
            }
            // Get MOwned proxy to remote component
            MOwned* owd = renva->lIf(owd);
            if (!owd) {
                LOGN2(EErr) << "Failed getting owned proxy to remote component";
                break;
            }
            cres = owrAttachOwned(owd);
            if (!cres) {
                LOGN2(EErr) << "Failed attaching owned " << owd->Uid();
                break;
            }
        } while (false);
    } else {
        // Local model
        // TODO TBD
    }
    return res;
}
