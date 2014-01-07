/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: TestCharacteristic.h
*  Author:   Robert Goor
*
*/
//
//  classes TestCharacteristic and TestCharacteristicSet test individual characteristics and sets of characteristics against 
//  various rules to determine acceptability.  The tests return -1 for unacceptable, 1 for acceptable and 0 for marginal.
//  Eventually, these classes may permit scripts to easily change what is tested and how, and to document those changes, at
//  run time.
//

#ifndef _TESTCHARACTERISTIC_H_
#define _TESTCHARACTERISTIC_H_

#include "rgpersist.h"
#include "rgdefs.h"


//
// Default values (unless changed through initialization or reset):
// For STRTestCharacteristic:
//     Minimum sample value acceptable (in RFU), MinimumRFU = 150.0
//     The minimum fit level acceptable for a "normal" peak, MinimumFitForNormalPeak = 0.90
//     The fit level below which a test for artifact signature is mandatory, TriggerFitForArtifactTest = 0.9995
//     Whether or not to test for a negative component in a fit curve, TestForNegative = TRUE
//     The maximum sample level acceptable (in RFU), MaximumRFU = 10000.0
// For STRTestCharacteristicSet:
//     The minimum fraction of average widths of set acceptable, MinimumFractionOfAverageWidth = 0.25
//     The maximum multiple of average widths of set acceptable, MaximumMultipleOfAverageWidth = 2.0
//     The minimum fraction of average of peaks of set acceptable, MinimumFractionOfAveragePeak = 0.333)
//     The maximum multiple of average of peaks of set acceptable, or, if negative, no test, MaximumMultipleOfAveragePeak = -1.0
//


class RGDList;
class DataSignal;
class RGString;
class RGTextOutput;

const int _STRTESTCHARACTERISTIC_ = 1014;
const int _STRTESTSAMPLECHARACTERISTIC_ = 1015;
const int _STRTESTCONTROLCHARACTERISTIC_ = 1016;
const int _STRTESTSAMPLECHARACTERISTICSET_ = 1017;
const int _STRTESTCONTROLCHARACTERISTICSET_ = 1018;

PERSISTENT_PREDECLARATION (STRTestCharacteristic)
PERSISTENT_PREDECLARATION (STRTestSampleCharacteristic)
PERSISTENT_PREDECLARATION (STRTestControlCharacteristic)
PERSISTENT_PREDECLARATION (STRTestSampleCharacteristicSet)
PERSISTENT_PREDECLARATION (STRTestControlCharacteristicSet)



class TestCharacteristic : public RGPersistent {

ABSTRACT_DECLARATION (TestCharacteristic)

public:
	TestCharacteristic ();
	virtual ~TestCharacteristic ();

	virtual int Test (DataSignal* testSignal, double minimumHeight, double maximumHeight) = 0;
	virtual int TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight) = 0;
	virtual int TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) = 0;

	virtual void SetTestForNegative (bool test) = 0;

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	static void SetGlobalTestForNegative (bool test) { globalTestForNegative = test; }
	static bool GetGlobalTestForNegative () { return globalTestForNegative; }

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	static bool globalTestForNegative;
};


class TestControlCharacteristic : public TestCharacteristic {

ABSTRACT_DECLARATION (TestControlCharacteristic)

public:
	TestControlCharacteristic ();
	virtual ~TestControlCharacteristic ();

	virtual int Test (DataSignal* testSignal, double minimumHeight, double maximumHeight) = 0;
	virtual int TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight) = 0;
	virtual int TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) = 0;

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;
};


class TestSampleCharacteristic : public TestCharacteristic {

ABSTRACT_DECLARATION (TestSampleCharacteristic)

public:
	TestSampleCharacteristic ();
	virtual ~TestSampleCharacteristic ();

	virtual int Test (DataSignal* testSignal, double minimumHeight, double maximumHeight) = 0;
	virtual int TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight) = 0;
	virtual int TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) = 0;

	virtual size_t StoreSize () const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;
};


class TestCharacteristicSet : public RGPersistent {

ABSTRACT_DECLARATION (TestCharacteristicSet)

public:
	TestCharacteristicSet ();
	virtual ~TestCharacteristicSet ();

	virtual int Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following) = 0;
	virtual int Test (RGDList& set, double& correlation) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) = 0;
	virtual void PrecomputeAverages (RGDList& set) = 0;
	virtual const double* GetIdealSet () const = 0;

	virtual int GetSetSize () const { return 0; }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;
};


