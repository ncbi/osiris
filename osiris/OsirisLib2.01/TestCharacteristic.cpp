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
*  FileName: TestCharacteristic.cpp
*  Author:   Robert Goor
*
*/
//
//  classes TestCharacteristic and TestCharacteristicSet test individual characteristics and sets of characteristics against 
//  various rules to determine acceptability.  The tests return -1 for unacceptable, 1 for acceptable and 0 for marginal.
//  Eventually, these classes may permit scripts to easily change what is tested and how, and to document those changes, at
//  run time.
//

#include "TestCharacteristic.h"
#include "rgdlist.h"
#include "DataSignal.h"
#include "rgstring.h"
#include "RGTextOutput.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "OutputLevelManager.h"
#include "Notices.h"
#include "SmartNotice.h"
#include "STRSmartNotices.h"
#include "OsirisPosix.h"


bool TestCharacteristic::globalTestForNegative = false;


ABSTRACT_DEFINITION (TestCharacteristic)
ABSTRACT_DEFINITION (TestControlCharacteristic)
ABSTRACT_DEFINITION (TestSampleCharacteristic)
ABSTRACT_DEFINITION (TestCharacteristicSet)
ABSTRACT_DEFINITION (TestControlCharacteristicSet)
ABSTRACT_DEFINITION (TestSampleCharacteristicSet)

PERSISTENT_DEFINITION (STRTestCharacteristic, _STRTESTCHARACTERISTIC_, "STRTestCharacteristic")
PERSISTENT_DEFINITION (STRTestSampleCharacteristic, _STRTESTSAMPLECHARACTERISTIC_, "STRTestSampleCharacteristic")
PERSISTENT_DEFINITION (STRTestControlCharacteristic, _STRTESTCONTROLCHARACTERISTIC_, "STRTestControlCharacteristic")
PERSISTENT_DEFINITION (STRTestSampleCharacteristicSet, _STRTESTSAMPLECHARACTERISTICSET_, "STRTestSampleCharacteristicSet")
PERSISTENT_DEFINITION (STRTestControlCharacteristicSet, _STRTESTCONTROLCHARACTERISTICSET_, "STRTestControlCharacteristicSet")



TestCharacteristic :: TestCharacteristic () : RGPersistent () {

}


TestCharacteristic :: ~TestCharacteristic () {

}


size_t TestCharacteristic :: StoreSize () const {

	return RGPersistent::StoreSize ();
}


void TestCharacteristic :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
}


void TestCharacteristic :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
}


void TestCharacteristic :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
}


void TestCharacteristic :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
}


TestControlCharacteristic :: TestControlCharacteristic () : TestCharacteristic () {

}


TestControlCharacteristic :: ~TestControlCharacteristic () {

}


size_t TestControlCharacteristic :: StoreSize () const {

	return TestCharacteristic::StoreSize ();
}


void TestControlCharacteristic :: RestoreAll (RGFile& f) {

	TestCharacteristic::RestoreAll (f);
}


void TestControlCharacteristic :: RestoreAll (RGVInStream& f) {

	TestCharacteristic::RestoreAll (f);
}


void TestControlCharacteristic :: SaveAll (RGFile& f) const {

	TestCharacteristic::SaveAll (f);
}


void TestControlCharacteristic :: SaveAll (RGVOutStream& f) const {

	TestCharacteristic::SaveAll (f);
}



TestSampleCharacteristic :: TestSampleCharacteristic () : TestCharacteristic () {

}


TestSampleCharacteristic :: ~TestSampleCharacteristic () {

}


size_t TestSampleCharacteristic :: StoreSize () const {

	return TestCharacteristic::StoreSize ();
}


void TestSampleCharacteristic :: RestoreAll (RGFile& f) {

	TestCharacteristic::RestoreAll (f);
}


void TestSampleCharacteristic :: RestoreAll (RGVInStream& f) {

	TestCharacteristic::RestoreAll (f);
}


void TestSampleCharacteristic :: SaveAll (RGFile& f) const {

	TestCharacteristic::SaveAll (f);
}


void TestSampleCharacteristic :: SaveAll (RGVOutStream& f) const {

	TestCharacteristic::SaveAll (f);
}



TestCharacteristicSet :: TestCharacteristicSet () : RGPersistent () {

}



TestCharacteristicSet :: ~TestCharacteristicSet () {

}


size_t TestCharacteristicSet :: StoreSize () const {

	return RGPersistent::StoreSize ();
}


void TestCharacteristicSet :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
}


void TestCharacteristicSet :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
}


void TestCharacteristicSet :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
}


void TestCharacteristicSet :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
}


TestControlCharacteristicSet :: TestControlCharacteristicSet () : TestCharacteristicSet () {

}


TestControlCharacteristicSet :: ~TestControlCharacteristicSet () {

}


size_t TestControlCharacteristicSet :: StoreSize () const {

	return TestCharacteristicSet::StoreSize ();
}


void TestControlCharacteristicSet :: RestoreAll (RGFile& f) {

	TestCharacteristicSet::RestoreAll (f);
}


void TestControlCharacteristicSet :: RestoreAll (RGVInStream& f) {

	TestCharacteristicSet::RestoreAll (f);
}


void TestControlCharacteristicSet :: SaveAll (RGFile& f) const {

	TestCharacteristicSet::SaveAll (f);
}


void TestControlCharacteristicSet :: SaveAll (RGVOutStream& f) const {

	TestCharacteristicSet::SaveAll (f);
}


TestSampleCharacteristicSet :: TestSampleCharacteristicSet () : TestCharacteristicSet () {

}


TestSampleCharacteristicSet :: ~TestSampleCharacteristicSet () {

}


size_t TestSampleCharacteristicSet :: StoreSize () const {

	return TestCharacteristicSet::StoreSize ();
}


void TestSampleCharacteristicSet :: RestoreAll (RGFile& f) {

	TestCharacteristicSet::RestoreAll (f);
}


void TestSampleCharacteristicSet :: RestoreAll (RGVInStream& f) {

	TestCharacteristicSet::RestoreAll (f);
}


void TestSampleCharacteristicSet :: SaveAll (RGFile& f) const {

	TestCharacteristicSet::SaveAll (f);
}


void TestSampleCharacteristicSet :: SaveAll (RGVOutStream& f) const {

	TestCharacteristicSet::SaveAll (f);
}


STRTestCharacteristic :: STRTestCharacteristic () : RGPersistent (),
MinimumRFU (DataSignal::GetMinimumHeight ()), MinimumFitForNormalPeak (0.95), TriggerFitForArtifactTest (0.97), TestForNegative (FALSE), MaximumRFU (-1.0) {

}


