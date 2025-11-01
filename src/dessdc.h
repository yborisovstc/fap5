#ifndef __FAP5_DESSDC_H
#define __FAP5_DESSDC_H

/** @brief System structure DES controller (SDC)
 * */


#include "mdes.h"
#include "mdata.h"
#include "msyst.h"
#include "desadp.h"
#include "rdatauri.h"
#include "rmutdata.h"

/** @brief SDC base agent
 * NOTE: In normal cases MDesObserver iface is not used, so we just keep in for the rare case
 * for instance if the agent includes DES or states.
 * To avoid the agent providing MDesObserver iface to agent host and interfering main DES agent
 * the request from agent host is ignoring in resolveIfc()
 * */
class ASdc : public Node, public MDesSyncable, public MDesObserver, public MObserver, public MDesInpObserver, public MSystExploring
{
    public:
        using TObserverCp = NCpOmnp<MObserver, MObservable>;
        using TDesSyncCp = NCpOnp<MDesSyncable, MDesObserver>;  /*!< DES syncable connpoint */
        using TExplCp = NCpOnp<MSystExploring, MSystExplorable>;
    public:
        /** @brief Node creation observer
         * */
        class NodeCreationObserver : public MObserver {
            public:
                using TObserverCp = NCpOmnp<MObserver, MObservable>;
            public:
                NodeCreationObserver(ASdc* aHost);
                void startObserving(const GUri& aTargUri);
                string MObserver_Uid() const override {return mHost->getUidC<MObserver>("NodeCreationObserver");}
                MIface* MObserver_getLif(TIdHash aId) override { return nullptr;}
                void onObsOwnerAttached(MObservable* aObl) override {}
                void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
                void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override { }
                void onObsContentChanged(MObservable* aObl, const string& aId) override { }
                void onObsChanged(MObservable* aObl) override { }
                void onObsEvent(MObservable* aObl, const MEvent* aEvent) override;

                TCp* observerCp() override { return &mOcp;}
            private:
                ASdc* mHost;
                GUri mTargUri;
                MNode* mTargOwr = nullptr;     //<! Target's owner
                int mTargOwrLevel = -1;
                TObserverCp mOcp;               /*!< Observer connpoint */
        };

        /** @brief Input access point base
         * */
        class SdcIapb {
            public:
                SdcIapb(const string& aName, ASdc* aHost, const string& aInpUri);
                void Construct();
                string getInpUri() const { return mInpUri;}
                virtual bool isDataValid() const = 0;
                virtual bool updateData() = 0;
            public:
                ASdc* mHost;
                string mName;    /*!< Iap name */
                string mInpUri;  /*!< Input URI */
                bool mUpdated = false;
                MVert* mInp = nullptr;
        };

        /** @brief Input access point operating with Sdata
         * @param  T  Sdata type 
         * */
        template <class T>
            class SdcIap: public SdcIapb {
                public:
                    SdcIap(const string& aName, ASdc* aHost, const string& aInpUri): SdcIapb(aName, aHost, aInpUri) {}
                    // Local
                    virtual bool updateData() override;
                    T& data(bool aConf = true);
                    bool isDataValid() const override { return mCdt.IsValid();}
                public:
                    T mCdt;  /*!< Data */
            };

        /** @brief Input "Enable" access point
         * @param  Specific is that boolean AND is applied for inp ifaces
         * */
        class SdcIapEnb: public SdcIap<Sdata<bool>> {
            public:
                SdcIapEnb(const string& aName, ASdc* aHost, const string& aInpUri): SdcIap<Sdata<bool>>(aName, aHost, aInpUri) {}
                // Local
                virtual bool updateData();
        };