class TestControlCharacteristicSet : public TestCharacteristicSet {

ABSTRACT_DECLARATION (TestControlCharacteristicSet)

public:
	TestControlCharacteristicSet ();
	~TestControlCharacteristicSet ();

	virtual int Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following) = 0;
	virtual int Test (RGDList& set, double& correlation) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) = 0;
	virtual void PrecomputeAverages (RGDList& set) = 0;
	virtual const double* GetIdealSet () const = 0;

	virtual int GetSetSize () const { return 0; }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;
};


class TestSampleCharacteristicSet : public TestCharacteristicSet {

ABSTRACT_DECLARATION (TestSampleCharacteristicSet)

public:
	TestSampleCharacteristicSet ();
	~TestSampleCharacteristicSet ();

	virtual int Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following) = 0;
	virtual int Test (RGDList& set, double& correlation) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) = 0;
	virtual void PrecomputeAverages (RGDList& set) = 0;
	virtual const double* GetIdealSet () const = 0;

	virtual int GetSetSize () const { return 0; }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;
};


class STRTestCharacteristic : public RGPersistent {

PERSISTENT_DECLARATION (STRTestCharacteristic)

public:
	STRTestCharacteristic ();
	STRTestCharacteristic (double minRFU, double minFitForNormal, double triggerForArtifact, bool testForNeg, double maxRFU);
	virtual ~STRTestCharacteristic ();

	void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim);

	void SetMinimumRFU (double rfu) { MinimumRFU = rfu; }
	double GetMinimumRFU () const { return MinimumRFU; }

	void SetMinimumFitForNormalPeak (double minFit) { MinimumFitForNormalPeak = minFit; }
	double GetMinimumFitForNormalPeak () const; // { return MinimumFitForNormalPeak; }

	void SetTriggerFitForArtifactTest (double triggerFit) { TriggerFitForArtifactTest = triggerFit; }
	double GetTriggerFitForArtifactTest () const; // { return TriggerFitForArtifactTest; }

	void SetTestForNegative (bool test) { TestForNegative = test; }
	bool GetTestForNegative () const { return TestForNegative; }

	void SetMaximumRFU (double rfu) { MaximumRFU = rfu; }
	double GetMaximumRFU () const { return MaximumRFU; }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	double MinimumRFU;
	double MinimumFitForNormalPeak;
	double TriggerFitForArtifactTest;
	bool TestForNegative;
	double MaximumRFU;
};


class STRTestCharacteristicSet : public TestControlCharacteristicSet {

ABSTRACT_DECLARATION (STRTestCharacteristicSet)

public:
	STRTestCharacteristicSet ();
	STRTestCharacteristicSet (double minWidth, double maxWidth, double minPeak, double maxPeak);
	~STRTestCharacteristicSet ();

	virtual int Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following) = 0;
	virtual int Test (RGDList& set, double& correlation) = 0;
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim);
	virtual void PrecomputeAverages (RGDList& set);
	virtual const double* GetIdealSet () const { return NULL; }

	void SetMinimumFractionOfAverageWidth (double fraction) { MinimumFractionOfAverageWidth = fraction; }
	double GetMinimumFractionOfAverageWidth () const { return MinimumFractionOfAverageWidth; }

	void SetMaximumMultipleOfAverageWidth (double multiple) { MaximumMultipleOfAverageWidth = multiple; }
	double GetMaximumMultipleOfAverageWidth () const { return MaximumMultipleOfAverageWidth; }

	void SetMinimumFractionOfAveragePeak (double fraction) { MinimumFractionOfAveragePeak = fraction; }
	double GetMinimumFractionOfAveragePeak () const { return MinimumFractionOfAveragePeak; }

	void SetMaximumMultipleOfAveragePeak (double multiple) { MaximumMultipleOfAveragePeak = multiple; }
	double GetMaximumMultipleOfAveragePeak () const { return MaximumMultipleOfAveragePeak; }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	double MinimumFractionOfAverageWidth;
	double MaximumMultipleOfAverageWidth;
	double MinimumFractionOfAveragePeak;
	double MaximumMultipleOfAveragePeak;

	double averageWidth;
	double averagePeak;
};


class STRTestSampleCharacteristic : public TestSampleCharacteristic {

PERSISTENT_DECLARATION (STRTestSampleCharacteristic)

public:
	STRTestSampleCharacteristic ();
	STRTestSampleCharacteristic (double minRFU, double minFitForNormal, double triggerForArtifact, bool testForNeg, double maxRFU);
	STRTestSampleCharacteristic (STRTestCharacteristic* test);
	virtual ~STRTestSampleCharacteristic ();

