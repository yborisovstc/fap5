
#ifndef __FAP5_ENVDM_H__
#define __FAP5_ENVDM_H__


#include "../src/env.h"

class MOwgRenvAdp;

/** @brief Environment supporting distributed model
 * */
class EnvDm : public Env
{
    public:
	EnvDm(const string& aSpecFileName, const string& aLogFileName = string());
	EnvDm(bool aOpt, const string& aSpec, const string& aLogFileName = string());
	virtual ~EnvDm();
        // From MEnv
	bool constructSystem() override;
    protected:
        MOwgRenvAdp* mRenva = nullptr;
};

#endif
