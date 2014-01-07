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
*  FileName: SingularValueDecomposition.cpp
*  Author:   Robert Goor
*
*/
//
//  class SingularValueDecomposition computes the SVD of a large rectangular matrix and returns the parts.
// It also uses the computed SVD to solve the multiple linear least squares regression problem.
//

#include "SingularValueDecomposition.h"
#include <math.h>


double SingularValueDecomposition::sWCoefficientThreshold = 1.0e-12;


SingularValueDecomposition :: SingularValueDecomposition (int rows, int columns) : 
mSVDComputed (false), mRows (rows), mColumns (columns), mWInverseComputed (false), mData (NULL) {
	
	// this is also ...int nDataSamples, int nChannels
	mAU = new double* [columns];  // store mAU as columns by rows but retrieve the other way around
	mW = new double [columns];
	mWInverse = new double [columns];
	mV = new double* [columns];
	mCheckIn = new bool [columns];

	for (int i=0; i<columns; i++) {

		mAU [i] = new double [rows];
		mV [i] = new double [columns];
		mCheckIn [i] = false;
		mW [i] = mWInverse [i] = 0.0;
	}
}


SingularValueDecomposition :: SingularValueDecomposition (SampleData* data) : mSVDComputed (false),
mWInverseComputed (false), mData (data) {

	// extract mRows = nDataSamples and mColumns = nChannels + 1 from mData
	mRows = data->GetNumberOfDataElementsForDataChannel (1);
	mColumns = data->GetNumberOfDataChannels () + 1;

	mAU = new double* [mColumns];  // store mAU as columns by rows but retrieve the other way around
	mW = new double [mColumns];
	mWInverse = new double [mColumns];
	mV = new double* [mColumns];
	mCheckIn = new bool [mColumns];

	for (int i=0; i<mColumns; i++) {

		mAU [i] = new double [mRows];
		mV [i] = new double [mColumns];
		mCheckIn [i] = false;
		mW [i] = mWInverse [i] = 0.0;
	}
}


SingularValueDecomposition :: ~SingularValueDecomposition () {

	delete[] mW;
	delete[] mWInverse;
	delete[] mCheckIn;

	for (int i=0; i<mColumns; i++) {

		delete[] mAU [i];
		delete[] mV [i];
	}

	delete[] mAU;
	delete[] mV;
}


void SingularValueDecomposition :: AddColumn (int columnNumber, double* column) {

	for (int i=0; i<mRows; i++)
		A (i, columnNumber) = column [i];

	mCheckIn [columnNumber] = true;
}


void SingularValueDecomposition :: AddColumn (int columnNumber, int* column) {

	for (int i=0; i<mRows; i++)
		A (i, columnNumber) = (double)column [i];

	mCheckIn [columnNumber] = true;
}



void SingularValueDecomposition :: AddColumn (int columnNumber) {

	if (mData != NULL) {

		const short* column1 = mData->GetRawDataForChannel (columnNumber + 1);
		int offset = mData->GetDataOffset ();
		const short* column = column1 + offset;

		for (int i=0; i<mRows; i++)
			A (i, columnNumber) = (double)column [i];

		mCheckIn [columnNumber] = true;
	}
}


void SingularValueDecomposition :: AddUnitColumn (int columnNumber) {

	for (int i=0; i<mRows; i++)
		A (i, columnNumber) = 1.0;

	mCheckIn [columnNumber] = true;
}


void SingularValueDecomposition :: PopulateDataMatrix () {

	if (mData != NULL) {
	
		int NChannels = mData->GetNumberOfDataChannels ();

		for (int j=0; j<NChannels; j++) {
			AddColumn (j);
		}

		AddUnitColumn (NChannels);
	}
}