	virtual int Test (DataSignal* testSignal, double minimumHeight, double maximumHeight);
	virtual int TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight);
	virtual int TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight);
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim);

	void SetMinimumRFU (double rfu) { testBody->SetMinimumRFU (rfu); }
	double GetMinimumRFU () const { return testBody->GetMinimumRFU (); }

	void SetMinimumFitForNormalPeak (double minFit) { testBody->SetMinimumFitForNormalPeak (minFit); }
	double GetMinimumFitForNormalPeak () const { return testBody->GetMinimumFitForNormalPeak (); }

	void SetTriggerFitForArtifactTest (double triggerFit) { testBody->SetTriggerFitForArtifactTest (triggerFit); }
	double GetTriggerFitForArtifactTest () const { return testBody->GetTriggerFitForArtifactTest (); }

	virtual void SetTestForNegative (bool test) { testBody->SetTestForNegative (test); }
	bool GetTestForNegative () const { return testBody->GetTestForNegative (); }

	void SetMaximumRFU (double rfu) { testBody->SetMaximumRFU (rfu); }
	double GetMaximumRFU () const { return testBody->GetMaximumRFU (); }


	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	STRTestCharacteristic* testBody;
	Boolean ExternalTest;
};


class STRTestControlCharacteristic : public TestControlCharacteristic {

PERSISTENT_DECLARATION (STRTestControlCharacteristic)

public:
	STRTestControlCharacteristic ();
	STRTestControlCharacteristic (double minRFU, double minFitForNormal, double triggerForArtifact, bool testForNeg, double maxRFU);
	STRTestControlCharacteristic (STRTestCharacteristic* test);
	virtual ~STRTestControlCharacteristic ();

	virtual int Test (DataSignal* testSignal, double minimumHeight, double maximumHeight);
	virtual int TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight);
	virtual int TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight);
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim);

	void SetMinimumRFU (double rfu) { testBody->SetMinimumRFU (rfu); }
	double GetMinimumRFU () const { return testBody->GetMinimumRFU (); }

	void SetMinimumFitForNormalPeak (double minFit) { testBody->SetMinimumFitForNormalPeak (minFit); }
	double GetMinimumFitForNormalPeak () const { return testBody->GetMinimumFitForNormalPeak (); }

	void SetTriggerFitForArtifactTest (double triggerFit) { testBody->SetTriggerFitForArtifactTest (triggerFit); }
	double GetTriggerFitForArtifactTest () const { return testBody->GetTriggerFitForArtifactTest (); }

	virtual void SetTestForNegative (bool test) { testBody->SetTestForNegative (test); }
	bool GetTestForNegative () const { return testBody->GetTestForNegative (); }

	void SetMaximumRFU (double rfu) { testBody->SetMaximumRFU (rfu); }
	double GetMaximumRFU () const { return testBody->GetMaximumRFU (); }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	STRTestCharacteristic* testBody;
	Boolean ExternalTest;
};


class STRTestSampleCharacteristicSet : public TestSampleCharacteristicSet {

PERSISTENT_DECLARATION (STRTestSampleCharacteristicSet)

public:
	STRTestSampleCharacteristicSet ();
//	STRTestSampleCharacteristicSet (double minWidth, double maxWidth, double minPeak, double maxPeak);
	~STRTestSampleCharacteristicSet ();

	virtual int Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following);
	virtual int Test (RGDList& set, double& correlation);
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim);

	virtual void PrecomputeAverages (RGDList& set) {}
	virtual const double* GetIdealSet () const { return NULL; }

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;
};


class STRTestControlCharacteristicSet : public STRTestCharacteristicSet {

PERSISTENT_DECLARATION (STRTestControlCharacteristicSet)

public:
	STRTestControlCharacteristicSet ();
	STRTestControlCharacteristicSet (double minWidth, double maxWidth, double minPeak, double maxPeak, double* idealArray, int N);
	STRTestControlCharacteristicSet (double* idealArray, int N);
	~STRTestControlCharacteristicSet ();

	virtual int Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following);
	virtual int Test (RGDList& set, double& correlation);
	virtual void Report (RGTextOutput& text, const RGString& indent, const RGString& Delim);
	virtual const double* GetIdealSet () const;
	virtual int GetSetSize () const;

	virtual size_t StoreSize () const;
//	virtual int CompareTo (const RGPersistent*) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	double* IdealBPArray;
	int NElements;
	int NElements1;
	double* DifferenceArray;
	double DifferenceNorm;
	DataSignal* previous;

	double VectorDotProductWithDifferences (double* a);
};

#endif  /*  _TESTCHARACTERISTIC_H_  */

