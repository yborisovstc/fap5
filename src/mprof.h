#ifndef __FAP5_MPROF_H
#define __FAP5_MPROF_H

#include <string>

/** @brief Profiler interface
 * */

using TPItemId = int; //!< ID item
using TPItemFId = int; //!< ID field

/** @brief Profilers event
 * */
// TODO YB This is more perf indicator variant than event, Pind is more indicators collector than
// indicator itself. To consider redesign
class PEvent
{
public:
    using TId = int; //!< Event identificator type
public:
    static const TId KUndefEid = -1; //!< Undefined event id
    PEvent(TId aId = KUndefEid, const std::string& aDescr = std::string()): mId(aId), mDescr(aDescr) {}
public:
    TId mId = KUndefEid; //!< Event Id
    std::string mDescr; //!< Event description
};

/** @brief Profilers record, base
 * */
class PRec
{
    public:
	PRec(): mEventId(PEvent::KUndefEid) {}
	PRec(PEvent::TId aId): mEventId(aId) {}
	void setEventId(PEvent::TId aId) { mEventId = aId;}
    public:
	PEvent::TId mEventId; //!< Event Id
    public:
	static const char KFieldSep = '\t';
};

/** @brief Performance indicator interface.
 * Inherited class should contain static int constant KTypeId of type TTid with unique value
 */
class MPind
{
public:
    using TClock = struct timespec; //!< Type of clock value
    using TTid = int;               //!< Id of indicator type */
public:
    /** @brief Return type ID */
    virtual TTid tid() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getBufLen() const = 0;
    virtual int getBufLenLimit() const = 0;
    /** Returns file suffix */
    virtual std::string getFileSuf() const = 0;
    /** Obtains clock resolution in nanoseconds */
    virtual bool getClockResolution(TClock& aRes) const =  0;
    virtual std::string recToString(int aRecNum) const = 0;
    virtual std::string toString() const = 0;
    virtual std::string fieldToString(TPItemFId aFId) const = 0;
    virtual bool saveToFile(const std::string& aPath) = 0;
    virtual void dump() const = 0;
    virtual int getId() const = 0;
};

/** @brief Profiler interface
 * */
class MProfiler
{
    public:
	/** @brief Logs entry to code block */
	virtual void enable() = 0;
	/** @brief Saves the metrics collected */
	virtual void saveMetrics() = 0;
	/** @brief Gets Pinds count */
	virtual int pindsCount() const = 0;
	/** @brief Gets pointer to indicator by Id */
	virtual MPind* getPind(int aId) = 0;
	/** @brief Adds pind */
	virtual void setPind(int aId, MPind* aPind) = 0;
	/** @brief Gets pointer to indicator by item type Id and item Id */
	template<typename T> T* getPind(T* aArg) { return reinterpret_cast<T*>(getPind(aArg->KTypeId));}
	template<typename T> T& getPind() { return reinterpret_cast<T&>(*getPind(T::KTypeId));}
        template<typename T> void addPind(const typename T::TIdata& aIdata) {
	    MPind* curPind = getPind(T::KTypeId);
	    if (!curPind) {
		setPind(T::KTypeId, new T(aIdata));
	    } else {
		MProfiler::getPind<T>().addItems(aIdata);
	    }
	}
	// Profiler cluster helper
	template<template<typename> typename C, typename T> T* getPindItem(TPItemId aId) { return getPind<C<T>>().getItem(aId);}
};


#endif
