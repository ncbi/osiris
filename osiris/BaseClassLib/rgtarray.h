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
*  FileName: rgtarray.h
*  Author:   Robert Goor
*
*/

//
//  Template class for dynamic arrays of pointers to template class T.  These arrays resize themselves dynamically as
//  needed.
//

#ifndef _RGTARRAY_H_
#define _RGTARRAY_H_



template <class T> class RGTarray {

public:
	RGTarray ();
	RGTarray (size_t size);
	~RGTarray ();

	// list modifiers

	void ClearAndDestroy ();
	void Clear ();

	void Resize (size_t NewSize);
	void Append (T* NewItem);
	void ResetArray ();

	T* Remove (const T* data);

	// list information

	T* Find (const T* data) const;

	T* GetElementAt (size_t index);
	T* ReplaceElementAt (size_t index, T* NewItem);  // performs limit checking and expands array if needed
	T* RemoveElementAt (size_t index);

	size_t Length () const { return CurrentLength; }

	static void ResetDefaultSize (size_t NewSize) { if (NewSize != 0) DefaultSize = NewSize; }
	static void ResetDefaultIncrement (size_t Increment) { if (Increment != 0) DefaultIncrement = Increment; }

protected:
	static size_t DefaultSize;
	static size_t DefaultIncrement;

	size_t ArraySize;
	size_t CurrentLength;

	T** tArray;

	size_t ComputeNewSize (size_t Target);
	size_t ComputeTargetSize (size_t Target);
	void NullNewItems (size_t First, size_t Last);
	void BuildNewArray (size_t size);
};


template <class T> size_t RGTarray<T>::DefaultSize = 50;
template <class T> size_t RGTarray<T>::DefaultIncrement = 50;

template <class T> RGTarray<T> :: RGTarray () : ArraySize (RGTarray<T>::DefaultSize), CurrentLength (0) {

	tArray = new T* [ArraySize];
	NullNewItems (0, ArraySize);
}


template <class T> RGTarray<T> :: RGTarray (size_t size) : ArraySize (RGPArray::DefaultSize), CurrentLength (0) {

	ArraySize = ComputeNewSize (size);
	tArray = new T* [ArraySize];
	NullNewItems (0, ArraySize);
}


template <class T> RGTarray<T> :: ~RGTarray () {

	delete[] tArray;
}


template <class T> void RGTarray<T> :: ClearAndDestroy () {

	for (size_t i=0; i<CurrentLength; i++) {
		
		delete tArray [i];
		tArray [i] = NULL;
	}

	CurrentLength = 0;
}


template <class T> void RGTarray<T> :: Clear () {

	for (size_t i=0; i<CurrentLength; i++) {
		
		tArray [i] = NULL;
	}

	CurrentLength = 0;
}


template <class T> void RGTarray<T> :: Resize (size_t NewSize) {

	size_t i;
	size_t Size = ComputeTargetSize (NewSize);
	T** NewArray;
	
	if (Size < ArraySize) {

		if (NewSize < CurrentLength) {

			for (i=NewSize; i<CurrentLength; i++) {

			//	delete tArray[i];
				tArray[i] = NULL;
			}

			CurrentLength = NewSize;
		}

		NewArray = new T* [Size];

		for (i=0; i<Size; i++)
			NewArray[i] = tArray[i];

		delete[] tArray;
		tArray = NewArray;
		ArraySize = Size;
	}

	else
		BuildNewArray (NewSize);
}


template <class T> void RGTarray<T> :: Append (T* NewItem) {

	BuildNewArray (CurrentLength + 1);
//	delete tArray [CurrentLength];
	tArray [CurrentLength] = NewItem;

	CurrentLength++;
}


template <class T> void RGTarray<T> :: ResetArray () {

	for (size_t i=0; i<CurrentLength; i++) {

	//	delete tArray [i];
		tArray [i] = NULL;
	}

	CurrentLength = 0;
}



template <class T> T* RGTarray<T> :: Remove (const T* data) {

	T* temp;
	
	if (data == NULL)
		return NULL;
	
	for (size_t i=0; i<CurrentLength; i++) {

		if (tArray [i] != NULL) {

			if (*tArray [i] == *data) {

				temp = tArray [i];
				tArray [i] = NULL;
				return temp;
			}
		}
	}

	return NULL;
}


template <class T> T* RGTarray<T> :: Find (const T* data) const {

	T* temp;
	
	if (data == NULL)
		return NULL;
	
	for (size_t i=0; i<CurrentLength; i++) {

		if (tArray [i] != NULL) {

			if (*tArray [i] == *data) {

				return tArray [i];
			}
		}
	}

	return NULL;
}


template <class T> T* RGTarray<T> :: GetElementAt (size_t index) {

	if (index >= CurrentLength)
		return NULL;

	return tArray [index];
}


template <class T> T* RGTarray<T> :: ReplaceElementAt (size_t i, T* NewItem) {
	
	// performs limit checking and expands array if needed

	if (i >= CurrentLength) {

		if (i >= ArraySize - 1)
			BuildNewArray (i + 1);

		CurrentLength = i + 1;
	}

	T* temp = tArray [i];
	tArray [i] = NewItem;
	return temp;
}


template <class T> T* RGTarray<T> :: RemoveElementAt (size_t index) {

	T* temp;

	if (index >= CurrentLength)
		return NULL;

	temp = tArray [index];
	tArray [index] = NULL;
	return temp;
}


template <class T> size_t RGTarray<T> :: ComputeNewSize (size_t Target) {

	size_t Difference;
	size_t NumberOfDefaults;
	size_t AddedSpace;
	size_t Incr = RGTarray<T>::DefaultIncrement;

	if (Target <= ArraySize)
		return ArraySize;

	Difference = Target - ArraySize;
	NumberOfDefaults = Difference / Incr;
	AddedSpace = Incr * NumberOfDefaults;

	if (AddedSpace < Difference)
		AddedSpace += Incr;

	return ArraySize + AddedSpace;
}


template <class T> size_t RGTarray<T> :: ComputeTargetSize (size_t Target) {

	size_t Difference;
	size_t NumberOfIncrements;
	size_t Incr = RGTarray<T>::DefaultIncrement;
	size_t Default = RGTarray<T>::DefaultSize;
	size_t AddedSize;
	
	if (Target <= Default)
		return Default;

	Difference = Target - Default;
	NumberOfIncrements = Difference / Incr;
	AddedSize = NumberOfIncrements * Incr;

	if (AddedSize < Difference)
		AddedSize += Incr;

	return Default + AddedSize;
}


template <class T> void RGTarray<T> :: NullNewItems (size_t First, size_t Last) {

	for (size_t i=First; i<Last; i++)
		tArray [i] = NULL;
}


template <class T> void RGTarray<T> :: BuildNewArray (size_t size) {

	T** NewArray;
	size_t NewSize = ComputeNewSize (size);

	if (NewSize > ArraySize) {

		NewArray = new T* [NewSize];

		for (size_t i=0; i<CurrentLength; i++)
			NewArray [i] = tArray [i];

		delete[] tArray;
		tArray = NewArray;
		NullNewItems (CurrentLength, NewSize);
		ArraySize = NewSize;
	}
}


#endif   /*  _RGTARRAY_H_  */
