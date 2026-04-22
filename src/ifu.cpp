#include <stdlib.h>
#include <stdio.h>
#include <stdexcept> 
#include <sstream>
#include <cassert>
#include "ifu.h" 


char Ifu::KEsc = '\\';
char Ifu::KUidSep = '%';
char Ifu::KUidSepIc = '-';
char Ifu::KRinvSep = ',';
int  Ifu::KDumpIndent = 3;
string Ifu::K_SpName_Ns = "_@";
string Ifu::K_SpName_Nil = "_";
string Ifu::KArraySep = ";";
 
Ifu::Ifu()
{
}

size_t Ifu::FindFirstCtrl(const string& aString, const char aCtrl, size_t aPos)
{
    size_t pos_beg = aPos;
    size_t pos;
    do {
	pos = aString.find_first_of(aCtrl, pos_beg); 
	pos_beg = pos + 1;
    } while(pos != string::npos && aString.at(pos - 1) == KEsc);
    return pos;
}

size_t Ifu::FindFirstCtrl(const string& aString, const string& aCtrls, size_t aPos)
{
    size_t pos_beg = aPos;
    size_t pos;
    do {
	char ctrl = 0x00;
	pos = aString.find_first_of(aCtrls, pos_beg); 
	pos_beg = pos + 1;
    } while(pos != string::npos && aString.at(pos - 1) == KEsc);
    return pos;
}

size_t Ifu::FindRightDelim(const string& aString, char aLeftDelim, char aRightDelim, size_t aPos)
{
    size_t res = string::npos;
    size_t pos_beg = aPos, pos = 0;
    int level = 0;
    do {
	string delims(1, aLeftDelim); delims += aRightDelim;
	pos = aString.find_first_of(delims, pos_beg);
	if (pos != string::npos) {
	    char delim =  aString.at(pos);
	    if (delim == aLeftDelim) {
		level++;
	    } else {
		if (level == 0) {
		    res = pos;
		} else {
		    level--;
		}
	    }
	    pos_beg = pos + 1;
	}
    } while (pos != string::npos && res == string::npos);
    return pos;
}

void Ifu::offset(int aIndent, ostream& aOs)
{
    for (int i = 0; i < aIndent; i++)  aOs << " ";
}

void Ifu::ParseIcSpec(const string& aSpec, string& aName, string& aSig, vector<string>& aArgs) 
{
    // Separate method name
    size_t mname_beg = 0;
    size_t mname_end = aSpec.find_first_of(KRinvSep, mname_beg); 
    if (mname_end == mname_beg) throw (runtime_error("Missing method name"));
    aName = aSpec.substr(mname_beg, (mname_end == string::npos) ? string::npos : mname_end - mname_beg);
    if (mname_end != string::npos) {
	// Separate signature
	size_t sig_beg = mname_end + 1;
	size_t sig_end = aSpec.find_first_of(KRinvSep, sig_beg); 
	aSig = aSpec.substr(sig_beg, (sig_end == string::npos) ? string::npos : sig_end - sig_beg);
	if (sig_end != string::npos) {
	    // Separate arguments
	    size_t arg_end = sig_end;
	    size_t arg_beg = arg_end + 1;
	    do {
		arg_beg = arg_end + 1;
		size_t arg_mid = arg_beg;
		// Find first non-escaped separator
		do {
		    arg_end = aSpec.find_first_of(KRinvSep, arg_mid); 
		    arg_mid = arg_end + 1;
		} while(arg_end != string::npos && aSpec.at(arg_end - 1) == KEsc);
		string arg = aSpec.substr(arg_beg, (arg_end == string::npos) ? string::npos : arg_end - arg_beg);
		aArgs.push_back(DeEscCtrl(arg, KRinvSep));
	    } while (arg_end != string::npos);
	}
    }
}

string Ifu::EscCtrl(const string& aInp, char aCtrl)
{
    string res;
    for (string::const_iterator it = aInp.begin(); it != aInp.end(); it++) {
	const char cc = *it;
	if (cc == aCtrl) {
	    res.push_back(KEsc);
	}
	res.push_back(cc);
    }
    return res;
}

string Ifu::DeEscCtrl(const string& aInp, char aCtrl)
{
    string res;
    for (string::const_iterator it = aInp.begin(); it != aInp.end(); it++) {
	const char cc = *it;
	const char cn = ((it + 1) != aInp.end()) ? *(it +1) : 0x0;
	if (cc == KEsc && cn == aCtrl) {
	    continue;
	}
	res.push_back(cc);
    }
    return res;
}

string Ifu::CombineIcSpec(const string& aName, const string& aSig)
{
    return aName + KRinvSep + aSig;
}

string Ifu::CombineIcSpec(const string& aName, const string& aSig, const string& aArg)
{
    return aName + KRinvSep + aSig + KRinvSep + EscCtrl(aArg, KRinvSep);
}

bool Ifu::ToBool(const string& aString)
{
    bool res = false;
    if (aString == "false") res = false;
    else if (aString == "true") res = true;
    else throw (runtime_error("Incorrect boolean value: " + aString));
    return res;
}

string Ifu::FromBool(bool aBool)
{
    return aBool ? "true" : "false";
}

int Ifu::ToInt(const string& aString)
{
    int res = 0;
    stringstream ss(aString);
    ss >> res;
    return res;
}

string Ifu::FromInt(int aInt)
{
    stringstream ss;
    ss << aInt;
    return ss.str();
}

void Ifu::ToStringArray(const string& aString, vector<string>& aRes)
{
    size_t end = 0;
    size_t beg = end + 1;
    do {
	beg = end + 1;
	size_t mid = beg;
	// Find first non-escaped separator
	do {
	    end = aString.find_first_of(KArraySep, mid); 
	    mid = end + 1;
	} while (end != string::npos && aString.at(end - 1) == KEsc);
	string elem = aString.substr(beg, (end == string::npos) ? string::npos : end - beg);
	aRes.push_back(elem);
    } while (end != string::npos);
}

void Ifu::ParseUid(const string& aUid, string& aOid, string& aType)
{
    size_t oid_beg = 0, oid_end = 0;
    oid_end = aUid.find_first_of(KUidSep, oid_beg); 
    aOid = aUid.substr(oid_beg, (oid_end == string::npos) ? string::npos : oid_end - oid_beg);
    if (oid_end != string::npos) {
	size_t type_beg = oid_end + 1;
	aType = aUid.substr(type_beg);
    }
}

void Ifu::CombineUid(const string& aOid, const string& aType, string& aUid)
{
    aUid = aOid + KUidSep + aType;
}

bool Ifu::IsSimpleIid(const string& aIid)
{
    size_t sep = aIid.find_first_of(KUidSep); 
    return (sep == string::npos);
}






/// Ife

void Ife::RegMethod(const string& aName, int aArgsNum)
{
    assert(mMpars.count(aName) == 0);
    mMpars.insert(pair<string, int>(aName, aArgsNum));
}

bool Ife::CheckMpars(const string& aName, int aArgsNum) const
{
    bool res = mMpars.at(aName) == aArgsNum;
    return res;
}

bool Ife::CheckMname(const string& aName) const
{
    bool res = mMpars.count(aName) > 0;
    return res;
}


