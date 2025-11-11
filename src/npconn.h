#ifndef __FAP5_NPCONN_H
#define __FAP5_NPCONN_H

#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <forward_list>
#include <iostream>

#include <assert.h>

#include "miface.h"

/** @brief Native polyphormic connpoint
 * */

using namespace std;

/** @brief Native connection point interface, provided iface proxy variant
 * @tparam  MIface  provided interface
 * @tparam  MIface  required interface
 * */
class MNpc
{
    public:
	/** @brief Iterator interface, ref ds_nn_tp_uit for design
	 * */
	class MIter : std::iterator<input_iterator_tag, MNpc*> {
	    public:
		virtual ~MIter() {}
		virtual MIter* clone() { return nullptr;}
		virtual MIter& operator++() { return *this;}
		virtual bool operator==(const MIter& aIt) const { return false;}
		virtual bool operator!=(const MIter& aIt) const { return false;}
		virtual MNpc* operator*() { return nullptr;}
	};
	class MCIter : std::iterator<input_iterator_tag, const MNpc*> {
	    public:
		virtual ~MCIter() {}
		virtual MCIter* clone() { return nullptr;}
		virtual MCIter& operator++() { return *this;}
		virtual bool operator==(const MCIter& aIt) const { return false;}
		virtual bool operator!=(const MCIter& aIt) const { return false;}
		virtual const MNpc* operator*() { return nullptr;}
	};

	/** @brief Pairs Iterator, ref ds_nn_tp_uit for design
	 * */
	class PairsIter :  std::iterator<input_iterator_tag, MNpc*> {
	    public:
		PairsIter(const PairsIter& aItr) {
		    mItr = aItr.mItr->clone();
		}
		PairsIter(MIter* aItr): mItr(aItr) {}
		virtual ~PairsIter() {
		    delete mItr;
		    mItr = nullptr;
		}
		PairsIter& operator=(const PairsIter& aItr) {
		    delete mItr;
		    mItr = aItr.mItr->clone();
		    return *this;
		}
		PairsIter& operator++() { ++(*mItr); return *this; }
		PairsIter operator++(int) { PairsIter tmp(*this); operator++(); return tmp; }
		bool operator==(const PairsIter& aIt) const {
		    return (*mItr == *(aIt.mItr));
		}
		bool operator!=(const PairsIter& aIt) const {
		    return (*mItr != *(aIt.mItr));
		}
		MNpc* operator*() { return *(*mItr);}
	    protected:
		MIter* mItr = nullptr;
	};
	class PairsConstIter :  std::iterator<input_iterator_tag, const MNpc*> {
	    public:
		PairsConstIter(const PairsConstIter& aItr) {
		    mItr = aItr.mItr->clone();
		}
		PairsConstIter(MCIter* aItr): mItr(aItr) {}
		virtual ~PairsConstIter() {
		    delete mItr;
		    mItr = nullptr;
		}
		PairsConstIter& operator=(const PairsConstIter& aItr) {
		    delete mItr;
		    mItr = aItr.mItr->clone();
		    return *this;
		}
		PairsConstIter& operator++() { ++(*mItr); return *this; }
		PairsConstIter operator++(int) { PairsConstIter tmp(*this); operator++(); return tmp; }
		bool operator==(const PairsConstIter& aIt) {
		    return (*mItr == *(aIt.mItr));
		}
		bool operator!=(const PairsConstIter& aIt) {
		    return (*mItr != *(aIt.mItr));
		}
		const MNpc* operator*() { return const_cast<const MNpc*>(*(*mItr));}
	    protected:
		MCIter* mItr = nullptr;
	};

