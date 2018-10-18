//
//  Reads panels file and parses
//

#ifndef _PANELS_H_
#define _PANELS_H_

#include "rgstring.h"
#include "rgtokenizer.h"
#include "LadderStructures.h"
#include "LadderInputFile.h"

class Panels {
public:
	Panels (const RGString& panelFileName);
	~Panels ();

	bool isValid () const { return mValid; }
	Locus* ReadNextLine (LadderInputFile& inputFile);

protected:
	RGString mPanelFileName;
	RGFile* mPanelInputFile;
	bool mFoundFirstLine;
	bool mAtEndOfFile;
	bool mValid;
	RGStringTokenizer* mLineTokenizer;
	RGStringTokenizer* mAlleleTokenizer;
	RGString mLineString;
	RGString mAlleleString;

	int PanelsNumberOfLinesSkipped;
	int ColorColumn;
	int RepeatSizeColumn;
	int AlleleListColumn;
	RGString AlleleListDelineation;
	RGString ColumnDelineation;
};


#endif  /*   _PANELS_H_  */
