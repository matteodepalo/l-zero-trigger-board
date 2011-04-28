// -----------------------
//  Level 0 Trigger Board Project
//  analyze response of the board in function of the width of the input signal
//
//  Input:
//   - data file
//  Output:
//   -  plots regarding the output of the board in function of the input
//
//   Structure:
//   1. read data file
//   2. fill trees with data
//   3. data analysis
//
//
// m.depalo jul.2010
//


void AnalyzeWidth(Int_t channel, const char *inFile = "Data.root") {
	
	gStyle->SetOptStat(0);
	
	//try to access data file and in case of failure return
	if(gSystem->AccessPathName(inFile,kFileExists)) {
		cout << "Error: file " << inFile << " does not exsist. Run .x DataParse.C to create it" << endl;
		return;
	}
	TFile *f = TFile::Open(inFile);
	TFolder *dataSet;
	
	TString dataFolderS = "TriggerBoardData";
	dataFolderS.Append(";1");

	dataSet = (TFolder*)f->Get(dataFolderS);
	cout << dataSet->GetName() << endl;

	TNtuple *nx;
	TNtuple *ny;
	TNtuple *nex;
	TNtuple *ney;

	Int_t graphPoints;

	Float_t px,py,pex,pey;

	vector<float> vx,vy,vex,vey;

	switch (channel) {
		
		case 5 :
		
		// Channel 5

		// fixed input 821.5 +/- 0.3 mV
	
		TString path = "Width/Channel 5/IOwidth/Fixed input/";

		nx = (TNtuple*)dataSet->FindObjectAny(path + "Input_width_after_the_shaper_above_the_threshold");
		ny = (TNtuple*)dataSet->FindObjectAny(path + "Output_FMHW");
		nex = (TNtuple*)dataSet->FindObjectAny(path + "Input_width_after_the_shaper_above_the_threshold_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + "Output_FMHW_errors");

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
		}

		TGraphErrors *gIOwidth = new TGraphErrors(graphPoints,&vx.at(0),&vy.at(0),&vex.at(0),&vey.at(0));
		gIOwidth->SetTitle("Output signal width in function of input signal width");

		gIOwidth->SetMarkerStyle(20);
		gIOwidth->SetMarkerSize(0.8);
		gIOwidth->GetXaxis()->SetTitle("Input signal width above the threshold (ns)");
		gIOwidth->GetYaxis()->SetTitle("Output FWHM (ns)");

		TCanvas *cIOwidthCh5 = new TCanvas("cIOwidthCh5", "Output signal width vs input signal width | Channel 5", 800,600);
		cIOwidthCh5->SetGrid();
		gIOwidth->Fit("pol1", "Q");
		gIOwidth->GetFunction("pol1")->SetLineWidth(1);
		gIOwidth->Draw("APE");

		// fixed threshold 196 mV

		Float_t xIOwidthFixDT[8];
		
		break;


		case 6 :
		
		//Channel 6

		// fixed threshold 197 mV. Actual value used to set the marker offset is 208
	
		TString path = "Width/Channel 6/IOwidth/Fixed DT/";

		nx = (TNtuple*)dataSet->FindObjectAny(path + "Input_width_after_the_shaper_above_the_threshold");
		ny = (TNtuple*)dataSet->FindObjectAny(path + "Output_FMHW");
		nex = (TNtuple*)dataSet->FindObjectAny(path + "Input_width_after_the_shaper_above_the_threshold_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + "Output_FMHW_errors");

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
		}
		
		TH2F *hIOwidth = new TH2F("hIOwidth","hIOwidth",2,0,vx.back() + 0.5,2,0,vy.back() + 0.5);
		hIOwidth->SetTitle("Output signal width in function of input signal width");
		hIOwidth->GetXaxis()->SetTitle("Input width above the threshold (ns)");
		hIOwidth->GetYaxis()->SetTitle("Output FWHM (ns)");
	
		TGraphErrors *gIOwidth = new TGraphErrors(graphPoints,&vx.at(0),&vy.at(0),&vex.at(0),&vey.at(0));
		
		gIOwidth->SetMarkerStyle(20);
		gIOwidth->SetMarkerSize(0.8);

		TCanvas *cIOwidthCh6 = new TCanvas("cIOwidthCh6", "Output signal width vs input signal width | Channel 6", 800,600);
		cIOwidthCh6->SetGrid();
		gIOwidth->Fit("pol1", "Q0");
		gIOwidth->GetFunction("pol1")->SetLineWidth(1);
		hIOwidth->Draw();
		gIOwidth->Draw("PEsame");
		gIOwidth->GetFunction("pol1")->SetRange(0,vx.back() + 0.5);
		gIOwidth->GetFunction("pol1")->Draw("same");

		break;
	}
}