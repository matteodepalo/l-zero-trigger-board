void WaveformsFileMaker(const char *outfname="Waveforms.root",const char *WaveformFilesDir = "./WaveformFiles") { 

	// This macro creates a .root file containing trees of x and y couples extracted from every .csv file in the sub folder WaveformFilesDir
	
	// create the list of files to access

	void *dir = gSystem->OpenDirectory(gSystem->ExpandPathName(WaveformFilesDir));
	if (dir) {
		const char *file = 0;
		TList *listOfFiles  = new TList();
		listOfFiles->SetOwner();
		while ((file = gSystem->GetDirEntry(dir))) {
			if (!strcmp(file,".") || !strcmp(file,"..")) continue;
			TString s = file;
			cout << s << endl;
			listOfFiles->Add(new TSystemFile(file, gSystem->ExpandPathName(WaveformFilesDir)));
		}
	}
	gSystem->FreeDirectory(dir);

	if(listOfFiles->GetEntries() != 0) {

		cout << "Output file: " << outfname << endl;
	// delete old file if exists (“RECREATE”)
		TFile *outfile = new TFile(outfname,"RECREATE");

	// Write trees in the file

		for(Int_t i = 0; i < listOfFiles->GetEntries(); i++)
		{
			TFile *csvFile = (TFile*)listOfFiles->At(i);
			TString *csvFileName = new TString(csvFile->GetName());
			TObjArray *csvFileNameList = csvFileName->Tokenize(".");
			if(strcmp(((TObjString *) csvFileNameList->Last())->GetString().Data(), "csv"))
				continue;
			csvFileNameList->RemoveLast();
			TString *joinedName = new TString(((TObjString *) csvFileNameList->First())->GetString().Data());
			if(csvFileNameList->GetEntries() > 1) {
				for(Int_t j = 1; j < csvFileNameList->GetEntries();j++) {
					joinedName->Append(".");
					joinedName->Append(((TObjString *) csvFileNameList->At(j))->GetString().Data());
				}
			}
			TTree *tree = new TTree(joinedName->Data(),joinedName->Data());
			TString *csvFilePath = new TString(WaveformFilesDir);
			csvFilePath->Append("/");
			csvFilePath->Append(csvFileName->Data());
			int nlines = (int) tree->ReadFile(csvFilePath->Data(),"x:y");
			cout<<"Read "<<nlines<<" lines of data."<<endl;
			tree->Write("",TObject::kOverwrite);
		}
	// Create TBranches for the TTree and read data
		outfile->Close();	
		
		} else cout << "Error: the directory " << WaveformFilesDir <<  " must contain at least one file" << endl;
}