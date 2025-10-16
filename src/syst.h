#ifndef __FAP5_SYST_H
#define __FAP5_SYST_H

#include "msyst.h"

#include "elem.h"
#include "connpoint.h"

// TODO these class allows to avoid link to managed agent in DES adapter
// is it really good solution or link based desing is still acceptable?
/** @brief Connection point of explorable system
 * Intended for using witin explorable system, no proxied, i.e. commutate iface calls by itself
 * */
class CpSystExplorable: public ConnPoint<MSystExplorable, MSystExploring>, public MSystExploring, public MSystExplorable
{
    public:
	using TBase = ConnPoint<MSystExplorable, MSystExploring>;
        using TExploringCp = NCpOnp<MSystExploring, MSystExplorable>;
    public:
	inline static constexpr std::string_view idStr() { return "CpSystExplorable"sv;}
        CpSystExplorable(const string &aType, const string& aName, MEnv* aEnv):
            ConnPoint<MSystExplorable, MSystExploring>(aType, aName, aEnv), mExploringCp(this) {}
	// From MOwned
        MIface *MOwned_getLif(TIdHash aId) override;
	// From MSystExplorable
	string MSystExplorable_Uid() const override {return getUid<MSystExplorable>();}
	MIface* MSystExplorable_getLif(TIdHash aId) override; 
	MNode* getMag() override;
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override; 
	void onMagChanged() override {}
	MSystExploring::TCp* getCp() override { return &mExploringCp;}
    protected:
        // From ConnPoint
	void onConnected() override;
	void onDisconnected() override;
	void onBound() override;
	void onUnbound() override;
    protected:
	TExploringCp mExploringCp;
	MSystExplorable* mMSystExplorable = nullptr;
	MSystExploring* mMSystExploring = nullptr;
};

/** @brief Connection point of exploring system
 * Intended to be used from exploring system (DES adapter etc). Proxied.
 * */
class CpSystExploring: public ConnPoint<MSystExploring, MSystExplorable>
{
    public:
	using TBase = ConnPoint<MSystExploring, MSystExplorable>;
    public:
	inline static constexpr std::string_view idStr() { return "CpSystExploring"sv;}
        CpSystExploring(const string &aType, const string& aName, MEnv* aEnv):
            TBase(aType, aName, aEnv) {}
    protected:
        // From ConnPoint
	void onConnected() override;
	void onDisconnected() override;
	void onBound() override;
	void onUnbound() override;
};

/** @brief Extender of CpSystExplorable
 * */
class ExtdSystExplorable : public ConnPoint<MSystExplorable, MSystExploring>, public MSystExplorable, public MSystExploring
{
    public:
	inline static constexpr std::string_view idStr() { return "ExtdSystExplorable"sv;}
        void Construct() override;
	ExtdSystExplorable(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        // From Vert
	MVert* getExtd() override { return mInt;}
	// From MSystExplorable
	string MSystExplorable_Uid() const override {return getUid<MSystExplorable>();}
	MIface* MSystExplorable_getLif(TIdHash aId) override { return nullptr;} 
	MNode* getMag() override;
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;} 
	void onMagChanged() override;
	MSystExploring::TCp* getCp() override { return nullptr;}
    public:
        CpSystExploring* mInt = nullptr;
    protected:
        static string KIntName;
};

/** @brief Extender of CP CpSystExploring
 * */
class ExtdSystExploring : public CpSystExploring, public MSystExplorable, public MSystExploring
{
    public:
	inline static constexpr std::string_view idStr() { return "ExtdSystExploring"sv;}
        void Construct() override;
	ExtdSystExploring(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        // From Vert
	MVert* getExtd() override { return mInt;}
	// From MSystExplorable
	string MSystExplorable_Uid() const override {return getUid<MSystExplorable>();}
	MIface* MSystExplorable_getLif(TIdHash aId) override { return nullptr;} 
	MNode* getMag() override;
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;} 
	void onMagChanged() override;
	MSystExploring::TCp* getCp() override { return nullptr;}
    public:
        CpSystExplorable* mInt = nullptr;
    protected:
        static string KIntName;
};