        /** @brief Parameter access point base
         * Acts as a binder of MDVarGet request to method of host
         * */
        class SdcPapb: public MDVarGet {
            public:
                SdcPapb(const string& aName, ASdc* aHost, const string& aCpUri);
                void Construct();
                string getCpUri() const { return mCpUri;}
                // From MDVarGet
                virtual string MDVarGet_Uid() const override {return mHost->getUidC<MDVarGet>(mName);}
                MIface* MDVarGet_getLif(TIdHash aId) override {
                    return checkLif2(aId, mMDVarGet);
                }
            public:
                void NotifyInpsUpdated();
            protected:
                template<class T> inline MIface* checkLif2(TIdHash aId, T*& aPtr) {
                    return (aId == T::idHash()) ? (aPtr ? aPtr : (aPtr = dynamic_cast<T*>(this)))  : nullptr;
                }
            public:
                string mName;
                ASdc* mHost;
                string mCpUri;  /*!< Output URI */
                MVert* mOutp = nullptr;
                MDVarGet* mMDVarGet = nullptr;
        };

        /** @brief Parameter access point, using Sdata
         * */
        template <typename T> class SdcPap: public SdcPapb {
            public:
                template<typename P> using TGetData = std::function<void (P&)>;
            public:
                SdcPap(const string& aName, ASdc* aHost, const string& aCpUri, TGetData<T> aGetData): SdcPapb(aName, aHost, aCpUri), mGetData(aGetData) {}
                // From MDVarGet
                virtual string VarGetIfid() const override {return T::TypeSig();}
                virtual MIface* DoGetDObj(const char *aName) override { return nullptr;}
                virtual const DtBase* VDtGet(const string& aType) override { mGetData(mData); return &mData;}
            public:
                TGetData<T> mGetData;
                T mData;
        };

        /** @brief Parameter access point, using cached Sdata
         * */
        template <typename T> class SdcPapc: public SdcPapb {
            public:
                SdcPapc(const string& aName, ASdc* aHost, const string& aCpUri): SdcPapb(aName, aHost, aCpUri) {}
                // From MDVarGet
                virtual string VarGetIfid() const override {return T::TypeSig();}
                virtual MIface* DoGetDObj(const char *aName) override { return nullptr;}
                virtual const DtBase* VDtGet(const string& aType) override { return &mData;}
                // Local
                void updateData(const T& aData);
            public:
                T mData;
        };


#if 0
        /** @brief Managed agent observer
         * */
        // TODO seems not used anymore, remove?
        class MagObs : public MObserver {
            public:
                MagObs(ASdc* aHost): mHost(aHost), mOcp(this) {}
                virtual ~MagObs() { }
                // From MObserver
                virtual string MObserver_Uid() const { return mHost->getUidC<MObserver>("MagObs");}
                virtual MIface* MObserver_getLif(TIdHash aId) override { return nullptr;}
                virtual void onObsOwnerAttached(MObservable* aObl) override {}
                virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override {
                    //mHost->notifyMaps();
                }
                virtual void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override {
                    //mHost->notifyMaps();
                }
                virtual void onObsContentChanged(MObservable* aObl, const string& aCont) override {
                    //mHost->notifyMaps();
                }
                virtual void onObsChanged(MObservable* aObl) override {
                    //mHost->notifyMaps();
                }
                void onObsEvent(MObservable* aObl, const MEvent* aEvent) override {
                }
                TCp* observerCp() override { return &mOcp;}
            public:
                TObserverCp mOcp;               /*!< Observer connpoint */
            private:
                ASdc* mHost;
        };
#endif

        /** @brief Mag data observer
         * */
        class MagDobs : public MObserver {
            public:
                /** @brief Event to be observed */
                enum EObs{
                    EO_ATCH = 0x01,
                    EO_DTCH = 0x02,
                    EO_CNT = 0x04,
                    EO_CHG = 0x08,
                    EO_ALL = EO_ATCH | EO_DTCH | EO_CNT | EO_CHG,
                };
            public:
                MagDobs(ASdc* aHost, int aMask = EO_ALL): mHost(aHost), mNuo(nullptr), mMask(aMask), mOcp(this) {}
                virtual ~MagDobs() { }
                void updateNuo(MNode* aNuo);
                // From MObserver
                virtual string MObserver_Uid() const {return mHost->getUidC<MObserver>(mNuo->name());}
                virtual MIface* MObserver_getLif(TIdHash aId) override { return nullptr;}
                virtual void onObsOwnerAttached(MObservable* aObl) override {}
                virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override {
                    //if (mMask & EO_ATCH) mHost->notifyOutp();
                }
                virtual void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override;
                virtual void onObsContentChanged(MObservable* aObl, const string& aCont) override {
                    //if (mMask & EO_CNT) mHost->notifyOutp();
                }
                virtual void onObsChanged(MObservable* aObl) override {
                    //if (mMask & EO_CHG) mHost->notifyOutp();
                }
                void onObsEvent(MObservable* aObl, const MEvent* aEvent) override {
                    if (aEvent->mId == TNodeEventChanged::idHash) {
                        mHost->notifyOutp();
                    }
                }

