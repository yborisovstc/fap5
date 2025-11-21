#ifndef __FAP5_CONNPOINT_H
#define __FAP5_CONNPOINT_H

#include "mconnpoint.h"
#include "vert.h"
#include "node.h"
#include "elem.h"
#include "npconn.h"

// TODO consider using prov/req ifaces id as arrts instead of template pars
/** @brief Connection point in native treee.
 * In contrastof native connpoints (nconn.h) it is native tree node
 * so can be managed via model mutation.
 * Connpoint proxies provided iface, set via binding operation
 * */
class ConnPoint : public Vert, public MConnPoint
{
    public:
        ConnPoint(const string &aType, const string &aName, MEnv* aEnv, MIface::TIdHash aPifId, MIface::TIdHash aRifId): Vert(aType, aName, aEnv),
            mPifId(aPifId), mRifId(aRifId) {}
        virtual ~ConnPoint() {}
        // From MNode
        MIface *MNode_getLif(TIdHash aId) override;
        MIface *MOwned_getLif(TIdHash aId) override;
        // From MConnPoint
        string MConnPoint_Uid() const override  { return getUid<MConnPoint>();}
        TIdHash idProvided() const override { return mPifId; }
        TIdHash idRequired() const override { return mRifId; }
        // From MVert
        MIface *MVert_getLif(TIdHash aId) override;
        bool isCompatible(const MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override { return ERegular;}
	bool isBound(const MVert* aBound) const override;
        bool bind(MVert* aPair) override;
        bool unbind(MVert* aPair) override;
        // From MConnPoint
        bool bind(MNpc* aPair) override;
        bool unbind(MNpc* aPair) override;
	bool isBound(const MNpc* aPair) const;
    protected:
        MIface::TIdHash mPifId;
        MIface::TIdHash mRifId;
        MVert* mMVert = nullptr;
        MConnPoint* mMConnPoint = nullptr;
};



/** @brief Socket, monolitic.
 * Note that socket pins shall be bindable, e.g. CpStateInpPin (not CpStateInp)
*/
class Socket: public Verte, public MSocket
{
    public:
        inline static constexpr std::string_view idStr() { return "Socket"sv;}
        Socket(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        // From MNode
        MIface* MNode_getLif(TIdHash aId) override;
        MIface *MOwned_getLif(TIdHash aId) override;
        //string parentName() const override { return Type(); }
        // From MVert
        MIface* MVert_getLif(TIdHash aId) override;
        bool isCompatible(const MVert* aPair, bool aExt = false) const override;
        MVert* getExtd() override;
        TDir getDir() const override;
	bool isBound(const MVert* aBound) const override;
        bool bind(MVert* aPair) override;
        bool unbind(MVert* aPair) override;
        // From MSocket
        string MSocket_Uid() const override  { return getUid<MSocket>();}
        int PinsCount() const override;
        MVert* GetPin(int aInd) override;
        MVert* GetPin(const string& aId) override;
        string GetPinId(int aInd) const override;
    protected:
        // From Vert
        void onConnected(MVert* aPair) override;
        void onDisconnecting(MVert* aPair) override;
        void onDisconnected() override;
    protected:
        // Bind/connect, unbind/disconnect pins
        void bindPins(MSocket* aPair, bool aConnect, bool aUndo = false);
        virtual MVert* checkPin(MOwned* aOwned);
    protected:
        // TODO consider adding binding point
        MVert* mMVert = nullptr;
        MSocket* mMSocket = nullptr;
};

/** @brief Socket extender, ds_sock_eos_sse solution 
 * */
class SocketExtd : public Socket
{
    public:
        inline static constexpr std::string_view idStr() { return "SocketExtd"sv;}
	SocketExtd(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        virtual ~SocketExtd() {}
    public:
	// From Node
	void onOwnedAttached(MOwned* aOwned) override;
	void onOwnedDetached(MOwned* aOwned) override;
        // From MVert
        MVert* getExtd() override;
        // From MSocket
        MVert* GetPin(const string& aId) override;
    protected:
        MVert* checkPin(MOwned* aOwned) override;
    public:
	static const string KUriInt;  /*!< Internal connpoint Uri */
};

/** @brief Socket extender int point
 * */
class SocketExtdInt : public Socket
{
    public:
        inline static constexpr std::string_view idStr() { return "SocketExtdInt"sv;}
	SocketExtdInt(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        virtual ~SocketExtdInt() {}
    public:
	// From Node
	void onOwnedAttached(MOwned* aOwned) override;
	void onOwnedDetached(MOwned* aOwned) override;
        // From MSocket
        MVert* GetPin(const string& aId) override;
    protected:
        MVert* checkPin(MOwned* aOwned) override;
};


/** @brief Generic extender of CP, ref ds_vbcr_extd_ep
 * It is "universal" but less effective that specialized extender
 * Extender doesnt act as iface implementor, i.e. doesn't propagate iface calls
 * So Internal CP has to be iface implementor itslef (i.e. be CpPin)
 * */
class Extd: public Vert
{
    public:
        inline static constexpr std::string_view idStr() { return "Extd"sv;}
	Extd(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        // From MOwner
        MIface *MOwner_getLif(TIdHash aId) override;
	// From MVert
	bool isCompatible(const MVert* aPair, bool aExt = false) const override;
	MVert* getExtd() override;
	TDir getDir() const override;
    public:
	static const string KUriInt;  /*!< Internal connpoint Uri */
    protected:
        MVert* mMVert = nullptr;
};





#endif
