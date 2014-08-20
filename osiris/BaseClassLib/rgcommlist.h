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
*  FileName: rgcommlist.h
*  Author:   Robert Goor
*
*/
//
// Module Name:
//        rgcommlist.h
// Abstract:
//        A template-based set of classes for specialized link lists
//


#ifndef _RGCOMMLIST_H_
#define _RGCOMMLIST_H_

#include <stddef.h>


template <class T> class CommDataElement {

public:
        CommDataElement () : Next (NULL), Previous (NULL), data (NULL), 
			ID (0) {}
        CommDataElement (const T* newdata, unsigned long id) : Next (NULL), 
			Previous (NULL), data ((T*)newdata), ID (id) {}
        ~CommDataElement () {}

        T* GetData () {return data;}
		void SetData (const T* t) { data = t; }
		unsigned long GetID () const { return ID; }
		void SetID (unsigned long id) { ID = id; }
        CommDataElement<T>*& next () {return Next;}
        CommDataElement<T>*& previous () {return Previous;}

protected:
        CommDataElement<T>* Next;
        CommDataElement<T>* Previous;
        T* data;
		unsigned long ID;
};





template <class T> class CommDataList {

public:
        CommDataList () : head (NULL), tail (NULL), NEntries (0) {}
        ~CommDataList () {}

        // list modifiers

        void clearAndDestroy ();

        CommDataElement<T>* append (const T* data);
        CommDataElement<T>* prepend (const T* data);

        T* remove (const T* data);
		T* RemoveFirst ();

        // list information

        int entries () const {return NEntries;}
        CommDataElement<T>* find (const T* data) const;
        CommDataElement<T>* index (int i) const;
        CommDataElement<T>* first () const { return head; }
        CommDataElement<T>* last () const { return tail; }

protected:
        CommDataElement<T>* head;
        CommDataElement<T>* tail;
        int NEntries;
};


template <class T> void CommDataList<T> :: clearAndDestroy () {
        
        CommDataElement<T>* next = head;
        CommDataElement<T>* next2;

        while (next) {
			
			next2 = next->next();
			delete next;
			next = next2;
        }

        head = tail = NULL;
        NEntries = 0;
}


template <class T> 
CommDataElement<T>* CommDataList<T> :: append (const T* data) {
        
		CommDataElement<T>* Found = find (data);
	
		if (Found)
			return NULL;

        CommDataElement<T>* newElement = new CommDataElement<T> (data, 0);
        newElement->previous() = tail;
        newElement->next() = NULL;

		if (tail == NULL)
			head = tail = newElement;

		else {
			tail->next() = newElement;
			tail = newElement;
		}

        NEntries++;
        return newElement;
}


template <class T> 
CommDataElement<T>* CommDataList<T> :: prepend (const T* data) {
        
		CommDataElement<T>* Found = find (data);
	
		if (Found)
			return NULL;    
	
	    CommDataElement<T>* newElement = new CommDataElement<T> (data, 0);
        newElement->next() = head;
        newElement->previous() = NULL;

		if (head == NULL)
			head = tail = newElement;

		else {
			head->previous() = newElement;
			head = newElement;
		}

        NEntries++;
        return newElement;
}


template <class T> 
T* CommDataList<T> :: remove (const T* data) {

        CommDataElement<T>* next = head;
        const T* CurrentData;

        while (next) {
                
			CurrentData = next->GetData();
			
			if (*CurrentData == *data) {
            
				CommDataElement<T>* NextPrev = next->previous();
				CommDataElement<T>* NextNext = next->next();
                
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


template <class T> 
T* CommDataList<T> :: RemoveFirst () {

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

	CommDataElement<T>* Next = head->next ();
	Next->previous () = NULL;
	delete head;
	head = Next;
	NEntries--;
	return temp;
}


template <class T> 
CommDataElement<T>* CommDataList<T> :: find (const T* data) const {

        CommDataElement<T>* next = head;
        const T* CurrentData;

        while (next) {
                
                CurrentData = next->GetData();
                
                if (*CurrentData == *data)
                        return next;

                next = next->next();
        }

        return NULL;
}


template <class T> 
CommDataElement<T>* CommDataList<T> :: index (int i) const {

        if ((i < 0) || (i >= NEntries))
			return NULL;
        
        CommDataElement<T>* next = head;
        int j = 0;

        while (next) {
			
			if (i == j)
				return next;
            
            j++;
            next = next->next();
        }

        return NULL;
}


#endif  /*  _RGCOMMLIST_H_   */