STRTestCharacteristic :: STRTestCharacteristic (double minRFU, double minFitForNormal, double triggerForArtifact, bool testForNeg, double maxRFU) :
RGPersistent (), MinimumRFU (minRFU), MinimumFitForNormalPeak (minFitForNormal), TriggerFitForArtifactTest (triggerForArtifact),
TestForNegative (testForNeg), MaximumRFU (maxRFU) {

}


STRTestCharacteristic :: ~STRTestCharacteristic () {

}


void STRTestCharacteristic :: Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) {

	Endl endLine;
	text << indent << "Parameters and thresholds for individual characteristic testing:" << endLine;
	text << indent << Delim << "Minimum RFU for valid peak:  " << Delim << Delim << Delim << Delim << Delim << MinimumRFU << endLine;
	text << indent << Delim << "Maximum RFU for valid peak:  " << Delim << Delim << Delim << Delim << Delim << MaximumRFU << endLine;
	text << indent << Delim << "Minimum acceptable fit level for valid peak:  " << Delim << Delim << Delim << Delim << Delim << GetMinimumFitForNormalPeak () << endLine;
	text << indent << Delim << "Minimum fit level to bypass artifact test:  " << Delim << Delim << Delim << Delim << Delim << GetTriggerFitForArtifactTest () << endLine;

	if (TestForNegative)
		text << indent << Delim << "Test for negative component in fitted curve:  " << Delim << Delim << Delim << Delim << Delim << "TRUE" << endLine;

	else
		text << indent << Delim << "Test for negative component in fitted curve:  " << Delim << Delim << Delim << Delim << Delim << "FALSE" << endLine;

	text << endLine;
}


double STRTestCharacteristic :: GetMinimumFitForNormalPeak () const {

	return ParametricCurve::GetMinimumFitThreshold ();
}


double STRTestCharacteristic :: GetTriggerFitForArtifactTest () const {

	return ParametricCurve::GetTriggerForArtifactTest ();
}



size_t STRTestCharacteristic :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += 4 * sizeof (double) + sizeof (Boolean);
	return size;
}


void STRTestCharacteristic :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	f.Read (MinimumRFU);
	f.Read (MinimumFitForNormalPeak);
	f.Read (TriggerFitForArtifactTest);
	f.Read ((short&)TestForNegative);
	f.Read (MaximumRFU);
}


void STRTestCharacteristic :: RestoreAll (RGVInStream& f) {

	RGPersistent::RestoreAll (f);
	f >> MinimumRFU;
	f >> MinimumFitForNormalPeak;
	f >> TriggerFitForArtifactTest;
	f >> (short&)TestForNegative;
	f >> MaximumRFU;
}


void STRTestCharacteristic :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	f.Write (MinimumRFU);
	f.Write (MinimumFitForNormalPeak);
	f.Write (TriggerFitForArtifactTest);
	f.Write ((short&)TestForNegative);
	f.Write (MaximumRFU);
}


void STRTestCharacteristic :: SaveAll (RGVOutStream& f) const {

	RGPersistent::SaveAll (f);
	f << MinimumRFU;
	f << MinimumFitForNormalPeak;
	f << TriggerFitForArtifactTest;
	f << (short&)TestForNegative;
	f << MaximumRFU;
}


STRTestCharacteristicSet :: STRTestCharacteristicSet () : TestControlCharacteristicSet (),
MinimumFractionOfAverageWidth (0.25), MaximumMultipleOfAverageWidth (2.0), MinimumFractionOfAveragePeak (0.333), 
MaximumMultipleOfAveragePeak (-1.0) {

}



STRTestCharacteristicSet :: STRTestCharacteristicSet (double minWidth, double maxWidth, double minPeak, double maxPeak) : TestControlCharacteristicSet (),
MinimumFractionOfAverageWidth (minWidth), MaximumMultipleOfAverageWidth (maxWidth), MinimumFractionOfAveragePeak (minPeak), 
MaximumMultipleOfAveragePeak (maxPeak) {

}



STRTestCharacteristicSet :: ~STRTestCharacteristicSet () {

}


void STRTestCharacteristicSet :: Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) {

	Endl endLine;
	text << indent << "Parameters and thresholds for characteristic testing within set:" << endLine;
	text << indent << Delim << "Minimum acceptable fraction of average width:  " << Delim << Delim << Delim << Delim << Delim << MinimumFractionOfAverageWidth << endLine;
	text << indent << Delim << "Maximum acceptable fraction of average width:  " << Delim << Delim << Delim << Delim << Delim << MaximumMultipleOfAverageWidth << endLine;
	text << indent << Delim << "Minimum acceptable fraction of average peak heights:  " << Delim << Delim << Delim << Delim << Delim << MinimumFractionOfAveragePeak << endLine;

	if (MaximumMultipleOfAveragePeak > 0.0)
		text << indent << Delim << "Maximum acceptable multiple of average peak heights:  " << Delim << Delim << Delim << Delim << Delim << MaximumMultipleOfAveragePeak << endLine;

	else
		text << indent << Delim << "No limit on height multiple of average peak heights" << endLine;

	text << endLine;
}


void STRTestCharacteristicSet :: PrecomputeAverages (RGDList& set) {

	RGDListIterator it (set);
	DataSignal* nextSignal;
	averageWidth = 0.0;
	averagePeak = 0.0;
	int NumberOfSignals = set.Entries ();
	double left, right;

	while ((nextSignal = (DataSignal*)it ())) {

		left = nextSignal->LeftEndPoint ();
		right = nextSignal->RightEndPoint ();
		averageWidth += nextSignal->GetStandardDeviation ();
		averagePeak += nextSignal->Peak ();
	}

	if (NumberOfSignals > 0) {

		averageWidth = averageWidth / NumberOfSignals;
		averagePeak = averagePeak / NumberOfSignals;
	}
}


size_t STRTestCharacteristicSet :: StoreSize () const {

	size_t size = TestControlCharacteristicSet::StoreSize ();
	size += 4 * sizeof (double);
	return size;
}


void STRTestCharacteristicSet :: RestoreAll (RGFile& f) {

	TestControlCharacteristicSet::RestoreAll (f);
	f.Read (MinimumFractionOfAverageWidth);
	f.Read (MaximumMultipleOfAverageWidth);
	f.Read (MinimumFractionOfAveragePeak);
	f.Read (MaximumMultipleOfAveragePeak);
}


