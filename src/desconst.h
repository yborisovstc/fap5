#ifndef __FAP5_DESCONST_H
#define __FAP5_DESCONST_H


class Const_SI : public Const
{
    public:
	Const_SI(const string &aType, const string& aName, MEnv* aEnv, const string& aSdata): Const(aType, aName, aEnv) {
	    mData = CreateData("SI"); mData->FromString(aSdata);
	}
};

class Const_SB : public Const
{
    public:
	Const_SB(const string &aType, const string& aName, MEnv* aEnv, const string& aSdata): Const(aType, aName, aEnv) {
	    mData = CreateData("SB"); mData->FromString(aSdata);
	}
};


class SI_0 : public Const_SI
{
    public:
	inline static constexpr std::string_view idStr() { return "SI_0"sv;}
	static const char* Type() { return "SI_0";};
	SI_0(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): Const_SI(aType, aName, aEnv, "SI 0") { }
};

class SI_1 : public Const_SI
{
    public:
	inline static constexpr std::string_view idStr() { return "SI_1"sv;}
	SI_1(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): Const_SI(aType, aName, aEnv, "SI 1") { }
};

class SB_True : public Const_SB
{
    public:
	inline static constexpr std::string_view idStr() { return "SB_True"sv;}
	SB_True(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): Const_SB(aType, aName, aEnv, "SB true") { }
};

class SB_False : public Const_SB
{
    public:
	inline static constexpr std::string_view idStr() { return "SB_False"sv;}
	SB_False(const string &aType, const string& aName = string(), MEnv* aEnv = NULL): Const_SB(aType, aName, aEnv, "SB false") { }
};



#endif


