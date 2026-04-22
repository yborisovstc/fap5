#ifndef __FAP5_SYSDM_H
#define __FAP5_SYSDM_H

#include "../src/syst.h"
//#include "../src/env.h"

/** @brief System with distributed model support
 * This class provides mechanisms for managing distributed models across multiple nodes.
 * It handles synchronization, state propagation, and model updates in a distributed environment.
 */
class SystDm : public Syst
{
public:
    inline static constexpr std::string_view idStr() { return "SystDm"sv;}
public:
    SystDm(const string &aType, const string &aName, MEnv *aEnv);
    virtual ~SystDm();
    /** @brief Override mutAddElem
     * Custom implementation for adding elements in distributed model system.
     * @param aMut Mutation node representing the element to add
     * @param aUpdOnly If true, update only without adding new elements
     * @param aCtx Mutation context
     * @return Pointer to the created/updated node, or nullptr on failure
     */
    MNode* mutAddElem(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx) override;


private:
    // Private member variables and helper methods would go here
};

#endif
