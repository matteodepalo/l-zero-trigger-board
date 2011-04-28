#include <Riostream.h>
#include <TList.h>
#include <TFolder.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TNtuple.h>
#include <TString.h>
#include <vector>

#ifndef TBDATAPARSER_H_
#define TBDATAPARSER_H_

class TBDataParser {
private:
	TList *_foldersStack;
	TObjArray *_vectorsStack;
	TObjArray *_joinedNameStack;
	TNtuple *_vector;
	TString *_currentMethod;
	TString *_currentElement;
	Int_t _motherVecEntries;
	
public:	
	TBDataParser();
	
	void	 Parse(const char *file);
	void     OnStartDocument();
	void     OnEndDocument();
	void     OnStartElement(const char*, const TList*);
	void     OnEndElement(const char*);
	void     OnCharacters(const char*);
	void     OnComment(const char*);
	void     OnWarning(const char*);
	void     OnError(const char*);
	void     OnFatalError(const char*);
	void     OnCdataBlock(const char*, Int_t);
};

#endif