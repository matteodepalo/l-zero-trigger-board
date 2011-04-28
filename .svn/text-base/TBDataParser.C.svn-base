#include "TBDataParser.h"
#include <TSAXParser.h>

TBDataParser::TBDataParser() {
	_foldersStack = new TList();
	_vectorsStack = new TObjArray();
	_joinedNameStack = new TObjArray();
	_joinedNameStack->SetOwner();
	_foldersStack->AddLast(gROOT->GetRootFolder());
	_currentMethod = NULL;
	_currentElement = NULL;
}

void TBDataParser::Parse(const char *file) 
{
	TSAXParser *parser = new TSAXParser();
	parser->ConnectToHandler("TBDataParser", this);
	parser->ParseFile(file);
}

void TBDataParser::OnStartDocument() 
{ 

}

void TBDataParser::OnEndDocument()
{

}

void TBDataParser::OnStartElement(const char *element, const TList *attributes)
{
	TXMLAttr *attr;
	TIter next(attributes);

	char *name = element;
	_currentElement = new TString(element);
	
	char *method = NULL;

	while ((attr = (TXMLAttr*) next())) {
		char *attrName = attr->GetName();
		char *attrValue = attr->GetValue();

		if(!strcmp(attrName, "name")) {
			name = attrValue; 
		}
		
		if(!strcmp(attrName, "method")) {
			method = attrValue;	
			_currentMethod = new TString(method);
		}
		
	}

	TFolder *currentFolder = _foldersStack->Last();

	if(!strcmp(element, "vector")) {
		TString *nameString = new TString(name);
		vector<float> *values = new vector<float>;
		_values = values;

		nameString->ReplaceAll(" ","_");

		TObjString *currentVector = new TObjString(nameString->Data());

		TObjArray *vectorsStack = _vectorsStack;
		vectorsStack->AddLast(currentVector);

		TString *joinedName = new TString(((TObjString *) vectorsStack->First())->GetString().Data());

		for(Int_t i = 1; i < vectorsStack->GetEntries(); i++) {
			joinedName->Append("_");
			joinedName->Append(((TObjString *) vectorsStack->At(i))->GetString().Data());
		}
		
		TObjString *joinedNameObj = new TObjString(joinedName->Data());
		TObjArray *joinedNameStack = _joinedNameStack;
		
		TNtuple *vector = new TNtuple(joinedName->Data(),joinedName->Data(),"values");
		_vector = vector;
		currentFolder->Add(vector);

		if(joinedNameStack->Contains(joinedName->Data()))
			cout << joinedName->Data() << "Error: " << "vector already exists. Be sure that in your XML file every vector has a unique path + name combination" << endl;
			
		joinedNameStack->Add(joinedNameObj);
			
			
		return;
	}

	_foldersStack->AddLast(currentFolder->AddFolder(name, name));
}

void TBDataParser::OnEndElement(const char *element)
{
	_currentMethod = NULL;
	_currentElement = NULL;
	
	if(!strcmp(element, "vector") && _vectorsStack->GetEntries() == 1) {
		_motherVecEntries = 0;
		
		_vectorsStack->RemoveLast();
		
		return;
	}
	
	if(!strcmp(element, "vector")) {
		_vectorsStack->RemoveLast();
		
		return;
	}
	
	if(!strcmp(element, "dataset")) {
		_joinedNameStack->Clear();
		_foldersStack->RemoveLast();
		return;
	}
	
	

	_foldersStack->RemoveLast();
}

void TBDataParser::OnCharacters(const char *characters)
{	
	if(_currentElement != NULL && !strcmp(_currentElement->Data(),"vector")) {
		TString *string = new TString(characters);
		TObjArray *values = string->Tokenize(", ");

		for(Int_t i = 0; i < values->GetEntries(); i++) {
			TObjString *object = (TObjString *) values->At(i);
			TString value = object->GetString().ReplaceAll("\n", "").ReplaceAll("\t", "").ReplaceAll(" ", "").ReplaceAll("\0", "");
			if(value.IsFloat()) {
				_vector->Fill(value.Atof());
			}
		}

		if(_vectorsStack->GetEntries() == 1) {
			_motherVecEntries = _vector->GetEntries();
		} else if(_currentMethod != NULL && !strcmp(_currentMethod->Data(),"all")) {
			for(Int_t i = 1; i < _motherVecEntries; i++) {
				TObjString *object = (TObjString *) values->First();
				TString value = object->GetString().ReplaceAll("\n", "").ReplaceAll("\t", "").ReplaceAll(" ", "").ReplaceAll("\0", "");
				if(value.IsFloat())
					_vector->Fill(value.Atof());
			}
		} 

		values->Delete();

	}

}

void TBDataParser::OnComment(const char *text)
{

}

void TBDataParser::OnWarning(const char *text)
{
	cout << "Warning: " << text << endl;
}

void TBDataParser::OnError(const char *text)
{
	cout << "Error: " << text << endl;
}

void TBDataParser::OnFatalError(const char *text)
{
	cout << "Fatal error: " << text << endl;
}

void TBDataParser::OnCdataBlock(const char *text, Int_t len)
{

}