void STRTestCharacteristicSet :: RestoreAll (RGVInStream& f) {

	TestControlCharacteristicSet::RestoreAll (f);
	f >> MinimumFractionOfAverageWidth;
	f >> MaximumMultipleOfAverageWidth;
	f >> MinimumFractionOfAveragePeak;
	f >> MaximumMultipleOfAveragePeak;
}


void STRTestCharacteristicSet :: SaveAll (RGFile& f) const {

	TestControlCharacteristicSet::SaveAll (f);
	f.Write (MinimumFractionOfAverageWidth);
	f.Write (MaximumMultipleOfAverageWidth);
	f.Write (MinimumFractionOfAveragePeak);
	f.Write (MaximumMultipleOfAveragePeak);
}


void STRTestCharacteristicSet :: SaveAll (RGVOutStream& f) const {

	TestControlCharacteristicSet::SaveAll (f);
	f << MinimumFractionOfAverageWidth;
	f << MaximumMultipleOfAverageWidth;
	f << MinimumFractionOfAveragePeak;
	f << MaximumMultipleOfAveragePeak;
}




STRTestSampleCharacteristic :: STRTestSampleCharacteristic () : TestSampleCharacteristic (), ExternalTest (FALSE) {

	testBody = new STRTestCharacteristic ();
}


STRTestSampleCharacteristic :: STRTestSampleCharacteristic (double minRFU, double minFitForNormal, double triggerForArtifact, bool testForNeg, 
double maxRFU) : TestSampleCharacteristic (), ExternalTest (FALSE) {

	testBody = new STRTestCharacteristic (minRFU, minFitForNormal, triggerForArtifact, testForNeg, maxRFU);
}


STRTestSampleCharacteristic :: STRTestSampleCharacteristic (STRTestCharacteristic* test) : TestSampleCharacteristic (), 
testBody (test), ExternalTest (TRUE) {

}


STRTestSampleCharacteristic :: ~STRTestSampleCharacteristic () {

	if (!ExternalTest)
		delete testBody;
}


int STRTestSampleCharacteristic :: Test (DataSignal* testSignal, double minimumHeight, double maximumHeight) {

	Boolean Marginal = FALSE;
	Boolean NotAcceptable = FALSE;
	//double fit = testSignal->GetCurveFit ();
	double peak = testSignal->Peak ();
	double rawPeak = testSignal->GetDataMode ();
	Notice* newNotice;
	RGString info;
	bool minViolated = false;
	bool maxViolated = false;
	testSignal->SetOsirisPeakBelowMinimum (false);
	testSignal->SetRawDataBelowMinimum (false);
	testSignal->SetOsirisPeakAboveMaximum (false);
	testSignal->SetRawDataAboveMaximum (false);

	if (peak < minimumHeight) {

		testSignal->SetOsirisPeakBelowMinimum (true);
		minViolated = true;
	}

	if (rawPeak < minimumHeight) {

		testSignal->SetRawDataBelowMinimum (true);
		minViolated = true;
	}

	if ((maximumHeight > 0) && (peak > maximumHeight)) {

		testSignal->SetOsirisPeakAboveMaximum (true);
		maxViolated = true;
	}

	if ((maximumHeight > 0) && (rawPeak > maximumHeight)) {

		testSignal->SetRawDataAboveMaximum (true);
		maxViolated = true;
	}

	if (minViolated) {

		if (!testSignal->isMarginallyAboveMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Peak below minimum threshold");
			testSignal->SetDontLook (true);
			NotAcceptable = TRUE;
			Marginal = FALSE;
			return -20;
		}

		else if (testSignal->isOsirisPeakBelowMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below minimum threshold but raw data above");
			newNotice = new OsirisBelowMinRFUButRawDataOK;
			testSignal->AddNoticeToList (newNotice);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Raw data below minimum threshold but Osiris above");
			newNotice = new RawDataBelowMinRFUButOsirisOK;
			testSignal->AddNoticeToList (newNotice);
		}
	}

	//if (fit < testBody->GetTriggerFitForArtifactTest ()) {

	//	Marginal = TRUE;
	//	testSignal->AddNoticeToList (OutputLevelManager::CurveFitWarrantsTestForArtifactSignature, "", 
	//		"Curve fit warrants test for artifact signature");
	//}

	if (maxViolated) {

		if (!testSignal->isMarginallyBelowMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Peak above maximum threshold");
			newNotice = new SignalPeakAboveMaxRFU;
			testSignal->AddNoticeToList (newNotice);
			NotAcceptable = TRUE;
			Marginal = FALSE;
		}

		else if (testSignal->isOsirisPeakAboveMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Osiris peak above maximum threshold but raw data below");
			newNotice = new OsirisAboveMaxRFUButRawDataOK;
			testSignal->AddNoticeToList (newNotice);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Raw data above maximum threshold but Osiris below");
			newNotice = new RawDataAboveMaxRFUButOsirisOK;
			testSignal->AddNoticeToList (newNotice);
		}

	}

	if (testSignal->CanBeNegative ()) {

		char buffer [30];
		RGString s = "Curve fit incorporates negative component (1, 2)\t\t\t\t\t";
		s += OsirisGcvt (testSignal->GetScale (1), 10, buffer);
		s += "\t";
		s += OsirisGcvt (testSignal->GetScale (2), 10, buffer);
		testSignal->AddNoticeToList (OutputLevelManager::CurveFitHasNegativeComponent, "", s);

		if (testBody->GetTestForNegative ()) {

			NotAcceptable = TRUE;
			Marginal = TRUE;
		}
	}

	//if (fit < testBody->GetMinimumFitForNormalPeak()) {

	//	NotAcceptable = TRUE;
	//	Marginal = TRUE;
	//	testSignal->AddNoticeToList (OutputLevelManager::CurveFitUnacceptable, "", "Curve fit below minimum acceptable level");
	//	newNotice = new CurveFitTooLow;
	//	info << "Fit = " << fit << " (" << testBody->GetMinimumFitForNormalPeak () << " - 1.0)";
	//	newNotice->AddDataItem (info);
	//	testSignal->AddNoticeToList (newNotice);
	//	return -20;
	//}

	if (!Marginal) {

		if (NotAcceptable)
			return -1;

		return 1;
	}

	if (NotAcceptable)
		return -10;

	return 0;
}


