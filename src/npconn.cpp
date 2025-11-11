
#include "npconn.h"



bool NpcOmip::attach(MNpc* aPair)
{
    string pid;
    assert(aPair && aPair->getId(pid) && !isConnected(aPair));
    auto res = mPairs.insert(MNpcsElem(pid, aPair));
    return res.second;
}

bool NpcOmip::detach(MNpc* aPair)
{
    string pid;
    assert(aPair && aPair->getId(pid) && isConnected(aPair));
    mPairs.erase(pid);
    return true;
}

bool NpcOmip::connect(MNpc* aPair)
{
    assert(aPair && !aPair->isConnected(this) && !isConnected(aPair));
    bool res = aPair->attach(this);
    if (res) {
       	res = attach(aPair);
    }
    return res;
}

bool NpcOmip::disconnect(MNpc* aPair)
{
    assert(aPair && aPair->isConnected(this) && isConnected(aPair));
    bool res = aPair->detach(this);
    if (res) {
       res = detach(aPair);
    }
    return res;
}

bool NpcOmip::isConnected(MNpc* aPair) const
{
    string pid;
    assert(aPair && aPair->getId(pid));
    auto it = mPairs.find(pid);
    return (it != mPairs.end()) && (it->second == aPair);
}





bool NpcOmnp::attach(MNpc* aPair)
{
    assert(aPair && !isConnected(aPair));
    mPairs.insert(aPair);
    return true;
}

bool NpcOmnp::detach(MNpc* aPair)
{
    assert(aPair && isConnected(aPair));
    mPairs.erase(aPair);
    return true;
}

bool NpcOmnp::connect(MNpc* aPair)
{
    assert(aPair && !aPair->isConnected(this) && !isConnected(aPair));
    bool res = aPair->attach(this);
    if (res) {
	res = attach(aPair);
    }
    return res;
}

bool NpcOmnp::disconnect(MNpc* aPair)
{
    assert(aPair && aPair->isConnected(this) && isConnected(aPair));
    bool res = aPair->detach(this);
    if (res) {
	res = detach(aPair);
    }
    assert(res);
    return res;
}

bool NpcOmnp::isConnected(MNpc* aPair) const
{
    return mPairs.find(aPair) != mPairs.end();
}




bool NpcOnp::attach(MNpc* aPair)
{
    assert(aPair && !mPair && !isConnected(aPair));
    mPair = aPair;
    return true;
}

bool NpcOnp::detach(MNpc* aPair)
{
    assert(aPair && isConnected(aPair));
    mPair = nullptr;
    return true;
}

bool NpcOnp::connect(MNpc* aPair)
{
    assert(aPair && !mPair && !aPair->isConnected(this) && !isConnected(aPair));
    bool res = aPair->attach(this);
    if (res) {
	res = attach(aPair);
    }
    return res;
}

bool NpcOnp::disconnect(MNpc* aPair)
{
    assert(aPair && aPair->isConnected(this) && isConnected(aPair));
    bool res = aPair->detach(this);
    if (res) {
	res = detach(aPair);
    }
    return res;
}

bool NpcOnp::isConnected(MNpc* aPair) const
{
    return mPair == aPair;
}


