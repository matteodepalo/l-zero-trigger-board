// This function created a graph using data contained in the folder "dataSet" in the path "path"

TGraphErrors* TBGraphErrors(TFolder *dataSet,TString path,TString xName,TString yName, Int_t nxMeasures = 1, Int_t nyMeasures = 1) {
	
	TNtuple *nx;
	TNtuple *ny;
	TNtuple *nex;
	TNtuple *ney;

	Int_t graphPoints;

	Float_t px,py,pex,pey;

	vector<float> vx,vy,vex,vey;
	
	xName.ReplaceAll(" ","_");
	yName.ReplaceAll(" ","_");
	
	TString exName = xName;
	exName.Append("_errors");
	
	TString eyName = yName;
	eyName.Append("_errors");
	
	nx = (TNtuple*)dataSet->FindObjectAny(path + xName);
	ny = (TNtuple*)dataSet->FindObjectAny(path + yName);
	nex = (TNtuple*)dataSet->FindObjectAny(path + exName);
	ney = (TNtuple*)dataSet->FindObjectAny(path + eyName);

	nx->SetBranchAddress("values",&px);
	ny->SetBranchAddress("values",&py);
	nex->SetBranchAddress("values",&pex);
	ney->SetBranchAddress("values",&pey);

	graphPoints = nx->GetEntries();

	for(Int_t i = 0; i < graphPoints; i++) {
		nx->GetEntry(i); vx.push_back(px);
		ny->GetEntry(i); vy.push_back(py);
		nex->GetEntry(i); vex.push_back(pex);
		ney->GetEntry(i); vey.push_back(pey);
		
		vex.at(i) /= TMath::Sqrt(nxMeasures);
		vey.at(i) /= TMath::Sqrt(nyMeasures);	
	}

	TGraphErrors *TBGraphErrors = new TGraphErrors(graphPoints,&vx.at(0),&vy.at(0),&vex.at(0),&vey.at(0));
	
	vx.clear(); vy.clear(); vex.clear(); vey.clear();
	nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();
	
	return TBGraphErrors;
}