bool SingularValueDecomposition :: ComputeSVD () {

	mSVDComputed = false;
	
	if (mRows < mColumns)
		return false;

	for (int ii=0; ii<mColumns; ii++) {

		if (!mCheckIn [ii])
			return false;
	}

	mSVDComputed = true;
	double* rv1 = new double [mRows];
	double G = 0.0;
	double Scale = 0.0;
	double ANorm = 0.0;
	int I;
	int L;
	int K;
	int J;
	double S;
	double F = 0.0;
	double H;
	int NM;
	int JJ;
	double X;
	double Y;
	double Z;
	double C;

	for (I=0; I<mColumns; I++) {  // do [25]

		L = I + 1;
		rv1 [I] = Scale * G;
		G = 0.0;
		S = 0.0;
		Scale = 0.0;

		if (I < mRows) {

			for (K=0; K<mRows; K++)
				Scale += fabs (A (K,I));

			if (Scale != 0.0) {

				for (K=I; K<mRows; K++) {

					A (K, I) = A (K, I) / Scale;
					S += A (K, I) * A (K, I);
				}

				F = A (I, I);
				G = sign (sqrt (S), F);
				H = F * G - S;
				A (I, I) = F - G;

				if (L != mColumns) {

					for (J=L; J<mColumns; J++) {

						S = 0.0;

						for (K=I; K<mRows; K++)
							S += A (K, I) * A (K, J);

						F = S / H;

						for (K=I; K<mRows; K++)
							A (K, J) += F * A (K, I);
					}
				}

				for (K=I; K<mRows; K++)
					A (K, I) *= Scale;
			}
		}

		mW [I] = Scale * G;
		G = 0.0;
		S = 0.0;
		Scale = 0.0;

		if ((I < mRows) && (L != mColumns)) {

			for (K=L; K<mColumns; K++)
				Scale += fabs (A (I, K));

			if (Scale != 0.0) {

				for (K=L; K<mColumns; K++) {

					A (I, K) = A (I, K) / Scale;
					S += A(I, K) * A (I, K);
				}

				F = A (I, L);
				G = - sign (sqrt (S), F);
				H = F * G - S;
				A (I, L) = F - G;

				for (K=L; K<mColumns; K++)
					rv1 [K] = A (I, K) / H;

				if (L != mRows) {

					for (J=L; J<mRows; J++) {

						S = 0.0;
						
						for (K=L; K<mColumns; K++)
							S += A (J, K) * A (I, K);

						for (K=L; K<mColumns; K++)
							A(J, K) += S * rv1 [K];
					}
				}

				for (K=L; K<mColumns; K++)
					A(I, K) *= Scale;
			}
		}

		ANorm = dmax (ANorm, (fabs (mW [I]) + fabs (rv1 [I])));
	}  // [25]

	for (I=mColumns-1; I>=0; I--) {  // do [32]

		if (L < mColumns) {

			if (G != 0.0) {

				for (J=L; J<mColumns; J++)
					mV [J][I] = (A (I, J) / A (I, L)) / G;

				for (J=L; J<mColumns; J++) {

					S = 0.0;

					for (K=L; K<mColumns; K++)
						S += A (I, K) * mV [K][J];

					for (K=L; K<mColumns; K++)
						mV [K][J] += S * mV [K][I];
				}
			}

			for (J=L; J<mColumns; J++) {

				mV [I][J] = 0.0;
				mV [J][I] = 0.0;
			}
		}

		mV [I][I] = 1.0;
		G = rv1 [I];
		L = I;
	}  // [32]

	for (I=mColumns-1; I>=0; I--) {  // do [39]

		L = I + 1;
		G = mW [I];

		if (L < mColumns) {

			for (J=L; J<mColumns; J++)
				A (I, J) = 0.0;
		}

		if (G != 0.0) {

			G = 1.0 / G;

			if (L != mColumns) {

				for (J=L; J<mColumns; J++) {  // do [36]

					S = 0.0;

					for (K=L; K<mRows; K++)
						S += A (K, I) * A (K, J);

					F = (S / A (I, I)) * G;

					for (K=I; K<mRows; K++)
						A (K, J) += F * A (K, I);
				}  // [36]
			}

			for (J=I; J<mRows; J++)
				A(J, I) *= G;
		}

		else {

			for (J=I; J<mRows; J++)
				A(J, I) = 0.0;
		}

		A(I, I) += 1.0;
	}  // [39]

	for (K=mColumns-1; K>=0; K--) {  // do [49]...loop over singular values

		for (int ITS=0; ITS<30; ITS++) {  // do [48]...loop over allowed interations

			for (L=K; L>=0; L--) {  // do [41]...test for splitting

				NM = L - 1;   // note that rv1[0] is always zero

				if ((fabs (rv1[L]) + ANorm) == ANorm) goto two;  // oops, a little non-structured programming!
				if ((fabs (mW [NM]) + ANorm) == ANorm) goto one;  // oops, a little more...
			}  // [41]

one:		C = 0.0;   //  Cancellation of rv1[L], if L>1
			S = 1.0;

			for (I=L; I<=K; I++) {  // do [43]

				F=S * rv1 [I];

				if ((fabs (F) + ANorm) != ANorm) {

					G = mW [I];
					H = sqrt (F * F + G * G);
					mW [I] = H;
					H = 1.0 / H;
					C = G * H;
					S = - (F * H);

					for (J=0; J<mRows; J++) {  // do [42]

						Y = A (J, NM);
						Z = A (J, I);
						A (J, NM) = (Y * C) + (Z * S);
						A (J, I) = - (Y * S) + (Z * C);
					}  // [42]
				}
			}  // [43]

two:		Z = mW [K];

			if (L == K) {  // convergence

				if (Z < 0.0) {

					mW [K] = -Z;

					for (J=0; J<mColumns; J++)
						mV [J][K] = - mV [J][K];
				}

				goto three;   // more non-structured stuff...whew!  This is more than I've ever seen in one place!
			}

			if (ITS == 29)  // No convergence in 30 iterations...what shall we do???  return false??  Wait, I know!  Make mSVDComputed = false!
				mSVDComputed = false;

			X = mW [L];
			NM = K - 1;
			Y = mW [NM];
			G = rv1 [NM];
			H = rv1 [K];
			F = ((Y - Z) * (Y + Z) + (G - H) * (G + H)) / (2.0 * H * Y);
			G = sqrt (F * F + 1.0);
			F = ((X - Z) * (X + Z) + H * ((Y / (F + sign (G, F))) - H)) / X;

			// Next QR transformation...

			C = 1.0;
			S = 1.0;

			for (J=L; J<=NM; J++) {  // do [47]

				I = J + 1;
				G = rv1 [I];
				Y = mW [I];
				H = S * G;
				G = C * G;
				Z = sqrt (F * F + H * H);
				rv1 [J] = Z;
				C = F / Z;
				S = H / Z;
				F = (X * C) + (G * S);
				G = -(X * S) + (G * C);
				H = Y * S;
				Y = Y * C;

				for (JJ=0; JJ<mColumns; JJ++) {  // do [45]

					X = mV [JJ][J];
					Z = mV [JJ][I];
					mV [JJ][J] = (X * C) + (Z * S);
					mV [JJ][I] = -(X * S) + (Z * C);
				}  // [45]

				Z = sqrt (F * F + H * H);
				mW [J] = Z;   // rotation can be arbitrary if Z = 0

				if (Z != 0.0) {

					Z = 1.0 / Z;
					C = F * Z;
					S = H * Z;
				}

				F = (C * G) + (S * Y);
				X = -(S * G) + (C * Y);

				for (JJ=0; JJ<mRows; JJ++) {  // do [46]

					Y = A (JJ, J);
					Z = A (JJ, I);
					A (JJ, J) = (Y * C) + (Z * S);
					A (JJ, I) = -(Y * S) + (Z * C);
				}  // [46]
			}  // [47]

			rv1 [L] = 0.0;
			rv1 [K] = F;
			mW [K] = X;
		}  // [48]

three:	continue;
	}  // [49]

	delete[] rv1;
	return true;
}