    public:
	virtual ~MNpc() {}
	virtual MIface* provided() = 0;
	virtual const MIface* provided() const = 0;
        virtual MIface::TIdHash pifId() const = 0;
        virtual MIface::TIdHash rifId() const = 0;
	virtual bool connect(MNpc* aPair) = 0;
	virtual bool disconnect(MNpc* aPair) = 0;
	virtual bool disconnectAll() = 0;
	virtual bool attach(MNpc* aPair) = 0;
	virtual bool detach(MNpc* aPair) = 0;
	virtual bool isConnected(MNpc* aPair) const = 0;
	virtual bool getId(string& aId) const = 0;
	virtual void dump(int aIdt) const {}
	/** @brief Gets binded connpoint, in tree node for instance, ref ds_nn_tree_bc */
	virtual MNpc* binded() = 0;
	virtual const MNpc* binded() const = 0;
	/** @brief Gets pairs count */
	virtual int pcount(bool aRcr = false) const = 0;
	/** @brief Gets binded pairs count */
	virtual int bpcount(bool aRcr = false) const = 0;
	/** @brief Pair by index, for debugging purpore mostly */
	virtual MNpc* pairAt(int aInd) = 0;
	virtual const MNpc* pairAt(int aInd) const = 0;
	/** @brief Pair by ID */
	virtual MNpc* pairAt(const string& aId) = 0;
	virtual const MNpc* pairAt(const string& aId) const = 0;
	// Traversal
	virtual PairsIter pairsBegin() = 0;
	virtual PairsIter pairsEnd() = 0;
	virtual PairsConstIter pairsCBegin() const = 0;
	virtual PairsConstIter pairsCEnd() const = 0;
        template<typename T> T* prov() { auto p = provided(); return (T::idHash() == p->id()) ? reinterpret_cast<T*>(p) : nullptr; }
};

class NpcBase : public MNpc
{
    public:
        NpcBase(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aPx):
            mPifId(aPifId), mRifId(aRifId), mPx(aPx) {}
    public:
	MIface* provided() override { return mPx;}
	const MIface* provided() const override { return mPx;}
        // From MNpc
        MIface::TIdHash pifId() const override { return mPifId; }
        MIface::TIdHash rifId() const override { return mRifId; }
    protected:
        MIface::TIdHash mPifId;
        MIface::TIdHash mRifId;
        MIface* mPx;
};

/** @brief Native connection point, one-to-many, seleting pair by Id, proxied
 * @tparam MIface  type of provided interface
 * @tparam MNpc type of pair
 * */
class NpcOmip : public NpcBase
{
    public:
	//using MNpcs = unordered_map<string, MNpc*>;
	using MNpcs = map<string, MNpc*>;
	using MNpcsElem = pair<string, MNpc*>;
	using MNpcsIterator =  MNpcs::iterator;
	using MNpcsConstIterator =  MNpcs::const_iterator;

    public:
	class PIter : public MIter {
	    public:
		PIter(const MNpcsIterator& aItr): mItr(aItr) {}
		PIter(const PIter& aItr): mItr(aItr.mItr) {}
		MIter* clone() override { return new PIter(*this);}
		PIter& operator=(const PIter& aIt) { this->mItr = aIt.mItr; return *this; }
		PIter& operator++() override { mItr++; return *this; }
		bool operator==(const  MIter& aIt) const override {
		    auto ait = reinterpret_cast<const PIter&>(aIt);
		    return (mItr == ait.mItr);
		}
		bool operator!=(const  MIter& aIt) const override {
		    auto ait = reinterpret_cast<const PIter&>(aIt);
		    return !(mItr == ait.mItr);
		}
		MNpc* operator*() override { return mItr->second;}
	    protected:
		MNpcsIterator mItr;
	};
	class PCIter : public MCIter {
	    public:
		PCIter(const MNpcsConstIterator& aItr): mItr(aItr) {}
		PCIter(const PCIter& aItr): mItr(aItr.mItr) {}
		 MCIter* clone() override { return new PCIter(*this);}
		PCIter& operator=(const PCIter& aIt) { this->mItr = aIt.mItr; return *this; }
		PCIter& operator++() override { mItr++; return *this; }
		bool operator==(const  MCIter& aIt) const override {
		    auto ait = reinterpret_cast<const PCIter&>(aIt);
		    return (mItr == ait.mItr);
		}
		bool operator!=(const  MCIter& aIt) const override {
		    auto ait = reinterpret_cast<const PCIter&>(aIt);
		    return !(mItr == ait.mItr);
		}
		const MNpc* operator*() override { return mItr->second;}
	    protected:
		MNpcsConstIterator mItr;
	};

