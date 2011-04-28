// -----------------------
//  Level 0 Trigger Board Project
//  analyze the behavior of the adder on the board
//
//  Input:
//   - data file
//  Output:
//   -  print alpha coefficients for every channel
//
//   Structure:
//   1. read data file
//   2. fill trees with data
//   3. compute alpha coefficients and print them
//
//
// m.depalo jul.2010
//


#include <vector>

void AnalyzeAdder(const char *inFile = "TriggerBoardData.root") {
	
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
	
	const Int_t workingChannels = 5;

	// alpha coefficient for channel 5
	
	Float_t vSingle, vSingle_err, vDiff, vDiff_err;
	
	TString path = "Adder/";

	nx = (TNtuple*)dataSet->FindObjectAny(path + "V_single");
	ny = (TNtuple*)dataSet->FindObjectAny(path + "V_differential");
	nex = (TNtuple*)dataSet->FindObjectAny(path + "V_single_errors");
	ney = (TNtuple*)dataSet->FindObjectAny(path + "V_differential_errors");

	nx->SetBranchAddress("values",&px);
	ny->SetBranchAddress("values",&py);
	nex->SetBranchAddress("values",&pex);
	ney->SetBranchAddress("values",&pey);

	graphPoints = nx->GetEntries();

	for(Int_t i = 0; i < graphPoints; i++) {
		nx->GetEntry(i); vSingle = px;
		ny->GetEntry(i); vDiff = py;
		nex->GetEntry(i); vSingle_err = pex;
		ney->GetEntry(i); vDiff_err = pey;
	}
	
	Float_t beta = vSingle / vDiff;
	Float_t beta_err = beta*TMath::Sqrt(TMath::Power(vSingle_err/vSingle,2) + TMath::Power(vDiff_err/vDiff,2));
	
	cout << "beta = " << beta << endl;
	cout << "beta error = " << beta_err << endl;
	
	nx->ResetBranchAddresses();
	ny->ResetBranchAddresses();
	nex->ResetBranchAddresses();
	ney->ResetBranchAddresses();
	
	// channel 5 alpha coefficient
	
	// the vInput vector is the same for every channel so i'm saving it now
	
	path = "Adder/Channel 5/";
	
	nx = (TNtuple*)dataSet->FindObjectAny(path + "V_input");
	nex = (TNtuple*)dataSet->FindObjectAny(path + "V_input_errors");

	nx->SetBranchAddress("values",&px);
	nex->SetBranchAddress("values",&pex);

	graphPoints = nx->GetEntries();

	for(Int_t i = 0; i < graphPoints; i++) {
		nx->GetEntry(i); vx.push_back(px);
		nex->GetEntry(i); vex.push_back(pex);
	}
	
	vector<float> vInput = vx;
	vector<float> vInput_err = vex;
	
	nx->ResetBranchAddresses();
	vx.clear();
	nex->ResetBranchAddresses();
	vex.clear();
	
	// now to the analysis
	
	path = "Adder/Channel ";
	
	TList *alphaGraphsList = new TList();
	TGraphErrors *gAlphaCh5 = new TGraphErrors();
	alphaGraphsList->Add(gAlphaCh5);
	TGraphErrors *gAlphaCh7 = new TGraphErrors();
	alphaGraphsList->Add(gAlphaCh7);
	TGraphErrors *gAlphaCh4 = new TGraphErrors();
	alphaGraphsList->Add(gAlphaCh4);
	TGraphErrors *gAlphaCh0 = new TGraphErrors();
	alphaGraphsList->Add(gAlphaCh0);
	TGraphErrors *gAlphaCh1 = new TGraphErrors();
	alphaGraphsList->Add(gAlphaCh1);
	
	TList *folderNameList = new TList();
	TObjString *objsCh5 = new TObjString("5");
	folderNameList->Add(objsCh5);
	TObjString *objsCh4 = new TObjString("4");
	folderNameList->Add(objsCh4);
	TObjString *objsCh7 = new TObjString("7");
	folderNameList->Add(objsCh7);
	TObjString *objsCh0 = new TObjString("0");
	folderNameList->Add(objsCh0);
	TObjString *objsCh1 = new TObjString("1");
	folderNameList->Add(objsCh1);
	
	for(Int_t j = 0; j < workingChannels; j++) {

		nx = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(j))->GetString().Data() + "/V_comparator_output");
		ny = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(j))->GetString().Data() + "/V_differential_adder_output");
		nex = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(j))->GetString().Data() + "/V_comparator_output_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(j))->GetString().Data() + "/V_differential_adder_output_errors");

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

		vector<float> vAdderOutputSingle;
		vector<float> vAdderOutputSingle_err;
		vector<float> vAlpha;
		vector<float> vAlpha_err;
		
		currentAlphaG = (TGraphErrors*)alphaGraphsList->At(j);
		currentAlphaG->Set(graphPoints);
		
		TString title = "Alpha coefficient Channel ";
		
		currentAlphaG->SetTitle(title + ((TObjString*)folderNameList->At(j))->GetString().Data());
		currentAlphaG->GetXaxis()->SetTitle("Input signal amplitude (mV)");

		for(Int_t i = 0; i < graphPoints; i++) {
			vAdderOutputSingle.push_back(beta*vy.at(i));
			vAdderOutputSingle_err.push_back(vAdderOutputSingle.at(i)*TMath::Sqrt(TMath::Power(beta_err/beta,2) + TMath::Power(vey.at(i)/vy.at(i),2)));

			vAlpha.push_back(vAdderOutputSingle.at(i) / vx.at(i));
			vAlpha_err.push_back(vAlpha.at(i)*TMath::Sqrt(TMath::Power(vAdderOutputSingle_err.at(i)/vAdderOutputSingle.at(i),2) + TMath::Power(vex.at(i)/vx.at(i),2)));
			
			currentAlphaG->SetPoint(i,vInput.at(i),vAlpha.at(i));
			currentAlphaG->SetPointError(i,vInput_err.at(i),vAlpha_err.at(i));
		}
		
		currentAlphaG->Fit("pol0", "Q");
		
		cout << "Channel " << ((TObjString*)folderNameList->At(j))->GetString().Data() << " alpha = "<< currentAlphaG->GetFunction("pol0")->GetParameter(0) << " +/- "
			<< currentAlphaG->GetFunction("pol0")->GetParError(0) << endl;

		nx->ResetBranchAddresses();
		ny->ResetBranchAddresses();
		nex->ResetBranchAddresses();
		ney->ResetBranchAddresses();
		vAdderOutputSingle.clear();
		vAdderOutputSingle_err.clear();
		vAlpha.clear();
		vAlpha_err.clear();
		vx.clear(); vy.clear(); vex.clear(); vey.clear();

	}
}