                TCp* observerCp() override { return &mOcp;}
            public:
                MNode* mNuo;                   /*!< Node under observation */
                int mMask;                     /*!< Events mask, EObs */
                TObserverCp mOcp;               /*!< Observer connpoint */
            private:
                ASdc* mHost;
        };

    public:
        static const char* Type() { return "ASdc";};
        ASdc(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
        virtual ~ASdc();
        // From Base
        virtual MIface* MNode_getLif(TIdHash aId) override;
        // From MOwned
        MIface* MOwned_getLif(TIdHash aId) override;
        // From MDesSyncable
        string MDesSyncable_Uid() const override {return getUid<MDesSyncable>();}
        void MDesSyncable_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
        MIface* MDesSyncable_getLif(TIdHash aId) override { return nullptr; }
        void update() override;
        void confirm() override;
        void setUpdated() override;
        void setActivated() override;
        bool isActive() const override { return false;}
        int countOfActive(bool aLocal = false) const override { return 1;}
        TDesSyncableCp* desSyncableCp() override { return &mSyncCp;}
        // From MDesObserver
        virtual string MDesObserver_Uid() const override {return getUid<MDesObserver>();}
        virtual void MDesObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
        virtual void onActivated(MDesSyncable* aComp) override;
        virtual void onUpdated(MDesSyncable* aComp) override;
        // From MObserver
        virtual string MObserver_Uid() const {return getUid<MObserver>();}
        virtual MIface* MObserver_getLif(TIdHash aId) override;
        virtual void onObsOwnerAttached(MObservable* aObl) override {}
        virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
        virtual void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override;
        virtual void onObsContentChanged(MObservable* aObl, const string& aCont) override;
        virtual void onObsChanged(MObservable* aObl) override;
        void onObsEvent(MObservable* aObl, const MEvent* aEvent) override;
        MObserver::TCp* observerCp() override { return &mObrCp;}
        // From MDesInpObserver
        void onInpUpdated() override { setActivated(); }
        string MDesInpObserver_Uid() const override {return getUid<MDesInpObserver>(); }
        void MDesInpObserver_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
        // From Node.MOwned
        virtual void onOwnerAttached() override;
        // From MSystExploring
        string MSystExploring_Uid() const override  {return getUid<MSystExploring>();}
        MIface* MSystExploring_getLif(TIdHash aId) override { return nullptr;}
        void onMagChanged() override;
        MSystExploring::TCp* getCp() override { return &mExploringCp;}
    public:
        bool registerIap(SdcIapb* aIap);
        bool registerPap(SdcPapb* aPap);
    protected:
        void Construct() override;
        template<typename T> bool GetInpSdata(const string aInpUri, T& aRes);
        template<typename T> bool GetInpData(const string aInpUri, T& aRes);
        void getOut(Sdata<bool>& aData);
        // Utilities
        MVert* addInput(const string& aName);
        MVert* addOutput(const string& aName);
        MSystExplorable* getExplorable();
        // Local
        virtual bool doCtl() { return false;}
        /** @brief Gets status of the query */
        virtual bool getState(bool aConf = false) {return false;}
        void notifyMaps();
        void notifyOutp();
        void UpdateMag();
        bool areInpsValid() const;
    protected:
        vector<SdcIapb*> mIaps; /*!< Input adapters registry */
        vector<SdcPapb*> mPaps; /*!< Param adapters registry */
        TObserverCp mObrCp;               /*!< Observer connpoint */
        TDesSyncCp mSyncCp; //<! DES Syncable connpoint
        TExplCp mExploringCp; /*! Exploring Cp */
        MNode* mMag; /*!< Managed agent */
        bool mUpdNotified;  //<! Sign of that State notified observers on Update
        bool mActNotified;  //<! Sign of that State notified observers on Activation
        ASdc::SdcIapEnb mIapEnb; /*!< "Enable" input access point */
        ASdc::SdcPap<Sdata<bool>> mOapOut; /*!< Controlling status access point */
        //MagObs mMagObs;             /*!< MAG observer */
        bool mCdone;               /*!<  Sign that controlling was completed, ref ds_dcs_sdc_dsgn_cc */
        bool mOutCInv = true;      //!< Sign of output data cache invalidated
        MDesSyncable* mMDesSyncable = nullptr;
        MDesObserver* mMDesObserver = nullptr;
        MObserver* mMObserver = nullptr;
        MDesInpObserver* mMDesInpObserver = nullptr;
        MSystExploring* mMSystExploring = nullptr;
};

