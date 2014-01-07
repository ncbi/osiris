//
//   class RGTree which is persistent and which manages subtrees as well as persistent data
//

#ifndef _RGTREE_H_
#define _RGTREE_H_

#include "persist.h"
#include "rgdlist.h"
#include "rgstring.h"

const int _RGTREE_ = 4;

class RGTree : public RGPersistent {

public:
	RGTree ();
	RGTree (RGTree* parent);
	RGTree (RGTree* parent, const RGString& name);
	RGTree (const RGString& name);
	~RGTree ();

	void SetName (const RGString& name);
	void SetParent (RGTree* parent);
	void RecursivelySetParent (RGTree* parent);
	void BuildFullPathName (const char* delimiter);

	RGTree* AddSubtree (RGTree* NewTree);
	RGPersistent* AddDataItem (RGPersistent* NewDataItem);
	RGTree* InsertSubtree (RGTree* NewTree);
	RGPersistent* InsertDataItem (RGPersistent* NewDataItem);
	RGTree* RemoveSubtree (RGTree* Target);
	RGTree* RemoveSubtree (const RGString& name);
	RGPersistent* RemoveDataItem (RGPersistent* Target);

	RGTree* GetParent () { return Parent; }
	
	void ResetSubtreeList ();
	void ResetAllSubtreeLists ();
	void ResetDataItemsList ();
	void ResetAllDataLists ();
	void ResetAllLists ();

	RGTree* NextSubtree ();
	RGPersistent* NextDataItem ();

	RGTree* NextDeepSubtree ();
	RGPersistent* NextDeepDataItem ();

protected:
	RGDList Subtrees;
	RGTree* Parent;
	RGDList DataList;
	RGString Name;
	RGDListIterator* SubtreeIterator;
	RGDListIterator* DataIterator;
	RGString FullPathName;
};

#endif  /*  _RGTREE_H_  */
