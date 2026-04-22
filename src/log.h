#ifndef __FAP5_LOG_H
#define __FAP5_LOG_H

#include <sstream>

#include "mlog.h"

/** @brief Log record structure
 * */
class TLog
{
    public:
	static const int KPrecision;
	TLog(int aCtg, const string& aId);
	TLog(int aCtg, const string& aId, const string& aContent);
	TLog(int aCtg, const string& aId, MLogRec* aRecorder): TLog(aCtg, aId) { mRecorder = aRecorder;}
	TLog(int aCtg, const string& aId, const ChromoNode& aMut);
	TLog(const string& aId);
	TLog(const string& aId, const ChromoNode& aMut);
	//TLog(const string& aString);
        ~TLog();
	TLog& operator +(const string& aString);
	operator string() const;
	string TimestampS() const;
	//string NodeUriS() const;
	int MutId() const;
	int Ctg() const { return mCtg;}
	string MutIdS() const;
	string CtgS() const;
	string Content() const { return mContent;};
	ostream& ContentStream() { return mCntStream;};
	void SetCtg(int aCtg);
    protected:
	int mCtg;
	string mTimestampS;
	string mCtgS;
	string mId;
	string mMutIdS;
	string mContent;
        ostringstream mCntStream;
        MLogRec* mRecorder = nullptr;
};



    /** Log recorder
     * */
    class Logrec: public MLogRec
    {
	public:
	    static const char* Type() { return "Logrec";};
	    Logrec(const string& aLogFileName);
	    virtual ~Logrec();
	public:
	    // From MLogRec/MIface
	    virtual string MLogRec_Uid() const override { return Type();}
	    virtual MIface* MLogRec_getLif(const char *aType) override { return nullptr;}
	    virtual void MLogRec_doDump(int aLevel, int aIdt, ostream& aOs) const override {}
	    // From MLogRec
	    virtual void WriteFormat(const char* aFmt,...);
	    virtual void Write(TLogRecCtg aCtg, const MNode* aNode, const char* aFmt,...);
	    virtual void Write(TLogRecCtg aCtg, MNode* aNode, const ChromoNode& aMut, const char* aFmt,...);
	    virtual void Flush();
	    virtual void SetContextMutId(int aMutId = -1);
	    virtual bool AddLogObserver(MLogObserver* aObs);
	    virtual void RemoveLogObserver(MLogObserver* aObs);
	    virtual int GetStat(TLogRecCtg aCtg) const;
	    virtual void Write(const TLog& aRec);
            virtual void SetLevel(int aLevel) override { mLevel = aLevel;}
            virtual bool MeetsLevel(int aLevel) const override { return (aLevel <= mLevel);}
	protected:
	    void WriteRecord(const char* aText);
	    void WriteRecord(const string& aText);
	protected:
	    FILE* iLogFile;
	    string iLogFileName;
	    bool iLogFileValid;
	    MLogObserver* iObs;
	    int mCtxMutId;
	    int mStat[ECtg_Max];
            int mLevel = EAll;
	public:
	    static const int KLogRecBufSize;
    };

#endif