int SingularValueDecomposition :: CalculateWInverse () {

	double WMax = 0.0;
	int ReturnValue = 0;
  int i;
	for (i=0; i<mColumns; i++) {

		if (fabs (mW [i]) > WMax)
			WMax = fabs (mW [i]);
	}

	double threshold = WMax * SingularValueDecomposition::GetWCoefficientThreshold ();

	for (i=0; i<mColumns; i++) {

		if (mW [i] < threshold) {

			ReturnValue++;
			mWInverse [i] = 0.0;
		}

		else
			mWInverse [i] = 1.0 / mW [i];
	}

	mWInverseComputed = true;
	return ReturnValue;
}



bool SingularValueDecomposition :: PerformRegression (double* const coefficients, const double* const inputData) {

	if (!(mWInverseComputed && mSVDComputed))
		return false;

	double* WUb = new double [mColumns];
	double innerProduct;
	int j;
  int i;
	for (i=0; i<mColumns; i++) {

		if (mWInverse [i] == 0.0)
			WUb [i] = 0.0;

		else {

			innerProduct = 0.0;

			for (j=0; j<mRows; j++)
				innerProduct += U (j, i) * inputData [j];
			
			WUb [i] = innerProduct * mWInverse [i];;
		}
	}

	for (i=0; i<mColumns; i++) {

		innerProduct = 0.0;

		for (j=0; j<mColumns; j++)
			innerProduct += mV [i][j] * WUb [j];

		coefficients [i] = innerProduct;
	}
	
	delete [] WUb;
	return true;
}


bool SingularValueDecomposition :: PerformRegression (double**& matrix) {

	if (mData == NULL)
		return false;
	
	bool returnValue = true;

	if (!(mWInverseComputed && mSVDComputed))
		return false;

	matrix = new double* [mColumns];
  int i;
	for (i=0; i<mColumns; i++)
		matrix [i] = new double [mColumns];

	double* inputData;
	int columns = mColumns - 1;

	for (i=0; i<columns; i++) {

		inputData = mData->GetDataForDataChannel (i + 1);

		if (!PerformRegression (matrix [i], inputData))
			returnValue = false;

		delete[] inputData;
	}

	for (i=0; i<mColumns; i++)
		matrix [columns][i] = 0.0;

	return returnValue;
}


double SingularValueDecomposition :: sign (double x, double y) {

	if (y >= 0.0)
		return fabs (x);

	return -fabs (x);
}


double SingularValueDecomposition :: dmax (double x, double y) {

	if (x > y)
		return x;

	return y;
}