    public:
	NpcOmip(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aPx): NpcBase(aPifId, aRifId, aPx) {}
	virtual ~NpcOmip() { disconnectAll(); }
	// From MNpc
	bool attach(MNpc* aPair) override;
	bool detach(MNpc* aPair) override;
	bool connect(MNpc* aPair) override;
	bool disconnect(MNpc* aPair) override;
	bool disconnectAll() override {
	    bool res = true;
	    auto pi = mPairs.begin();
	    while (pi != mPairs.end()) {
		res = disconnect(pi->second);
		assert(res);
		auto pi = mPairs.begin();
	    }
	    return res;
	}
	bool isConnected(MNpc* aPair) const override;
	bool getId(string& aId) const override { return false;}
	void dump(int aIdt) const override {
	    for (auto item : mPairs) {
		item.second->dump(aIdt);
	    }
	}
	MNpc* binded() override { return nullptr;}
	const MNpc* binded() const override { return nullptr;}
	int pcount(bool aRcr = false) const override {
	    return mPairs.size();
        }
	int bpcount(bool aRcr = false) const override {
	    int res = mPairs.size();
	    if (aRcr) {
		for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
		    res += it->second->bpcount(aRcr);
		}
	    }
	    return res;
	}
	const MNpc* pairAt(int aInd) const override {
	    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
		if (aInd-- == 0) return it->second;
	    }
	    return nullptr;
	}
	MNpc* pairAt(int aInd) override {
	    for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
		if (aInd-- == 0) return it->second;
	    }
    	    return nullptr;
	}
	MNpc* pairAt(const string& aId) override {
            auto it = mPairs.find(aId);
            return (it == mPairs.end()) ? nullptr : it->second;
	}
	const MNpc* pairAt(const string& aId) const override {
            auto it = mPairs.find(aId);
            return (it == mPairs.end()) ? nullptr : it->second;
	}
	MNpcs& pairsCtr() { return mPairs;}
	// Traversal
	 PairsIter pairsBegin() override {
	    return  PairsIter(new PIter(mPairs.begin()));
	}
	 PairsIter pairsEnd() override {
	    return  PairsIter(new PIter(mPairs.end()));
	}
	 PairsConstIter pairsCBegin() const  override {
	    return  PairsConstIter(new PCIter(mPairs.cbegin()));
	}
	 PairsConstIter pairsCEnd() const override {
	    return  PairsConstIter(new PCIter(mPairs.cend()));
	}
    protected:
	MNpcs mPairs;
};

/** @brief Native connection point, one-to-many, no Id, proxy
 * @tparam MIface  type of provided interface
 * @tparam MNpc type of pair
 * @tparam THost type of host
 * @tparam TPx type of provided iface proxy
 * */
class NpcOmnp : public NpcBase
{
    public:
	using MNpcs = set<MNpc*>;
	using MNpcsIterator =  MNpcs::iterator;
	using MNpcsConstIterator =  MNpcs::const_iterator;

    public:
	class PIter : public MIter {
	    public:
		PIter(const MNpcsIterator& aItr): mItr(aItr) {}
		PIter(const PIter& aItr): mItr(aItr.mItr) {}
		 MIter* clone() override { return new PIter(*this);}
		PIter& operator=(const PIter& aIt) { this->mItr = aIt.mItr; return *this; }
		PIter& operator++() override { mItr++; return *this; }
		//PIter operator++(int) { PIter tmp(*this); operator++(); return tmp; }
		bool operator==(const  MIter& aIt) const override {
		    auto ait = reinterpret_cast<const PIter&>(aIt);
		    return (mItr == ait.mItr);
		}
		bool operator!=(const  MIter& aIt) const override {
		    auto ait = reinterpret_cast<const PIter&>(aIt);
		    return (mItr != ait.mItr);
		}
		MNpc* operator*() override { return *mItr;}
	    protected:
		MNpcsIterator mItr;
	};
	class PCIter : public MCIter {
	    public:
		PCIter(const MNpcsConstIterator& aItr): mItr(aItr) {}
		PCIter(const PCIter& aItr): mItr(aItr.mItr) {}
		 MCIter* clone() override { return new PCIter(*this);}
		PCIter& operator=(const PCIter& aIt) { this->mItr = aIt.mItr; return *this; }
		PCIter& operator++() override { mItr++; return *this; }
		bool operator==(const  MCIter& aIt) const override {
		    auto ait = reinterpret_cast<const PCIter&>(aIt);
		    return (mItr == ait.mItr);
		}
		bool operator!=(const  MCIter& aIt) const override {
		    auto ait = reinterpret_cast<const PCIter&>(aIt);
		    return (mItr != ait.mItr);
		}
		const MNpc* operator*() override { return *mItr;}
	    protected:
		MNpcsConstIterator mItr;
	};
    public:
	NpcOmnp(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aPx): NpcBase(aPifId, aRifId, aPx) {}
	virtual ~NpcOmnp() { disconnectAll(); }
	bool connect(MNpc* aPair) override ;
	bool disconnect(MNpc* aPair) override;
	bool disconnectAll() override {
	    bool res = true;
	    auto pi = mPairs.begin();
	    while (pi != mPairs.end()) {
		res = disconnect(*pi);
		assert(res);
		pi = mPairs.begin();
	    }
	    return res;
	}
	virtual bool attach(MNpc* aPair) override;
	virtual bool detach(MNpc* aPair) override;
	virtual bool isConnected(MNpc* aPair) const override;
	virtual bool getId(string& aId) const override { return false;}
	virtual MNpc* binded() override { return nullptr;}
	virtual const MNpc* binded() const override { return nullptr;}
	virtual int pcount(bool aRcr = false) const override {
	    return mPairs.size();
        }
	virtual int bpcount(bool aRcr = false) const override {
	    int res = mPairs.size();
	    if (aRcr) {
		for (auto it = mPairs.begin(); it != mPairs.end(); it++) {
		    auto pair = *it;
		    res += pair->bpcount(aRcr);
		}
	    }
	    return res;
	}
	virtual const MNpc* pairAt(int aInd) const override { for (auto it = mPairs.begin(); it != mPairs.end(); it++) if (aInd-- == 0) return *it; return nullptr; }
	virtual MNpc* pairAt(int aInd) override { for (auto it = mPairs.begin(); it != mPairs.end(); it++) if (aInd-- == 0) return *it; return nullptr; }
	virtual MNpc* pairAt(const string& aId) override { return nullptr;}
	virtual const MNpc* pairAt(const string& aId) const override { return nullptr;}
	MNpcs& pairsCtr() { return mPairs;}
	// Traversal
	virtual  PairsIter pairsBegin() {
	    auto pb = mPairs.begin();
	    auto* pi = new PIter(pb);
	    return  PairsIter(pi);
	}
	virtual  PairsIter pairsEnd() {
	    return  PairsIter(new PIter(mPairs.end()));
	}
	virtual  PairsConstIter pairsCBegin() const override {
	    return  PairsConstIter(new PCIter(mPairs.cbegin()));
	}
	virtual  PairsConstIter pairsCEnd() const override {
	    return  PairsConstIter(new PCIter(mPairs.cend()));
	}
    public:
	MNpcs mPairs;
    protected:
	MIface* mPx;
};

