#ifndef __FAP5_MDATA_H
#define __FAP5_MDATA_H

#include <set>
#include <vector>
#include <string>
#include "rdata.h"
#include "miface.h"

using namespace std;


/** @brief Interface of getter of variable type data
 * */
class MDVarGet: public MIface
{
    public:
        using TData = vector<const DtBase*>;
    public:
	inline static constexpr std::string_view idStr() { return "MDVarGet"sv;}
	inline static constexpr TIdHash idHash() { return 0x5ee8e840211d938d;}
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	string Uid() const override { return MDVarGet_Uid();}
	void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MDVarGet_doDump(aLevel, aIdt, std::cout);}
	MIface* getLif(TIdHash aId) override { return MDVarGet_getLif(aId); }
	virtual string MDVarGet_Uid() const = 0;
	virtual void MDVarGet_doDump(int aLevel, int aIdt, ostream& aOs) const {}
	virtual MIface* MDVarGet_getLif(TIdHash aId) {return nullptr;}
	// Local
	template <class T> T* GetDObj(T* aInst) { return aInst = static_cast<T*>(DoGetDObj(aInst->Type()));};
	virtual MIface* DoGetDObj(const char *aName) = 0;
	/** @brief Gets an iface id. Is used for type negotiation from root to leafs */
	virtual string VarGetIfid() const = 0;
	/** @brief Gets data by type
	 * */
	virtual bool VDtGet(const DtBase& aData) { return false;}
	virtual const DtBase* VDtGet(const string& aType) { return nullptr;}
	/** @brief Gets the set of data by type. Appends data to aData.
	 * */
	virtual void VDtGet(const string& aType, TData& aData) {
            aData.push_back(VDtGet(aType));
        }
	template<class T> const T* DtGet(const T* aData) { return reinterpret_cast<const T*>(VDtGet(aData->TypeSig()));}
	template<class T> const T* DtGet() { return reinterpret_cast<const T*>(VDtGet(T::TypeSig()));}
};

/** @brief Interface of getter of variable type data
 * */
class MDVarSet: public MIface
{
    public:
	inline static constexpr std::string_view idStr() { return "MDVarSet"sv;}
	inline static constexpr TIdHash idHash() { return 0xb4b2e736046562fb;}
    public:
	// From MIface
	TIdHash id() const override { return idHash();}
	virtual string Uid() const override { return MDVarSet_Uid();}
	virtual string MDVarSet_Uid() const = 0;
	// Local
	virtual const bool VDtSet(const DtBase& aData) { return false;}
	// Get iface id. Is used for type negotiation from root to leafs
	virtual string VarGetSIfid() = 0;
};


#if 0

/** @brief Generic data getter base
 * Temporary solution for ds_rdi
 * */
class MDtGetBase: public MIface
{
    public:
	virtual bool DtbGet(MDtBase& aData) { return false;}
	/** @brief Duplicate (allocate and copy) data
	 * */
	virtual MDtBase* DtbDup() { return nullptr;}
	static const string MDtGetType(const string& aDataType) { return string("MDtGet_") + aDataType; }
};

// Generic data getter

template <class T> class MDtGet: public MDtGetBase
{
    public:
	static const string mType;
	static const char* Type() { return mType.c_str();};
	// From MIface
	virtual string Uid() const override { return MDtGet_Uid();}
	virtual string MDtGet_Uid() const { return mType;}
	virtual void doDump(int aLevel, int aIdt, ostream& aOs) const override { return MDtGet_doDump(aLevel, aIdt, std::cout);}
	virtual void MDtGet_doDump(int aLevel, int aIdt, ostream& aOs) const {}
	// From MDtGetBase
	virtual bool DtbGet(MDtBase& aData) override {
	    bool res = false;
	    T* data = dynamic_cast<T*>(&aData);
	    if (data) {
		DtGet(*data); res = true;
	    }
	    return res;
	}
	virtual MDtBase* DtbDup() override {
	    MDtBase* data = new T; DtbGet(*data); return data;
	}
	// Local
	virtual void DtGet(T& aData) = 0;
};

template<class T> const string MDtGet<T>::mType = string("MDtGet_") + T::TypeSig();


/** @brief Generic data setter interface
 * */
template <class T> class MDtSet: public MIface
{
    public:
	static const string mType;
	static const char* Type() { return mType.c_str();};
	// From MIface
	virtual string Uid() const override { return mType;}
	// Local
	virtual void DtSet(const T& aData) = 0;
};

template<class T> const string MDtSet<T>::mType = string("MDtSet_") + T::TypeSig();

// Matrix
template <class T> class MMtrGet: public MIface
{
    public:
	static const char* Type();
	static const char* TypeSig();
	virtual void MtrGet(Mtr<T>& aData) = 0;
};

template <typename T>
class MVectorGet : public MIface
{
    public:
	static const char* Type();
    public: 
	virtual int Size() const = 0;
	virtual bool GetElem(int aInd, T& aElem) const = 0;
	virtual string MVectorGet_Mid() const = 0;
	virtual MIface* MVectorGet_Call(const string& aSpec, string& aRes) = 0;
	// From MIface
	MIface* Call(const string& aSpec, string& aRes) override {return MVectorGet_Call(aSpec, aRes);}
	string Mid() const override { return MVectorGet_Mid();}
};

#endif

#endif
