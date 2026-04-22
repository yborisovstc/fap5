#ifndef __FAP5_IFU_H
#define __FAP5_IFU_H

#include <vector>
#include <string>
#include <sstream>
#include <map>


using namespace std;

/** @brief Interface support utility
 * */
class Ifu
{
    public:
	// Dump masks
	enum TDm {
	    EDM_Recursive = 0x01,
	    EDM_Base = 0x02,
	    EDM_Comps = 0x04,
	    EDM_Opt1 = 0x10,
	    EDM_Opt2 = 0x20,
	    EDM_Opt3 = 0x40,
	};
    public:
	Ifu();
	static size_t FindFirstCtrl(const string& aString, const char aCtrl, size_t aPos); 
	static size_t FindFirstCtrl(const string& aString, const string& aCtrls, size_t aPos); 
	static size_t FindRightDelim(const string& aString, char LeftDelim, char RightDelim, size_t aPos);
	static void offset(int aIndent, ostream& aOs);
        // Parsing of Iface call invocation spec
        // TODO Introduce IcSpec class instead
        static void ParseIcSpec(const string& aSpec, string& aName, string& aSig, vector<string>& aArgs);
        static string DeEscCtrl(const string& aInp, char aCtrl);
        // IcSpec
	static string CombineIcSpec(const string& aName, const string& aSig);
	static string CombineIcSpec(const string& aName, const string& aSig, const string& aArg);
        // Pack/unpack
        template<typename TArg> static string Pack(TArg aArg);
        template<typename TArg> static TArg Unpack(const string& aString, TArg& aArg);
        inline static string PackMethod(const string& aName);
        template<typename TArg1> static string PackMethod(const string& aName, TArg1 aAgr1);
        template<typename TArg1, typename TArg2> static string PackMethod(const string& aName, TArg1 aAgr1, TArg2 aArg2);
        template<typename TArg1, typename TArg2, typename TArg3> static string PackMethod(const string& aName, TArg1 aAgr1, TArg2 aArg2, TArg3 aArg3);
        template<typename TArg1, typename TArg2, typename TArg3, typename TArg4>
            static string PackMethod(const string& aName, TArg1 aAgr1, TArg2 aArg2, TArg3 aArg3, TArg4 aArg4);
        // Converting
	static bool ToBool(const string& aString);
	static string FromBool(bool aBool);
	static int ToInt(const string& aString);
	static string FromInt(int aInt);
	static void ToStringArray(const string& aString, vector<string>& aRes);
        // UID
        static void ParseUid(const string& aUid, string& aOid, string& aType);
        static void CombineUid(const string& aOid, const string& aType, string& aUid);
        static bool IsSimpleIid(const string& aIid);
        // Escape seq
	static string EscCtrl(const string& aInp, char aCtrl);


    public:
        static char KEsc;
        static char KUidSep;
        static char KUidSepIc;  /*!< UID separator of internal component (not native hier) */
        static char KRinvSep;
	static string KArraySep;
        static int  KDumpIndent;
        static string K_SpName_Ns;
        static string K_SpName_Nil;
};


inline string Ifu::PackMethod(const string& aName)
{
    string res = CombineIcSpec(aName, "1");
    return res;
}

template<typename TArg1> string Ifu::PackMethod(const string& aName, TArg1 aArg1)
{
    string res = CombineIcSpec(aName, "1");
    AddIcSpecArg(res, Pack(aArg1));
    return res;
}

template<typename TArg1, typename TArg2> string Ifu::PackMethod(const string& aName, TArg1 aArg1, TArg2 aArg2)
{
    string res = Ifu::CombineIcSpec(aName, "1");
    AddIcSpecArg(res, Pack(aArg1));
    AddIcSpecArg(res, Pack(aArg2));
    return res;
}

template<typename TArg1, typename TArg2, typename TArg3> string Ifu::PackMethod(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3)
{
    string res = Ifu::CombineIcSpec(aName, "1");
    AddIcSpecArg(res, Pack(aArg1));
    AddIcSpecArg(res, Pack(aArg2));
    AddIcSpecArg(res, Pack(aArg3));
    return res;
}

template<typename TArg1, typename TArg2, typename TArg3, typename TArg4> string Ifu::PackMethod(const string& aName, TArg1 aArg1, TArg2 aArg2, TArg3 aArg3, TArg4 aArg4)
{
    string res = Ifu::CombineIcSpec(aName, "1");
    AddIcSpecArg(res, Pack(aArg1));
    AddIcSpecArg(res, Pack(aArg2));
    AddIcSpecArg(res, Pack(aArg3));
    AddIcSpecArg(res, Pack(aArg4));
    return res;
}




/** @brief Iface utility extention
 * */
struct Ife
{
    void RegMethod(const string& aName, int aArgsNum);
    bool CheckMname(const string& aName) const;
    bool CheckMpars(const string& aName, int aArgsNum) const;
    // Methods parameters
    map<string, int> mMpars;
};


#endif