int STRTestSampleCharacteristic :: TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight) {

	//
	//  This is ladder and sample stage 1
	//

	Boolean Marginal = FALSE;
	Boolean NotAcceptable = FALSE;
	//double fit = testSignal->GetCurveFit ();
	double peak = testSignal->Peak ();
	double rawPeak = testSignal->GetDataMode ();
	RGString info;
	bool minViolated = false;
	bool maxViolated = false;
	testSignal->SetOsirisPeakBelowMinimum (false);
	testSignal->SetRawDataBelowMinimum (false);
	testSignal->SetOsirisPeakAboveMaximum (false);
	testSignal->SetRawDataAboveMaximum (false);

	smOsirisBelowMinRFUButRawDataAbove osirisBelowRawAboveMin;
	smRawDataBelowMinRFUButOsirisAbove rawBelowOsirisAboveMin;
	smOsirisAboveMaxRFUButRawDataBelow osirisAboveRawBelowMax;
	smRawDataAboveMaxRFUButOsirisBelow rawAboveOsirisBelowMax;
	smPeakHeightAboveMax signalAboveMaxRFU;

	if (peak < minimumHeight) {

		testSignal->SetOsirisPeakBelowMinimum (true);
		minViolated = true;
	}

	if (rawPeak < minimumHeight) {

		testSignal->SetRawDataBelowMinimum (true);
		minViolated = true;
	}

	if ((maximumHeight > 0) && (peak > maximumHeight)) {

		testSignal->SetOsirisPeakAboveMaximum (true);
		maxViolated = true;
	}

	if ((maximumHeight > 0) && (rawPeak > maximumHeight)) {

		testSignal->SetRawDataAboveMaximum (true);
		maxViolated = true;
	}

	if (minViolated) {

		if (!testSignal->isMarginallyAboveMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Peak below minimum threshold");
			testSignal->SetDontLook (true);
			NotAcceptable = TRUE;
			Marginal = FALSE;
			return -20;
		}

		else if (testSignal->isOsirisPeakBelowMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below minimum threshold but raw data above");
			testSignal->SetMessageValue (osirisBelowRawAboveMin, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Raw data below minimum threshold but Osiris above");
			testSignal->SetMessageValue (rawBelowOsirisAboveMin, true);
		}
	}

	if (maxViolated) {

		if (!testSignal->isMarginallyBelowMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Peak above maximum threshold");
			testSignal->SetMessageValue (signalAboveMaxRFU, true);
//			NotAcceptable = TRUE;
//			Marginal = FALSE;
		}

		else if (testSignal->isOsirisPeakAboveMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Osiris peak above maximum threshold but raw data below");
			testSignal->SetMessageValue (osirisAboveRawBelowMax, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Raw data above maximum threshold but Osiris below");
			testSignal->SetMessageValue (rawAboveOsirisBelowMax, true);
		}

	}

	if (testSignal->CanBeNegative ()) {

		char buffer [30];
		RGString s = "Curve fit incorporates negative component (1, 2)\t\t\t\t\t";
		s += OsirisGcvt (testSignal->GetScale (1), 10, buffer);
		s += "\t";
		s += OsirisGcvt (testSignal->GetScale (2), 10, buffer);
		testSignal->AddNoticeToList (OutputLevelManager::CurveFitHasNegativeComponent, "", s);

		if (testBody->GetTestForNegative ()) {

			NotAcceptable = TRUE;
			Marginal = TRUE;
		}
	}

	if (!Marginal) {

		if (NotAcceptable)
			return -1;

		return 1;
	}

	if (NotAcceptable)
		return -10;

	return 0;
}


int STRTestSampleCharacteristic :: TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight) {

	//
	//  This is ladder and sample stage 1
	//

	//double fit = testSignal->GetCurveFit ();
	double peak = testSignal->Peak ();
	double rawPeak = testSignal->GetDataMode ();
	RGString info;
	bool minViolated = false;
	bool maxViolated = false;
	bool remove = false;
	testSignal->SetOsirisPeakBelowMinimum (false);
	testSignal->SetRawDataBelowMinimum (false);
	testSignal->SetOsirisPeakAboveMaximum (false);
	testSignal->SetRawDataAboveMaximum (false);

	smOsirisBelowMinRFUButRawDataAbove osirisBelowRawAboveMin;
	smRawDataBelowMinRFUButOsirisAbove rawBelowOsirisAboveMin;
	smOsirisAboveMaxRFUButRawDataBelow osirisAboveRawBelowMax;
	smRawDataAboveMaxRFUButOsirisBelow rawAboveOsirisBelowMax;
	smPeakHeightAboveMax signalAboveMaxRFU;
	smBelowMinRFU belowAnalysisThreshold;

	if (peak < analysisThreshold) {

		testSignal->SetOsirisPeakBelowMinimum (true);
		minViolated = true;
	}

	if (rawPeak < analysisThreshold) {

		testSignal->SetRawDataBelowMinimum (true);
		minViolated = true;
	}

	if (peak < detectionThreshold)
		remove = true;

	if ((maximumHeight > 0) && (peak > maximumHeight)) {

		testSignal->SetOsirisPeakAboveMaximum (true);
		maxViolated = true;
	}

	if ((maximumHeight > 0) && (rawPeak > maximumHeight)) {

		testSignal->SetRawDataAboveMaximum (true);
		maxViolated = true;
	}

	if (remove) {

		testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Peak below minimum detection threshold");
		testSignal->SetDontLook (true);
		return -20;
	}

	if (minViolated) {

		if (!testSignal->isMarginallyAboveMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below analysis threshold but above detection threshold");
			testSignal->SetMessageValue (belowAnalysisThreshold, true);
		}

		else if (testSignal->isOsirisPeakBelowMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below minimum threshold but raw data above");
			testSignal->SetMessageValue (osirisBelowRawAboveMin, true);
			testSignal->SetMessageValue (belowAnalysisThreshold, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Raw data below minimum threshold but Osiris above");
			testSignal->SetMessageValue (rawBelowOsirisAboveMin, true);
		}
	}

	if (maxViolated) {

		if (!testSignal->isMarginallyBelowMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Peak above maximum threshold");
			testSignal->SetMessageValue (signalAboveMaxRFU, true);
		}

		else if (testSignal->isOsirisPeakAboveMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Osiris peak above maximum threshold but raw data below");
			testSignal->SetMessageValue (osirisAboveRawBelowMax, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Raw data above maximum threshold but Osiris below");
			testSignal->SetMessageValue (rawAboveOsirisBelowMax, true);
		}

	}

	if (testSignal->CanBeNegative ()) {

		char buffer [30];
		RGString s = "Curve fit incorporates negative component (1, 2)\t\t\t\t\t";
		s += OsirisGcvt (testSignal->GetScale (1), 10, buffer);
		s += "\t";
		s += OsirisGcvt (testSignal->GetScale (2), 10, buffer);
		testSignal->AddNoticeToList (OutputLevelManager::CurveFitHasNegativeComponent, "", s);

		if (testBody->GetTestForNegative ())
			return -10;
	}

	return 0;
}



void STRTestSampleCharacteristic :: Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) {

	testBody->Report (text, indent, Delim);
}


size_t STRTestSampleCharacteristic :: StoreSize () const {

	return TestSampleCharacteristic::StoreSize () + testBody->StoreSize ();
}


void STRTestSampleCharacteristic :: RestoreAll (RGFile& f) {

	TestSampleCharacteristic::RestoreAll (f);
	testBody->RestoreAll (f);

}


void STRTestSampleCharacteristic :: RestoreAll (RGVInStream& f) {

	TestSampleCharacteristic::RestoreAll (f);
	testBody->RestoreAll (f);
}


void STRTestSampleCharacteristic :: SaveAll (RGFile& f) const {

	TestSampleCharacteristic::SaveAll (f);
	testBody->SaveAll (f);
}


void STRTestSampleCharacteristic :: SaveAll (RGVOutStream& f) const {

	TestSampleCharacteristic::SaveAll (f);
	testBody->SaveAll (f);
}




STRTestControlCharacteristic :: STRTestControlCharacteristic () : TestControlCharacteristic (), ExternalTest (FALSE) {

	testBody = new STRTestCharacteristic ();
}


STRTestControlCharacteristic :: STRTestControlCharacteristic (double minRFU, double minFitForNormal, double triggerForArtifact, bool testForNeg, 
double maxRFU) : TestControlCharacteristic (), ExternalTest (FALSE) {

	testBody = new STRTestCharacteristic (minRFU, minFitForNormal, triggerForArtifact, testForNeg, maxRFU);
}


STRTestControlCharacteristic :: STRTestControlCharacteristic (STRTestCharacteristic* test) : TestControlCharacteristic (),
testBody (test), ExternalTest (TRUE) {

}


STRTestControlCharacteristic :: ~STRTestControlCharacteristic () {

	if (!ExternalTest)
		delete testBody;
}
//smOsirisBelowMinRFUButRawDataAbove osirisBelowRawAboveMin;
//	smRawDataBelowMinRFUButOsirisAbove rawBelowOsirisAboveMin;
//	smOsirisAboveMaxRFUButRawDataBelow osirisAboveRawBelowMax;
//	smRawDataAboveMaxRFUButOsirisBelow rawAboveOsirisBelowMax;
//	smPeakHeightAboveMax signalAboveMaxRFU;

int STRTestControlCharacteristic :: Test (DataSignal* testSignal, double minimumHeight, double maximumHeight) {

	Boolean Marginal = FALSE;
	Boolean NotAcceptable = FALSE;
	double fit = testSignal->GetCurveFit ();
	double peak = testSignal->Peak ();
	double rawPeak = testSignal->GetDataMode ();
	RGString info;
	Notice* newNotice;
	bool minViolated = false;
	bool maxViolated = false;
	testSignal->SetOsirisPeakBelowMinimum (false);
	testSignal->SetRawDataBelowMinimum (false);
	testSignal->SetOsirisPeakAboveMaximum (false);
	testSignal->SetRawDataAboveMaximum (false);

	if (peak < minimumHeight) {

		testSignal->SetOsirisPeakBelowMinimum (true);
		minViolated = true;
	}

	if (rawPeak < minimumHeight) {

		testSignal->SetRawDataBelowMinimum (true);
		minViolated = true;
	}

	if ((maximumHeight > 0) && (peak > maximumHeight)) {

		testSignal->SetOsirisPeakAboveMaximum (true);
		maxViolated = true;
	}

	if ((maximumHeight > 0) && (rawPeak > maximumHeight)) {

		testSignal->SetRawDataAboveMaximum (true);
		maxViolated = true;
	}

	if (minViolated) {

		if (!testSignal->isMarginallyAboveMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Peak below minimum threshold");
			testSignal->SetDontLook (true);
			NotAcceptable = TRUE;
			Marginal = FALSE;
			return -20;
		}

		else if (testSignal->isOsirisPeakBelowMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below minimum threshold but raw data above");
			newNotice = new OsirisBelowMinRFUButRawDataOK;
			testSignal->AddNoticeToList (newNotice);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Raw data below minimum threshold but Osiris above");
			newNotice = new RawDataBelowMinRFUButOsirisOK;
			testSignal->AddNoticeToList (newNotice);
		}
	}

	//if (fit < testBody->GetTriggerFitForArtifactTest ()) {

	//	Marginal = TRUE;
	//	testSignal->AddNoticeToList (OutputLevelManager::CurveFitWarrantsTestForArtifactSignature, "", 
	//		"Curve fit warrants test for artifact signature");
	//}

	if (maxViolated) {

		if (!testSignal->isMarginallyBelowMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Peak above maximum threshold");
			newNotice = new SignalPeakAboveMaxRFU;
			testSignal->AddNoticeToList (newNotice);
			NotAcceptable = TRUE;
			Marginal = FALSE;
		}

		else if (testSignal->isOsirisPeakAboveMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Osiris peak above maximum threshold but raw data below");
			newNotice = new OsirisAboveMaxRFUButRawDataOK;
			testSignal->AddNoticeToList (newNotice);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Raw data above maximum threshold but Osiris below");
			newNotice = new RawDataAboveMaxRFUButOsirisOK;
			testSignal->AddNoticeToList (newNotice);
		}

	}

	if (testSignal->CanBeNegative ()) {

		char buffer [30];
		RGString s = "Curve fit incorporates negative component (1, 2)\t\t\t\t\t";
		s += OsirisGcvt (testSignal->GetScale (1), 10, buffer);
		s += "\t";
		s += OsirisGcvt (testSignal->GetScale (2), 10, buffer);
		testSignal->AddNoticeToList (OutputLevelManager::CurveFitHasNegativeComponent, "", s);

		if (testBody->GetTestForNegative ()) {

			NotAcceptable = TRUE;
			Marginal = TRUE;
		}
	}

	//if (fit < testBody->GetMinimumFitForNormalPeak()) {

	//	NotAcceptable = TRUE;
	//	Marginal = TRUE;
	//	testSignal->AddNoticeToList (OutputLevelManager::CurveFitUnacceptable, "", "Curve fit below minimum acceptable level");
	//	newNotice = new CurveFitTooLow;
	//	info << "Fit = " << fit << " (" << testBody->GetMinimumFitForNormalPeak () << " - 1.0)";
	//	newNotice->AddDataItem (info);
	//	testSignal->AddNoticeToList (newNotice);
	//	return -20;
	//}

	if (!Marginal) {

		if (NotAcceptable)
			return -1;

		return 1;
	}

	if (NotAcceptable)
		return -10;

	return 0;
}


int STRTestControlCharacteristic :: TestSM (DataSignal* testSignal, double minimumHeight, double maximumHeight) {

	Boolean Marginal = FALSE;
	Boolean NotAcceptable = FALSE;
	double fit = testSignal->GetCurveFit ();
	double peak = testSignal->Peak ();
	double rawPeak = testSignal->GetDataMode ();
	RGString info;
	bool minViolated = false;
	bool maxViolated = false;
	testSignal->SetOsirisPeakBelowMinimum (false);
	testSignal->SetRawDataBelowMinimum (false);
	testSignal->SetOsirisPeakAboveMaximum (false);
	testSignal->SetRawDataAboveMaximum (false);

	smOsirisBelowMinRFUButRawDataAbove osirisBelowRawAboveMin;
	smRawDataBelowMinRFUButOsirisAbove rawBelowOsirisAboveMin;
	smOsirisAboveMaxRFUButRawDataBelow osirisAboveRawBelowMax;
	smRawDataAboveMaxRFUButOsirisBelow rawAboveOsirisBelowMax;
	smPeakHeightAboveMax signalAboveMaxRFU;

	if (peak < minimumHeight) {

		testSignal->SetOsirisPeakBelowMinimum (true);
		minViolated = true;
	}

	if (rawPeak < minimumHeight) {

		testSignal->SetRawDataBelowMinimum (true);
		minViolated = true;
	}

	if ((maximumHeight > 0) && (peak > maximumHeight)) {

		testSignal->SetOsirisPeakAboveMaximum (true);
		maxViolated = true;
	}

	if ((maximumHeight > 0) && (rawPeak > maximumHeight)) {

		testSignal->SetRawDataAboveMaximum (true);
		maxViolated = true;
	}

	if (minViolated) {

		if (!testSignal->isMarginallyAboveMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Peak below minimum threshold");
			testSignal->SetDontLook (true);
			NotAcceptable = TRUE;
			Marginal = FALSE;
			return -20;
		}

		else if (testSignal->isOsirisPeakBelowMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below minimum threshold but raw data above");
			testSignal->SetMessageValue (osirisBelowRawAboveMin, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Raw data below minimum threshold but Osiris above");
			testSignal->SetMessageValue (rawBelowOsirisAboveMin, true);
		}
	}

	if (maxViolated) {

		if (!testSignal->isMarginallyBelowMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Peak above maximum threshold");
			testSignal->SetMessageValue (signalAboveMaxRFU, true);
			NotAcceptable = TRUE;
			Marginal = FALSE;
		}

		else if (testSignal->isOsirisPeakAboveMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Osiris peak above maximum threshold but raw data below");
			testSignal->SetMessageValue (osirisAboveRawBelowMax, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Raw data above maximum threshold but Osiris below");
			testSignal->SetMessageValue (rawAboveOsirisBelowMax, true);
		}

	}

	if (testSignal->CanBeNegative ()) {

		char buffer [30];
		RGString s = "Curve fit incorporates negative component (1, 2)\t\t\t\t\t";
		s += OsirisGcvt (testSignal->GetScale (1), 10, buffer);
		s += "\t";
		s += OsirisGcvt (testSignal->GetScale (2), 10, buffer);
		testSignal->AddNoticeToList (OutputLevelManager::CurveFitHasNegativeComponent, "", s);

		if (testBody->GetTestForNegative ()) {

			NotAcceptable = TRUE;
			Marginal = TRUE;
		}
	}

	if (!Marginal) {

		if (NotAcceptable)
			return -1;

		return 1;
	}

	if (NotAcceptable)
		return -10;

	return 0;
}


int STRTestControlCharacteristic :: TestSM (DataSignal* testSignal, double detectionThreshold, double analysisThreshold, double maximumHeight) {

	double fit = testSignal->GetCurveFit ();
	double peak = testSignal->Peak ();
	double rawPeak = testSignal->GetDataMode ();
	RGString info;
	bool minViolated = false;
	bool maxViolated = false;
	bool remove = false;
	testSignal->SetOsirisPeakBelowMinimum (false);
	testSignal->SetRawDataBelowMinimum (false);
	testSignal->SetOsirisPeakAboveMaximum (false);
	testSignal->SetRawDataAboveMaximum (false);

	smOsirisBelowMinRFUButRawDataAbove osirisBelowRawAboveMin;
	smRawDataBelowMinRFUButOsirisAbove rawBelowOsirisAboveMin;
	smOsirisAboveMaxRFUButRawDataBelow osirisAboveRawBelowMax;
	smRawDataAboveMaxRFUButOsirisBelow rawAboveOsirisBelowMax;
	smPeakHeightAboveMax signalAboveMaxRFU;
	smBelowMinRFU belowAnalysisThreshold;

	if (peak < analysisThreshold) {

		testSignal->SetOsirisPeakBelowMinimum (true);
		minViolated = true;
	}

	if (rawPeak < analysisThreshold) {

		testSignal->SetRawDataBelowMinimum (true);
		minViolated = true;
	}

	if (peak < detectionThreshold)
		remove = true;

	if ((maximumHeight > 0) && (peak > maximumHeight)) {

		testSignal->SetOsirisPeakAboveMaximum (true);
		maxViolated = true;
	}

	if ((maximumHeight > 0) && (rawPeak > maximumHeight)) {

		testSignal->SetRawDataAboveMaximum (true);
		maxViolated = true;
	}

	if (remove) {

		testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Peak below minimum detection threshold");
		testSignal->SetDontLook (true);
		return -20;
	}

	if (minViolated) {

		if (!testSignal->isMarginallyAboveMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below analysis threshold but above detection threshold");
			testSignal->SetMessageValue (belowAnalysisThreshold, true);
		}

		else if (testSignal->isOsirisPeakBelowMinimum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Osiris peak below minimum threshold but raw data above");
			testSignal->SetMessageValue (osirisBelowRawAboveMin, true);
			testSignal->SetMessageValue (belowAnalysisThreshold, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightBelowMinimumStandard, "", "Raw data below minimum threshold but Osiris above");
			testSignal->SetMessageValue (rawBelowOsirisAboveMin, true);
		}
	}

	if (maxViolated) {

		if (!testSignal->isMarginallyBelowMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Peak above maximum threshold");
			testSignal->SetMessageValue (signalAboveMaxRFU, true);
		}

		else if (testSignal->isOsirisPeakAboveMaximum ()) {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Osiris peak above maximum threshold but raw data below");
			testSignal->SetMessageValue (osirisAboveRawBelowMax, true);
		}

		else {

			testSignal->AddNoticeToList (OutputLevelManager::PeakHeightAboveMaximumStandard, "", "Raw data above maximum threshold but Osiris below");
			testSignal->SetMessageValue (rawAboveOsirisBelowMax, true);
		}

	}

	if (testSignal->CanBeNegative ()) {

		char buffer [30];
		RGString s = "Curve fit incorporates negative component (1, 2)\t\t\t\t\t";
		s += OsirisGcvt (testSignal->GetScale (1), 10, buffer);
		s += "\t";
		s += OsirisGcvt (testSignal->GetScale (2), 10, buffer);
		testSignal->AddNoticeToList (OutputLevelManager::CurveFitHasNegativeComponent, "", s);

		if (testBody->GetTestForNegative ())
			return -10;
	}

	return 0;
}


void STRTestControlCharacteristic :: Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) {

	testBody->Report (text, indent, Delim);
}


size_t STRTestControlCharacteristic :: StoreSize () const {

	return testBody->StoreSize () + TestControlCharacteristic::StoreSize ();
}


void STRTestControlCharacteristic :: RestoreAll (RGFile& f) {

	TestControlCharacteristic::RestoreAll (f);
	testBody->RestoreAll (f);
}


void STRTestControlCharacteristic :: RestoreAll (RGVInStream& f) {

	TestControlCharacteristic::RestoreAll (f);
	testBody->RestoreAll (f);
}


void STRTestControlCharacteristic :: SaveAll (RGFile& f) const {

	TestControlCharacteristic::SaveAll (f);
	testBody->SaveAll (f);
}


void STRTestControlCharacteristic :: SaveAll (RGVOutStream& f) const {

	TestControlCharacteristic::SaveAll (f);
	testBody->SaveAll (f);
}



STRTestSampleCharacteristicSet :: STRTestSampleCharacteristicSet () : TestSampleCharacteristicSet () {

}

/*
STRTestSampleCharacteristicSet :: STRTestSampleCharacteristicSet (double minWidth, double maxWidth, double minPeak, double maxPeak) :
TestSampleCharacteristicSet () {

}*/


STRTestSampleCharacteristicSet :: ~STRTestSampleCharacteristicSet () {

}


int STRTestSampleCharacteristicSet :: Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

	return -1;
}


int STRTestSampleCharacteristicSet :: Test (RGDList& set, double& correlation) {

	return -1;
}


void STRTestSampleCharacteristicSet :: Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) {

//	STRTestCharacteristicSet::Report (text, indent, Delim);
}


size_t STRTestSampleCharacteristicSet :: StoreSize () const {

	return TestSampleCharacteristicSet::StoreSize ();
}


void STRTestSampleCharacteristicSet :: RestoreAll (RGFile& f) {

	TestSampleCharacteristicSet::RestoreAll (f);
}


void STRTestSampleCharacteristicSet :: RestoreAll (RGVInStream& f) {

	TestSampleCharacteristicSet::RestoreAll (f);
}


void STRTestSampleCharacteristicSet :: SaveAll (RGFile& f) const {

	TestSampleCharacteristicSet::SaveAll (f);
}


void STRTestSampleCharacteristicSet :: SaveAll (RGVOutStream& f) const {

	TestSampleCharacteristicSet::SaveAll (f);
}




STRTestControlCharacteristicSet :: STRTestControlCharacteristicSet () : STRTestCharacteristicSet () {

	IdealBPArray = new double [3];
	NElements = 3;
	NElements1 = 2;
	IdealBPArray [0] = 0.0;
	IdealBPArray [1] = 1.0;
	IdealBPArray [2] = 2.0;
	DifferenceArray = new double [2];
	DifferenceArray [0] = DifferenceArray [1] = 1.0;
	DifferenceNorm = sqrt (2.0);
}


STRTestControlCharacteristicSet :: STRTestControlCharacteristicSet (double minWidth, double maxWidth, double minPeak, double maxPeak, 
double* idealArray, int N) :
STRTestCharacteristicSet (minWidth, maxWidth, minPeak, maxPeak), NElements (N), NElements1 (N - 1), previous (NULL) {

	IdealBPArray = new double [NElements];
  int i;
	for (i=0; i<NElements; i++)
		IdealBPArray [i] = idealArray [i];

	DifferenceArray = new double [NElements1];
	double temp;
	double norm = 0.0;

	for (i=0; i<NElements1; i++) {

		DifferenceArray [i] = temp = IdealBPArray [i + 1] - IdealBPArray [i];
		norm += temp * temp;
	}

	DifferenceNorm = sqrt (norm);
}


STRTestControlCharacteristicSet :: STRTestControlCharacteristicSet (double* idealArray, int N) :
STRTestCharacteristicSet (), NElements (N), NElements1 (N - 1), previous (NULL) {

	IdealBPArray = new double [NElements];
  int i;
	for (i=0; i<NElements; i++)
		IdealBPArray [i] = idealArray [i];

	DifferenceArray = new double [NElements1];
	double temp;
	double norm = 0.0;

	for (i=0; i<NElements1; i++) {

		DifferenceArray [i] = temp = IdealBPArray [i + 1] - IdealBPArray [i];
		norm += temp * temp;
	}

	DifferenceNorm = sqrt (norm);
}


STRTestControlCharacteristicSet :: ~STRTestControlCharacteristicSet () {

	delete[] IdealBPArray;
	delete[] DifferenceArray;
}



int STRTestControlCharacteristicSet :: Test (DataSignal* previous, DataSignal* testSignal, DataSignal* following) {

//	double Width = testSignal->GetStandardDeviation ();
//	double peak = testSignal->Peak ();
	Boolean NotAcceptable = FALSE;
	Boolean Marginal = FALSE;
//	Notice* newNotice;
	RGString info;

/*	if (Width < MinimumFractionOfAverageWidth * averageWidth) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooNarrow, "", "Signal is too narrow for set");
	}*/
/*

	if (Width > MaximumMultipleOfAverageWidth * averageWidth) {

		NotAcceptable = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooWide, "", "Signal is too wide for set");
		newNotice = new PoorPeakMorphology;
		info << "(Width)";
		newNotice->AddDataItem (info);
		testSignal->AddNoticeToList (newNotice);
	}

	if (peak < MinimumFractionOfAveragePeak * averagePeak) {

		Marginal = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooLow, "", "Signal is too low for set");
	}

	if ((MaximumMultipleOfAveragePeak > 0.0) && (peak > MaximumMultipleOfAveragePeak * averagePeak)) {

		Marginal = TRUE;
		testSignal->AddNoticeToList (OutputLevelManager::SignalTooHigh, "", "Signal is too high for set");
	}
*/

	double ThisMean = testSignal->GetMean ();
	double ThisPeak = testSignal->Peak ();
	double ThisSigma = testSignal->GetStandardDeviation ();

	//
	// test for non-template addition (-A and +A) and for stutter (n - 4 and n + 4)
	//

	if (following != NULL) {

		double NextMean = following->GetMean ();
		double NextPeak = following->Peak ();

		if (NextMean - ThisMean < 8.0 * ThisSigma) {

			if (ThisPeak < 0.3333 * NextPeak) {

				NotAcceptable = TRUE;
				testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForFollowing, "", 
					"Signal represents non-template addition (-A) for following peak");
			}
		}

		else if ((NextMean - ThisMean < 24.0 * ThisSigma) && (ThisPeak < 0.2 * NextPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForFollowing, "", 
				"Signal represents stutter for following peak");
		}
	}

	if (previous != NULL) {

		double PreviousMean = previous->GetMean ();
		double PreviousPeak = previous->Peak ();

		if ((ThisMean - PreviousMean < 8.0 * ThisSigma) && (ThisPeak < 0.333 * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::NonTemplateAdditionForPreceding, "", 
				"Signal represents non-template addition (+A) for previous peak");
		}

		else if ((ThisMean - PreviousMean < 24.0 * ThisSigma) && (ThisPeak < 0.2 * PreviousPeak)) {

			NotAcceptable = TRUE;
			testSignal->AddNoticeToList (OutputLevelManager::StutterForPreceding, "", 
				"Signal represents stutter for previous peak");
		}
	}

	if (previous != NULL)
		testSignal->SetPreviousPeak (previous->Peak ());
	
	else
		testSignal->SetPreviousPeak (0.0);

	if (following != NULL)
		testSignal->SetNextPeak (following->Peak ());
	
	else
		testSignal->SetNextPeak (0.0);

	if (NotAcceptable)
		return -1;

	if (Marginal)
		return 0;

	return 1;
}