/** @brief Native connection point, one-to-one, no Id, proxy
 * @tparam MIface  type of provided interface
 * @tparam MNpc type of pair
 * @tparam THost type of host
 * @tparam TPx type of provided iface proxy
 * */
class NpcOnp : public NpcBase
{
    public:
	using MNpcsIterator = MNpc*;
	using MNpcsConstIterator = const MNpc*;
    public:
	class PIter : public MIter {
	    public:
		PIter(const MNpcsIterator& aItr): mItr(aItr) {}
		PIter& operator=(const PIter& aIt) { this->mItr = aIt.mItr; return *this; }
		PIter& operator++() override { mItr = nullptr; return *this; }
		//PIter operator++(int) override { PIter tmp(*this); operator++(); return tmp; }
		bool operator==(const  MIter& aIt) const override { auto ait = reinterpret_cast<const PIter&>(aIt); return (mItr == ait.mItr); }
		bool operator!=(const  MIter& aIt) const override { auto ait = reinterpret_cast<const PIter&>(aIt); return !(mItr == ait.mItr); }
		MNpc* operator*() override { return mItr;}
	    protected:
		MNpc* mItr;
	};
	class PCIter : public MCIter {
	    public:
		PCIter(const MNpcsConstIterator& aItr): mItr(aItr) {}
		PCIter& operator=(const PCIter& aIt) { this->mItr = aIt.mItr; return *this; }
		PCIter& operator++() override { mItr = nullptr; return *this; }
		//PIter operator++(int) override { PIter tmp(*this); operator++(); return tmp; }
		bool operator==(const  MCIter& aIt) const override {
		    auto ait = reinterpret_cast<const PCIter&>(aIt);
		    return (mItr == ait.mItr);
		}
		bool operator!=(const  MCIter& aIt) const override {
		    auto ait = reinterpret_cast<const PCIter&>(aIt);
		    return !(mItr == ait.mItr);
		}
		const MNpc* operator*() override { return mItr;}
	    protected:
		const MNpc* mItr;
	};
    public:
	NpcOnp(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aPx): NpcBase(aPifId, aRifId, aPx) {}
	virtual ~NpcOnp() { disconnectAll(); }
	virtual bool connect(MNpc* aPair) override ;
	virtual bool disconnectAll() override {
	    bool res = true;
	    if (mPair) res = disconnect(mPair);
	    return res;
	}
	virtual bool disconnect(MNpc* aPair) override;
	virtual bool attach(MNpc* aPair) override;
	virtual bool detach(MNpc* aPair) override;
	virtual bool isConnected(MNpc* aPair) const override;
	virtual bool getId(string& aId) const override { return false;}
	virtual MNpc* binded() override { return nullptr;}
	virtual const MNpc* binded() const override { return nullptr;}
	virtual int pcount(bool aRcr = false) const override {return mPair ? 1 : 0;}
	virtual int bpcount(bool aRcr = false) const override {
            return binded() ? binded()->bpcount(aRcr) : 0;
        }
	virtual const MNpc* pairAt(int aInd) const override { return (aInd < pcount()) ? mPair : nullptr; }
	virtual MNpc* pairAt(int aInd) override { return (aInd < pcount()) ? mPair : nullptr; }
	virtual MNpc* pairAt(const string& aId) override { return nullptr;}
	virtual const MNpc* pairAt(const string& aId) const override { return nullptr;}
	// Traversal
	virtual  PairsIter pairsBegin() {
	    auto it =  PairsIter(new PIter(mPair));
	    return it;
	}
	virtual  PairsIter pairsEnd() override {
	    return  PairsIter(new PIter(nullptr));
	}
	virtual  PairsConstIter pairsCBegin() const override {
	    return  PairsConstIter(new PCIter(mPair));
	}
	virtual  PairsConstIter pairsCEnd() const override {
	    return  PairsConstIter(new PCIter(nullptr));
	}

