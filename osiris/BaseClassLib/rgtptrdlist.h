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
*  FileName: rgtptrdlist.h
*  Author:   Robert Goor
*
*/
//
// Module Name:
//        rgtptrdlist.h
// Abstract:
//        A template-based set of classes for doubly linked lists of pointers to objects of a class T.
//	The class T should support the function Boolean operator==.
//


#ifndef _RGTPTRDLIST_H_
#define _RGTPTRDLIST_H_



template <class T> class RGTPtrDataElement {

public:
        RGTPtrDataElement () : Next (NULL), Previous (NULL), data (NULL) {}
        RGTPtrDataElement (const T* newdata) : Next (NULL), 
			Previous (NULL), data ((T*)newdata) {}
        ~RGTPtrDataElement () {}

        T* GetData () {return data;}
		void SetData (const T* t) { data = t; }
        RGTPtrDataElement<T>*& next () {return Next;}
        RGTPtrDataElement<T>*& previous () {return Previous;}

protected:
        RGTPtrDataElement<T>* Next;
        RGTPtrDataElement<T>* Previous;
        T* data;
};





template <class T> class RGTPtrDList {

public:
        RGTPtrDList () : head (NULL), tail (NULL), NEntries (0) {}
        ~RGTPtrDList () {}

        // list modifiers

        void clearAndDestroy ();

        T* append (T* data);
        T* prepend (T* data);

        T* remove (const T* data);
		T* RemoveFirst ();

        // list information

        int entries () const {return NEntries;}
        T* find (const T* data) const;
        T* index (int i) const;
        T* first () const { return head->GetData (); }
        T* last () const { return tail->GetData (); }

protected:
        RGTPtrDataElement<T>* head;
        RGTPtrDataElement<T>* tail;
        int NEntries;
};


template <class T> void RGTPtrDList<T> :: clearAndDestroy () {
        
        RGTPtrDataElement<T>* next = head;
        RGTPtrDataElement<T>* next2;

        while (next) {
			
			next2 = next->next();
			delete (next->GetData ());
			delete next;
			next = next2;
        }

        head = tail = NULL;
        NEntries = 0;
}


template <class T> T* RGTPtrDList<T> :: append (T* data) {
        
		RGTPtrDataElement<T>* newElement = new RGTPtrDataElement<T> (data);
        newElement->previous() = tail;
        newElement->next() = NULL;

		if (tail == NULL)
			head = tail = newElement;

		else {
			tail->next() = newElement;
			tail = newElement;
		}

        NEntries++;
        return data;
}


template <class T> T* RGTPtrDList<T> :: prepend (T* data) {
        
	    RGTPtrDataElement<T>* newElement = new RGTPtrDataElement<T> (data);
        newElement->next() = head;
        newElement->previous() = NULL;

		if (head == NULL)
			head = tail = newElement;

		else {
			head->previous() = newElement;
			head = newElement;
		}

        NEntries++;
        return data;
}


template <class T> T* RGTPtrDList<T> :: remove (const T* data) {

        RGTPtrDataElement<T>* next = head;
        const T* CurrentData;

        while (next) {
                
			CurrentData = next->GetData();
			
			if (*CurrentData == *data) {
            
				RGTPtrDataElement<T>* NextPrev = next->previous();
				RGTPtrDataElement<T>* NextNext = next->next();
                
                if (next == head) {
                
					if (next == tail) {
						
						head = tail = NULL;
						NEntries = 0;
						delete next;
						return (T*) CurrentData;
                    }
					
                    NextNext->previous() = NULL;
                    head = NextNext;
                    delete next;
                    return (T*) CurrentData;
                }
				
                if (next == tail) {
                
					if (next == head) {
						
						head = tail = NULL;
						NEntries = 0;
						delete next;
						return (T*) CurrentData;
                    }
					
                    NextPrev->next() = NULL;
                    tail = NextPrev;
                    delete next;
                    return (T*) CurrentData;
                }
                
                NextPrev->next() = NextNext;
                NextNext->previous() = NextPrev;
                NEntries--;
                delete next;
                return (T*) CurrentData;
            }
			
            next = next->next();
        }

        return NULL;
}


template <class T> T* RGTPtrDList<T> :: RemoveFirst () {

    if (head == NULL)
		return NULL;

	T* temp = head->GetData ();

	if (NEntries == 1) {

		delete head;
		head = NULL;
		tail = NULL;
		NEntries = 0;
		return temp;
	}

	RGTPtrDataElement<T>* Next = head->next ();
	Next->previous () = NULL;
	delete head;
	head = Next;
	NEntries--;
	return temp;
}


template <class T> T* RGTPtrDList<T> :: find (const T* data) const {

        RGTPtrDataElement<T>* next = head;
        const T* CurrentData;

        while (next) {
                
                CurrentData = next->GetData();
                
                if (*CurrentData == *data)
                        return (T*) CurrentData;

                next = next->next();
        }

        return NULL;
}


template <class T> T* RGTPtrDList<T> :: index (int i) const {

        if ((i < 0) || (i >= NEntries))
			return NULL;
        
        RGTPtrDataElement<T>* next = head;
        int j = 0;

        while (next) {
			
			if (i == j)
				return next->GetData ();
            
            j++;
            next = next->next();
        }

        return NULL;
}


#endif  /*  _RGTPTRDLIST_H_   */
