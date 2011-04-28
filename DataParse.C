#include "TBDataParser.h"
#include <string.h>

void DataParse(const char *dataFile = "Data.xml", const char *dataFolder = "TriggerBoardData") {
	
	gROOT->LoadMacro("TBDataParser.C");
	TBDataParser *parser = new TBDataParser();
	parser->Parse(dataFile);
	
	TString dataFileS = dataFile;
	TObjArray *dataFileArray = dataFileS.Tokenize(".");
	TString dataRootFile = ((TObjString*)dataFileArray->First())->GetString();
	dataRootFile.Append(".root");
	
	TFile *f = new TFile(dataRootFile,"UPDATE");
	(TFolder*)gROOT->FindObjectAny(dataFolder)->Write("",TObject::kOverwrite);
	f->Close();
	TBrowser *browser = new TBrowser();

}