int STRTestControlCharacteristicSet :: Test (RGDList& set, double& correlation) {

	int n = set.Entries ();

	if (n < NElements) {

		correlation = 0.0;
		return -1;
	}

	int n1 = n - 1;
	double* means = new double [n];
	DataSignal* nextSignal;
	RGDListIterator it (set);
	int i = 0;
	double* differences = new double [n1];

	while ((nextSignal = (DataSignal*) it ())) {

		means [i] = nextSignal->GetMean ();
		i++;
	}

	for (i=0; i<n1; i++)
		differences [i] = means [i + 1] - means [i];

	int Displacement = 0;
	double MaxCorrelation = 0.0;
	double Correlation;
	int NSets = n - NElements + 1;

	for (int j=0; j<NSets; j++) {

		Correlation = VectorDotProductWithDifferences (differences + j);

		if (Correlation > MaxCorrelation) {

			MaxCorrelation = Correlation;
			Displacement = j;
		}
	}

	delete[] means;
	delete[] differences;

	correlation = MaxCorrelation;
	return Displacement;
}


void STRTestControlCharacteristicSet :: Report (RGTextOutput& text, const RGString& indent, const RGString& Delim) {

	Endl endLine;
	STRTestCharacteristicSet::Report (text, indent, Delim);
	text << indent << Delim << "Number of array elements:  " << Delim << Delim << Delim << Delim << NElements << endLine;
	text << indent << Delim << "Array:  " << Delim << Delim;

	for (int i=0; i<NElements; i++)
		text << IdealBPArray [i] << Delim;

	text << endLine;
}