    template <typename T>
void ASdc::SdcPapc<T>::updateData(const T& aData)
{
    if (aData != mData) {
        mData = aData;
        NotifyInpsUpdated();
    }
}

/** @brief SDC agent "Mutate"
 * Performs generic mutation
 * */
class ASdcMut : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcMut"sv;}
        ASdcMut(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapTarg; /*!< "Target" input access point, URI */
        ASdc::SdcIap<DChr2> mIapMut; /*!< "Mutation" input access point, Chromo2 */
        bool mMutApplied; /*!< Indicatio of mut has been applied successfully */
};


/** @brief SDC agent "Adding Component"
 * */
class ASdcComp : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcComp"sv;}
        ASdcComp(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
        void onOwnerAttached() override;
        // From MObserver
        void onObsEvent(MObservable* aObl, const MEvent* aEvent) override;
        void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
        void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapName; /*!< "Name" input access point */
        ASdc::SdcIap<Sdata<string>> mIapParent; /*!< "Parent" input access point */
};

/** @brief SDC agent "Adding Component into target"
 * */
class ASdcCompT : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcCompT"sv;}
        ASdcCompT(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
        // From MObserver
        virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
        virtual void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override;
    protected:
        ASdc::SdcIap<DGuri> mIapTarg; /*!< "Target" input access point */
        ASdc::SdcIap<Sdata<string>> mIapName; /*!< "Name" input access point */
        ASdc::SdcIap<Sdata<string>> mIapParent; /*!< "Parent" input access point */
};


/** @brief SDC agent "Removing Component"
 * */
class ASdcRm : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcRm"sv;}
        ASdcRm(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
        // From MObserver
        virtual void onObsOwnedAttached(MObservable* aObl, MOwned* aOwned) override;
        virtual void onObsOwnedDetached(MObservable* aObl, MOwned* aOwned) override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapName; /*!< "Name" input access point */
};



/** @brief SDC agent "Connect"
 * */
class ASdcConn : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcConn"sv;}
        ASdcConn(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapV1; /*!< "V1" input access point */
        ASdc::SdcIap<Sdata<string>> mIapV2; /*!< "V2" input access point */
        NodeCreationObserver mNco1;
        NodeCreationObserver mNco2;
};

/** @brief SDC agent "Connect targeted"
 * */
class ASdcConnT : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcConnT"sv;}
        ASdcConnT(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<DGuri> mIapTarg; /*!< "Target" input access point */
        ASdc::SdcIap<DGuri> mIapV1; /*!< "V1" input access point */
        ASdc::SdcIap<DGuri> mIapV2; /*!< "V2" input access point */
        NodeCreationObserver mNco1;
        NodeCreationObserver mNco2;
};


/** @brief SDC agent "Disconnect"
 * */
class ASdcDisconn : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcDisconn"sv;}
        ASdcDisconn(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapV1; /*!< "V1" input access point */
        ASdc::SdcIap<Sdata<string>> mIapV2; /*!< "V2" input access point */
};

/** @brief SDC agent "Insert node into list, ver. 2. Insertion before the chain given node"
 * TODO The curret approach for insertion is wrong: the element is inserted BEFORE
 * given chain node (position node). But the insertion causes the change of position of this given chain node
 * so getting status of the control operation results to false. We need to use opposite approach -
 * insert AFTER given chain node. In this case the position of this given node is not changed.
 * Actually even the current approach works ok if the given chan node is specified basing on the position from end.
 * But in many cases this is rather inconvenient. For instance in the colums chain in column layout the position of
 * the columnn is calculated from start to end.
 * */
class ASdcInsert2 : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcInsert2"sv;}
        ASdcInsert2(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From MObserver
        virtual void onObsChanged(MObservable* aObl) override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        bool isBindedToEnd(MVert* aCurPrevv, const MVert* aEndNextv, const GUri& aUriP, const GUri& aUriN);
    protected:
        ASdc::SdcIap<Sdata<string>> mIapName; /*!< "Link name" input access point */
        ASdc::SdcIap<Sdata<string>> mIapPrev; /*!< "Prev CP" input access point */
        ASdc::SdcIap<Sdata<string>> mIapNext; /*!< "Next CP" input access point */
        ASdc::SdcIap<Sdata<string>> mIapPname; /*!< "Position - name" input access point */
        ASdc::MagDobs mDobsNprev; /*!< "Link prev CP" observation */
        MVert* mCpPair;
};

#if 0
/** @brief SDC agent "Insert node into list, ver. 3. DS_ISS_013 fixed.
 * TODO Migrate to DES based SDC
 * */
class ASdcInsert3 : public ASdc
{
    public:
        static const char* Type() { return "ASdcInsert3";};
        ASdcInsert3(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        // From MObserver
        virtual void onObsChanged(MObservable* aObl) override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<GUri> mIapStartUri; /*!< "List start" input access point */
        ASdc::SdcIap<GUri> mIapEndUri; /*!< "List end" input access point */
        ASdc::SdcIap<GUri> mIapName; /*!< "Link name" input access point */
        ASdc::SdcIap<GUri> mIapPrev; /*!< "Prev CP" input access point */
        ASdc::SdcIap<GUri> mIapNext; /*!< "Next CP" input access point */
        ASdc::SdcIap<GUri> mIapPname; /*!< "Position - name" input access point */
        ASdc::MagDobs mDobsNprev; /*!< "Link prev CP" observation */
        MVert* mCpPair;
};
#endif

/** @brief SDC agent "Insert node into list AFTER a the chain given node"
 * */
class ASdcInsertN : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcInsertN"sv;}
        ASdcInsertN(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From MObserver
        virtual void onObsChanged(MObservable* aObl) override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapName; /*!< "Link name" input access point */
        ASdc::SdcIap<Sdata<string>> mIapPrev; /*!< "Prev CP" input access point */
        ASdc::SdcIap<Sdata<string>> mIapNext; /*!< "Next CP" input access point */
        ASdc::SdcIap<Sdata<string>> mIapPname; /*!< "Position - name" input access point */
        ASdc::MagDobs mDobsNprev; /*!< "Link prev CP" observation */
        MVert* mCpPair;
};



/** @brief SDC agent "Extract, reverse to Insert"
 * Extract the link from the chain
 * */
class ASdcExtract : public ASdc
{
    public:
        inline static constexpr std::string_view idStr() { return "ASdcExtract"sv;}
        ASdcExtract(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        void Construct() override;
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        ASdc::SdcIap<Sdata<string>> mIapName; /*!< "Link name" input access point */
        ASdc::SdcIap<Sdata<string>> mIapPrev; /*!< "Prev CP" input access point */
        ASdc::SdcIap<Sdata<string>> mIapNext; /*!< "Next CP" input access point */
        MVert* mCpPair;
};

#if 0
/** @brief SDC agent "Pause manageable"
 * */
class ASdcPause : public ASdc
{
    public:
        static const char* Type() { return "ASdcPause";};
        ASdcPause(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        MDesManageable* getDesManageable();
};

/** @brief SDC agent "Resume manageable"
 * */
class ASdcResume : public ASdc
{
    public:
        static const char* Type() { return "ASdcResume";};
        ASdcResume(const string &aType, const string& aName = string(), MEnv* aEnv = NULL);
    protected:
        // From ASdc
        virtual bool getState(bool aConf = false) override;
        bool doCtl() override;
    protected:
        MDesManageable* getDesManageable();
};
#endif






#endif // __FAP3_DESSDC_H