/** @brief Pin of system Explorable
 * */
class PinSystExplorable : public ConnPoint<MSystExplorable, MSystExploring>, public MSystExplorable, public MSystExploring
{
    public:
	inline static constexpr std::string_view idStr() { return "PinSystExplorable"sv;}
	PinSystExplorable(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From MSystExplorable
	string MSystExplorable_Uid() const override {return getUid<MSystExplorable>();}
	MIface* MSystExplorable_getLif(TIdHash aId) override { return nullptr;} 
	MNode* getMag() override { return mProvidedPx ? mProvidedPx->getMag() : nullptr; }
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;} 
	void onMagChanged() override;
	MSystExploring::TCp* getCp() override { return nullptr;}
};


/** @brief Pin of CP CpSystExploring
 * */
class PinSystExploring : public CpSystExploring, public MSystExplorable, public MSystExploring
{
    public:
	inline static constexpr std::string_view idStr() { return "PinSystExploring"sv;}
	PinSystExploring(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	// From MSystExplorable
	string MSystExplorable_Uid() const override {return getUid<MSystExplorable>();}
	MIface* MSystExplorable_getLif(TIdHash aId) override { return nullptr;} 
	MNode* getMag() override;
	// From MSystExploring
	string MSystExploring_Uid() const override {return getUid<MSystExploring>();}
	MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;} 
	void onMagChanged() override { if (mProvidedPx) mProvidedPx->onMagChanged(); }
	MSystExploring::TCp* getCp() override { return nullptr;}
};




/** @brief System, inheritable
 * Isn't connectable itlelf but can represent itself via connpoints
 * Can contain connectables and connect them
 * */
class Syst : public Elem, public MSyst, public MAhost, public MSystExplorable
{
    protected:
        using TExplorableCp = NCpOmnp<MSystExplorable, MSystExploring>;
	using TAgtCp = NCpOmnp<MAhost, MAgent>;
        using TBase = Elem;
    public:
	inline static constexpr std::string_view idStr() { return "Syst"sv;}
	inline static constexpr TIdHash idHash() { return 0xac85b9e6fc11ee6e;}
	Syst(const string &aType, const string &aName, MEnv* aEnv);
	virtual ~Syst();
	// From Node
	MIface* MNode_getLif(TIdHash aId) override;
	MIface* MOwned_getLif(TIdHash aId) override;
	// From MSyst
	string MSyst_Uid() const override {return getUid<MSyst>();}
	MIface* MSyst_getLif(TIdHash aId) override;
	const TEdges& connections() const override { return mEdges; }
	// From MAhost
	string MAhost_Uid() const override {return getUid<MSyst>();}
	MIface* MAhost_getLif(TIdHash aId) override;
	// From MSystExplorable
	string MSystExplorable_Uid() const override {return getUid<MSystExplorable>();}
	MIface* MSystExplorable_getLif(TIdHash aId) override; 
	MNode* getMag() override;
	// From MOwner
	void onOwnedAttached(MOwned* aOwned) override;
    protected:
	// From Node
	void mutConnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx) override;
	void mutDisconnect(const ChromoNode& aMut, bool aUpdOnly, const MutCtx& aCtx) override;
    protected:
        // TODO Consider again if we need agents, ref ds_samn
	TAgtCp mAgtCp;  /*!< Agents connpoint */
	TEdges mEdges;  /*!< Edges */
	MSyst* mMSyst = nullptr;
	TExplorableCp mExplorableCp;
};

/** @brief Agent base
 * */
class AgtBase: public Node, public MAgent
{
    public:
	using TAgtCp = NCpOnp<MAgent, MAhost>;  /*!< Agent conn point */
    public:
	AgtBase(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
	virtual ~AgtBase();
	// From MAgent
	string MAgent_Uid() const override {return getUid<MAgent>();}
	MIface* MAgent_getLif(TIdHash aId) override;
        MAgent::TCp* getMAgentCp() override { return &mAgtCp; }
    protected:
	MNode* ahostNode();
    protected:
	TAgtCp mAgtCp;                   /*!< Agent connpoint */
	MAgent* mMAgentPtr = nullptr;
};


#endif