const double* STRTestControlCharacteristicSet :: GetIdealSet () const {

	return IdealBPArray;
}


int STRTestControlCharacteristicSet :: GetSetSize () const {

	return NElements;
}


size_t STRTestControlCharacteristicSet :: StoreSize () const {

	size_t size = STRTestCharacteristicSet::StoreSize ();
	size += sizeof (int);
	size += NElements * sizeof (double);
	return size;
}

void STRTestControlCharacteristicSet :: RestoreAll (RGFile& f) {

	STRTestCharacteristicSet::RestoreAll (f);
	delete[] IdealBPArray;
	delete[] DifferenceArray;
	f.Read (NElements);
	NElements1 = NElements - 1;
	IdealBPArray = new double [NElements];
	DifferenceArray = new double [NElements1];
	f.Read (IdealBPArray, NElements);
	double temp;
	double norm = 0.0;

	for (int i=0; i<NElements1; i++) {

		DifferenceArray [i] = temp = IdealBPArray [i + 1] - IdealBPArray [i];
		norm += temp * temp;
	}

	DifferenceNorm = sqrt (norm);
}


void STRTestControlCharacteristicSet :: RestoreAll (RGVInStream& f) {

	STRTestCharacteristicSet::RestoreAll (f);
	delete[] IdealBPArray;
	delete[] DifferenceArray;
	f >> NElements;
	NElements1 = NElements - 1;
	IdealBPArray = new double [NElements];
	DifferenceArray = new double [NElements1];
	f.Get (IdealBPArray, NElements);
	double temp;
	double norm = 0.0;

	for (int i=0; i<NElements1; i++) {

		DifferenceArray [i] = temp = IdealBPArray [i + 1] - IdealBPArray [i];
		norm += temp * temp;
	}

	DifferenceNorm = sqrt (norm);
}


void STRTestControlCharacteristicSet :: SaveAll (RGFile& f) const {

	STRTestCharacteristicSet::SaveAll (f);
	f.Write (NElements);
	f.Write (IdealBPArray, NElements);
}


void STRTestControlCharacteristicSet :: SaveAll (RGVOutStream& f) const {

	STRTestCharacteristicSet::SaveAll (f);
	f << NElements;
	f.put (IdealBPArray, NElements);
}


double STRTestControlCharacteristicSet :: VectorDotProductWithDifferences (double* a) {

	double Sum = 0.0;
	double norm = 0.0;
	double temp;

	for (int i=0; i<NElements1; i++) {

		temp = a[i];
		Sum += temp * DifferenceArray [i];
		norm += temp * temp;
	}

	norm = sqrt (norm);
	Sum = Sum / (norm * DifferenceNorm);
	return Sum;
}