	// Local
	void resetPx() {mPx = nullptr;}
	bool disconnect() { return  mPair ? disconnect(mPair) : false; }
    public:
	MNpc* mPair = nullptr;
};

/** @brief Native net tree node, identified sub-nodes
 * */
class NpTnip : public NpcOnp
{
    public:
	using TScp = NpcOnp;  /*!< Self connpoint type */
	using TCnode = NpcOmip; /*!< Complement node (pole) type */
    public:
	class Cnode: public TCnode {
	    public:
		Cnode(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aPx, NpTnip* aHost): TCnode(aPifId, aRifId, aPx), mHost(aHost) {}
		// From MNpc
		virtual  TCnode::MNpc* binded() override { return mHost->cnodeBinded();}
                virtual const  TCnode::MNpc* binded() const override { return mHost->cnodeBinded();}
	    private:
		NpTnip* mHost;
	};
    public:
	NpTnip(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aProvPx, MIface* aReqPx): NpcOnp(aPifId, aRifId, aProvPx), mCnode(aRifId, aPifId, aReqPx, this) {}
	// From MNpc
	virtual  TScp::MNpc* binded() override { return &mCnode;}
	virtual const  TScp::MNpc* binded() const override { return &mCnode;}
	virtual int bpcount(bool aRcr = false) const override {
	    return mCnode.bpcount(aRcr);
	}
	virtual bool disconnectAll() override {
	    bool res = TScp::disconnectAll();
	    res = res && mCnode.disconnectAll();
	    return res;
	}
	// Local
	virtual  TCnode::MNpc* cnodeBinded() { return this;}
    public:
	Cnode mCnode;
};


/** @brief Native net tree node, not indexed sub-nodes
 * */
class NpTnnp : public NpcOnp
{
    public:
	using TScp = NpcOnp;  /*!< Self connpoint type */
	using TCnode = NpcOmnp; /*!< Complement node (pole) type */
    public:
	class Cnode: public TCnode {
	    public:
		Cnode(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aPx, NpTnnp* aHost): TCnode(aPifId, aRifId, aPx), mHost(aHost) {}
		// From MNpc
		virtual  TCnode::MNpc* binded() override { return mHost;}
		virtual const  TCnode::MNpc* binded() const override { return mHost;}
	    private:
		NpTnnp* mHost;
	};
    public:
	NpTnnp(MIface::TIdHash aPifId, MIface::TIdHash aRifId, MIface* aProvPx, MIface* aReqPx): NpcOnp(aPifId, aRifId, aProvPx), mCnode(aRifId, aPifId,aReqPx, this) {}
	// From MNpc
	virtual  TScp::MNpc* binded() override { return &mCnode;}
	virtual const  TScp::MNpc* binded() const override { return &mCnode;}
	virtual bool disconnectAll() override {
	    bool res = TScp::disconnectAll();
	    res = res && mCnode.disconnectAll();
	    return res;
	}
	// TODO is it correct solution? why its redirected to binded?
	virtual int pcount(bool aRcr = false) const override {
	    return mCnode.pcount(aRcr);
	}
    public:
	Cnode mCnode;
};

#endif
