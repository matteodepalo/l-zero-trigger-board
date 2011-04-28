// -----------------------
//  Level 0 Trigger Board Project
//  analyze the linearity on the board
//
//  Input:
//   - data file
//  Output:
//   -  plots regarding linearity
//
//   Structure:
//   1. read data file
//   2. fill trees with data
//   3. data analysis
//
//
// m.depalo jul.2010
//

#include "functions.h"
#include <vector>

void AnalyzeLinearity(Int_t channel, const char *inFile = "Data.root") {

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

		// This graph shows the V max input amplitude after tha shaper in function of the nominal amplitude given by the generator.
		
		TString path = "Linearity/Channel 5/1Gs/InputShaper/";

		nx = (TNtuple*)dataSet->FindObjectAny(path + "V_generator");
		ny = (TNtuple*)dataSet->FindObjectAny(path + "V_inputshaper");
		nex = (TNtuple*)dataSet->FindObjectAny(path + "V_generator_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + "V_inputshaper_errors");

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

		TGraphErrors *gInputShaper = new TGraphErrors(graphPoints,&vx.at(0),&vy.at(0),&vex.at(0),&vey.at(0)); 

		gInputShaper->SetTitle("Amplitude after shaper in function of the generator amplitude");

		gInputShaper->SetMarkerStyle(20);
		gInputShaper->SetMarkerSize(0.8);
		gInputShaper->GetXaxis()->SetTitle("Generator signal amplitude (mV)");
		gInputShaper->GetYaxis()->SetTitle("Amplitude of the signal after the shaper (mV)");

		TCanvas *cInputShaper = new TCanvas("cInputShaper","V Shaper vs V input",800,600);
		gInputShaper->Draw("APEL");

		// This graph shows the FMHW of the input after tha shaper in function of the nominal amplitude given by the generator.

		path = "Linearity/Channel 5/1Gs/InputShaper/";

		ny->ResetBranchAddresses(); ney->ResetBranchAddresses();
		ny = (TNtuple*)dataSet->FindObjectAny("Input_Width");
		ney = (TNtuple*)dataSet->FindObjectAny("Input_Width_errors");
		ny->SetBranchAddress("values",&py);
		ney->SetBranchAddress("values",&pey);
		vy.clear(); vey.clear();

		for(Int_t i = 0; i < ny->GetEntries(); i++) {
			ny->GetEntry(i); vy.push_back(py);
			ney->GetEntry(i); vey.push_back(pey);
		}

		TGraphErrors *gWidthInputShaper = new TGraphErrors(graphPoints,&vx.at(0),&vy.at(0),&vex.at(0),&vey.at(0)); 
		gWidthInputShaper->SetTitle("FMHW of the signal after shaper in function of the generator amplitude");
		gWidthInputShaper->SetMarkerStyle(20);
		gWidthInputShaper->SetMarkerSize(0.8);
		gWidthInputShaper->GetXaxis()->SetTitle("Generator signal amplitude (mV)");
		gWidthInputShaper->GetYaxis()->SetTitle("FMHW of the signal after the shaper (ns)");
		gWidthInputShaper->Fit("pol0","Q","",0,800);
		cout << "Average width between 100 and 800 mV: "<< gWidthInputShaper->GetFunction("pol0")->GetParameter(0)
			<< " +/- " << gWidthInputShaper->GetFunction("pol0")->GetParError(0) << endl;

		TCanvas *cWidthInputShaper = new TCanvas("cWidthInputShaper","FMHW Shaper vs V input",800,600);
		gWidthInputShaper->Draw("APEL");


		// These graphs show the V max output amplitude in function of the threshold.
		// The input signal was set at different values between 100 and 800 mV.
		// 1 Gs/s clock resulting in 1.852 ± 0.004 ns FMHW of the input signal

		TList *responseGraphs1GsList = new TList();
		TGraphErrors *gResponse100mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse100mV1Gs);
		TGraphErrors *gResponse200mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse200mV1Gs);
		TGraphErrors *gResponse300mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse300mV1Gs);
		TGraphErrors *gResponse400mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse400mV1Gs);
		TGraphErrors *gResponse500mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse500mV1Gs);
		TGraphErrors *gResponse600mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse600mV1Gs);
		TGraphErrors *gResponse700mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse700mV1Gs);
		TGraphErrors *gResponse800mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse800mV1Gs);

		TList *folderNameList = new TList();
		TObjString *objs100mV = new TObjString("100mV");
		folderNameList->Add(objs100mV);
		TObjString *objs200mV = new TObjString("200mV");
		folderNameList->Add(objs200mV);
		TObjString *objs300mV = new TObjString("300mV");
		folderNameList->Add(objs300mV);
		TObjString *objs400mV = new TObjString("400mV");
		folderNameList->Add(objs400mV);
		TObjString *objs500mV = new TObjString("500mV");
		folderNameList->Add(objs500mV);
		TObjString *objs600mV = new TObjString("600mV");
		folderNameList->Add(objs600mV);
		TObjString *objs700mV = new TObjString("700mV");
		folderNameList->Add(objs700mV);
		TObjString *objs800mV = new TObjString("800mV");
		folderNameList->Add(objs800mV);

		path = "Linearity/Channel 5/1Gs/Response/";

		TGraphErrors *currentGraph;
		Double_t *x;

		for(Int_t i = 0; i < responseGraphs1GsList->GetEntries();i++) {
			vx.clear(); vy.clear(); vex.clear(); vey.clear();
			nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

			nx = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/DT");
			ny = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/V_output");
			nex = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/DT_errors");
			ney = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/V_output_errors");

			nx->SetBranchAddress("values",&px); ny->SetBranchAddress("values",&py); nex->SetBranchAddress("values",&pex); ney->SetBranchAddress("values",&pey);

			graphPoints = nx->GetEntries();

			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));

			currentGraph->Set(graphPoints);

			for(Int_t n = 0; n < graphPoints; n++) {
				nx->GetEntry(n); vx.push_back(px);
				ny->GetEntry(n); vy.push_back(py);
				nex->GetEntry(n); vex.push_back(pex);
				ney->GetEntry(n); vey.push_back(pey);
				currentGraph->SetPoint(n,vx.at(n),vy.at(n));
				currentGraph->SetPointError(n,vex.at(n),vey.at(n));
			}

			currentGraph->SetTitle("Amplitude of the output signal in function of the threshold");
			currentGraph->SetMarkerStyle(20);
			currentGraph->SetMarkerSize(0.8);
			currentGraph->GetXaxis()->SetTitle("Threshold (mV)");
			currentGraph->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");

		}

		TMultiGraph *gResponse1Gs = new TMultiGraph("gResponse1Gs", "Collection of graphs for the 1Gs/s pulse");

		for(Int_t i = 0; i < responseGraphs1GsList->GetEntries();i++) {
			gResponse1Gs->Add((TGraphErrors*)responseGraphs1GsList->At(i));
		}

		TCanvas *cResponse1Gs = new TCanvas("cResponse1Gs", "Threshold vs Output all mV 1Gs/s", 800,600);
		gResponse1Gs->Draw("APEL");

		cResponse1Gs->Update();
		gResponse1Gs->GetXaxis()->SetTitle("Threshold (mV)");
		gResponse1Gs->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
		cResponse1Gs->Modified();

		// Graph of the input amplitude in function of the "critical" threshold.
		// "critical" means the threshold at which the output amplitude reaches the mean value between the max and the min output amplitudes.
		// To find the critical threshold the bisection method is used.
		// The equation to solve is:
		// "f(x) - mean = 0" where f(x) is the TSpline generated by root to interpolate lineary the graph points.
		// Error for the bisection method is set to 0.0001
		// The graph expected is a straight line

		const int nResponseCh5 = 8;

		Float_t DTcritic[nResponseCh5];
		Float_t VoutputMaxMean[nResponseCh5];
		Float_t VoutputMaxMeanErr[nResponseCh5];
		Float_t VoutputMinMean[nResponseCh5];
		Float_t VoutputMinMeanErr[nResponseCh5];
		Float_t VoutputMean[nResponseCh5];
		Float_t VoutputMeanErr[nResponseCh5]; 

		for(Int_t i = 0; i < nResponseCh5; i++) {
			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));

			x = currentGraph->GetX();

			currentGraph->Fit("pol0","Q0","",x[0],x[2]);

			VoutputMaxMean[i] = currentGraph->GetFunction("pol0")->GetParameter(0);
			VoutputMaxMeanErr[i] = currentGraph->GetFunction("pol0")->GetParError(0);

			graphPoints = currentGraph->GetN();

			currentGraph->Fit("pol0","Q0","",x[graphPoints - 2],x[graphPoints - 1]);

			VoutputMinMean[i] = currentGraph->GetFunction("pol0")->GetParameter(0);
			VoutputMinMeanErr[i] = currentGraph->GetFunction("pol0")->GetParError(0);
			VoutputMean[i] = (VoutputMaxMean[i] + VoutputMinMean[i])/2;
			VoutputMeanErr[i] = TMath::Sqrt(TMath::Power(VoutputMaxMeanErr[i],2) + TMath::Power(VoutputMinMeanErr[i],2));
			
			DTcritic[i] = Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMean[i]);
		}

		for(Int_t i = 1; i < nResponseCh5 + 1; i++) {
			cout << "DT critic " << i*100 << "mV = " << DTcritic[i - 1] << endl;
		}

		// Fetch data regarding V input after the shaper and V nominal imput given by the generator

		path = "Linearity/Channel 5/1Gs/Response/V input/";

		vx.clear(); vy.clear(); vex.clear(); vey.clear();
		nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

		nx = (TNtuple*)dataSet->FindObjectAny(path + "V_input_shaper");
		ny = (TNtuple*)dataSet->FindObjectAny(path + "V_nominal_input");
		nex = (TNtuple*)dataSet->FindObjectAny(path + "V_input_shaper_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + "V_nominal_input_errors");

		nx->SetBranchAddress("values",&px); ny->SetBranchAddress("values",&py); nex->SetBranchAddress("values",&pex); ney->SetBranchAddress("values",&pey);

		for(Int_t n = 0; n < graphPoints; n++) {
			nx->GetEntry(n); vx.push_back(px);
			ny->GetEntry(n); vy.push_back(py);
			nex->GetEntry(n); vex.push_back(pex);
			ney->GetEntry(n); vey.push_back(pey);
		}

		Float_t DTcriticErrP[nResponseCh5];
		Float_t DTcriticErrN[nResponseCh5];

		// compute the errors of the critical DT

		for(Int_t i = 0; i < nResponseCh5; i++) {
			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));
			x = currentGraph->GetX();
			graphPoints = currentGraph->GetN();
			DTcriticErrP[i] = DTcritic[i] - Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMean[i] + VoutputMeanErr[i]);
			DTcriticErrN[i] = fabs(DTcritic[i] - Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMean[i] - VoutputMeanErr[i]));
		}

		for(Int_t i = 0; i < nResponseCh5; ++i)
		{
			cout << "Critcal DT: " << DTcritic[i] << " Positive error: " << DTcriticErrP[i] << endl
				<< " Negative error: " << DTcriticErrN[i] << " Difference between those errors: " << DTcriticErrP[i] - DTcriticErrN[i] << endl;
		} 

		// The Positive Error is used because the difference between the positive and the negative is very small.

		TGraphErrors *gLinearityN1Gs = new TGraphErrors(nResponseCh5, DTcritic, &vy.at(0),DTcriticErrP,&vey.at(0));
		gLinearityN1Gs->SetMarkerStyle(20);
		gLinearityN1Gs->SetMarkerSize(0.8);
		gLinearityN1Gs->SetTitle("Critical DT vs Nominal Input Amplitude");
		gLinearityN1Gs->GetXaxis()->SetTitle("Critical DT (mV)");
		gLinearityN1Gs->GetYaxis()->SetTitle("Nominal amplitude of the input signal (mV)");
		gLinearityN1Gs->Fit("pol1", "Q");
		gLinearityN1Gs->GetFunction("pol1")->SetLineWidth(1);

		TCanvas *cLinearityN1Gs = new TCanvas("cLinearityN1Gs", "Critical DT vs Nominal Input Amplitude", 800,600);
		cLinearityN1Gs->SetGrid();
		gLinearityN1Gs->Draw("APE");

		// The following graph shows the actual input amplitude after the shaper (measured with the oscilloscope) in function of
		// the critical threshold

		TGraphErrors *gLinearityS1Gs = new TGraphErrors(nResponseCh5, DTcritic, &vx.at(0),DTcriticErrP,&vex.at(0));
		gLinearityS1Gs->SetMarkerStyle(20);
		gLinearityS1Gs->SetMarkerSize(0.8);
		gLinearityS1Gs->SetTitle("Critical DT vs Input Amplitude after the shaper");
		gLinearityS1Gs->GetXaxis()->SetTitle("Critical DT (mV)");
		gLinearityS1Gs->GetYaxis()->SetTitle("Amplitude of the input signal after the shaper (mV)");
		gLinearityS1Gs->Fit("pol1", "");
		gLinearityS1Gs->GetFunction("pol1")->SetLineWidth(1);

		TCanvas *cLinearityS1Gs = new TCanvas("cLinearityS1Gs", "Critical DT vs Input Amplitude after the shaper", 800,600);
		cLinearityS1Gs->SetGrid();
		gLinearityS1Gs->Draw("APE");

		vx.clear(); vy.clear(); vex.clear(); vey.clear();
		nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

		// 0.3 Gs signal
		
		// same analysis as the one done for the 1Gs signal

		TList *responseGraphsPoint3GsList = new TList();
		TGraphErrors *gResponse200mVPoint3Gs = new TGraphErrors();
		responseGraphsPoint3GsList->Add(gResponse200mVPoint3Gs);
		TGraphErrors *gResponse400mVPoint3Gs = new TGraphErrors();
		responseGraphsPoint3GsList->Add(gResponse400mVPoint3Gs);
		TGraphErrors *gResponse600mVPoint3Gs = new TGraphErrors();
		responseGraphsPoint3GsList->Add(gResponse600mVPoint3Gs);
		
		TList *folderNameListPoint3Gs = new TList();
		folderNameListPoint3Gs->Add(objs200mV);
		folderNameListPoint3Gs->Add(objs400mV);
		folderNameListPoint3Gs->Add(objs600mV);
		
		path = "Linearity/Channel 5/0.3Gs/Response/";
		
		for(Int_t i = 0; i < responseGraphsPoint3GsList->GetEntries();i++) {
			vx.clear(); vy.clear(); vex.clear(); vey.clear();
			nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

			nx = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameListPoint3Gs->At(i))->GetString().Data() + "/DT");
			ny = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameListPoint3Gs->At(i))->GetString().Data() + "/V_output");
			nex = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameListPoint3Gs->At(i))->GetString().Data() + "/DT_errors");
			ney = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameListPoint3Gs->At(i))->GetString().Data() + "/V_output_errors");

			nx->SetBranchAddress("values",&px); ny->SetBranchAddress("values",&py); nex->SetBranchAddress("values",&pex); ney->SetBranchAddress("values",&pey);

			graphPoints = nx->GetEntries();

			currentGraph = ((TGraphErrors*)responseGraphsPoint3GsList->At(i));

			currentGraph->Set(graphPoints);

			for(Int_t n = 0; n < graphPoints; n++) {
				nx->GetEntry(n); vx.push_back(px);
				ny->GetEntry(n); vy.push_back(py);
				nex->GetEntry(n); vex.push_back(pex);
				ney->GetEntry(n); vey.push_back(pey);
				currentGraph->SetPoint(n,vx.at(n),vy.at(n));
				currentGraph->SetPointError(n,vex.at(n),vey.at(n));
			}

			currentGraph->SetTitle("Amplitude of the output signal in function of the threshold");
			currentGraph->SetMarkerStyle(20);
			currentGraph->SetMarkerSize(0.8);
			currentGraph->GetXaxis()->SetTitle("Threshold (mV)");
			currentGraph->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
			
			TString title = "Amplitude of the output signal in function of the threshold | ";
			title += ((TObjString*)folderNameListPoint3Gs->At(i))->GetString().Data();
			
			currentGraph->SetTitle(title);
			currentGraph->SetMarkerStyle(20);
			currentGraph->SetMarkerSize(0.8);
			currentGraph->GetXaxis()->SetTitle("Threshold (mV)");
			currentGraph->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
		}
		
		TMultiGraph *gResponsePoint3Gs = new TMultiGraph("gResponsePoint3Gs", "Collection of graphs for the 0.3Gs/s pulse");
		gResponsePoint3Gs->Add(gResponse200mVPoint3Gs);
		gResponsePoint3Gs->Add(gResponse400mVPoint3Gs);
		gResponsePoint3Gs->Add(gResponse600mVPoint3Gs);

		TCanvas *cResponsePoint3Gs = new TCanvas("cResponsePoint3Gs", "Threshold vs Output all mV 0.3Gs/s", 800,600);
		gResponsePoint3Gs->Draw("APEL");

		cResponsePoint3Gs->Update();
		gResponsePoint3Gs->GetXaxis()->SetTitle("Threshold (mV)");
		gResponsePoint3Gs->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
		cResponsePoint3Gs->Modified();
		
		const int nResponseCh5Point3Gs = 3;

		Float_t DTcriticPoint3Gs[nResponseCh5Point3Gs];
		Float_t VoutputMaxMeanPoint3Gs[nResponseCh5Point3Gs];
		Float_t VoutputMaxMeanErrPoint3Gs[nResponseCh5Point3Gs];
		Float_t VoutputMinMeanPoint3Gs[nResponseCh5Point3Gs];
		Float_t VoutputMinMeanErrPoint3Gs[nResponseCh5Point3Gs];
		Float_t VoutputMeanPoint3Gs[nResponseCh5Point3Gs];
		Float_t VoutputMeanErrPoint3Gs[nResponseCh5Point3Gs];
		
		for(Int_t i = 0; i < nResponseCh5Point3Gs; i++) {
			currentGraph = ((TGraphErrors*)responseGraphsPoint3GsList->At(i));

			x = currentGraph->GetX();

			currentGraph->Fit("pol0","Q0","",x[0],x[4]);

			VoutputMaxMeanPoint3Gs[i] = currentGraph->GetFunction("pol0")->GetParameter(0);
			VoutputMaxMeanErrPoint3Gs[i] = currentGraph->GetFunction("pol0")->GetParError(0);

			graphPoints = currentGraph->GetN();

			currentGraph->Fit("pol0","Q0","",x[graphPoints - 2],x[graphPoints - 1]);

			VoutputMinMeanPoint3Gs[i] = currentGraph->GetFunction("pol0")->GetParameter(0);
			VoutputMinMeanErrPoint3Gs[i] = currentGraph->GetFunction("pol0")->GetParError(0);
			VoutputMeanPoint3Gs[i] = (VoutputMaxMeanPoint3Gs[i] + VoutputMinMeanPoint3Gs[i])/2;
			VoutputMeanErrPoint3Gs[i] = TMath::Sqrt(TMath::Power(VoutputMaxMeanErrPoint3Gs[i],2) + TMath::Power(VoutputMinMeanErrPoint3Gs[i],2));
			
			DTcriticPoint3Gs[i] = Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMeanPoint3Gs[i]);
		}

		for(Int_t i = 1; i < nResponseCh5Point3Gs + 1; i++) {
			cout << "DT critic 0.3 Gs/s " << i*200 << "mV = " << DTcriticPoint3Gs[i - 1] << endl;
		}
		
		// V input 0.3 Gs
		
		path = "Linearity/Channel 5/0.3Gs/Response/V input/";

		vx.clear(); vy.clear(); vex.clear(); vey.clear();
		nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

		nx = (TNtuple*)dataSet->FindObjectAny(path + "V_input_shaper");
		ny = (TNtuple*)dataSet->FindObjectAny(path + "V_nominal_input");
		nex = (TNtuple*)dataSet->FindObjectAny(path + "V_input_shaper_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + "V_nominal_input_errors");

		nx->SetBranchAddress("values",&px); ny->SetBranchAddress("values",&py); nex->SetBranchAddress("values",&pex); ney->SetBranchAddress("values",&pey);

		for(Int_t n = 0; n < graphPoints; n++) {
			nx->GetEntry(n); vx.push_back(px);
			ny->GetEntry(n); vy.push_back(py);
			nex->GetEntry(n); vex.push_back(pex);
			ney->GetEntry(n); vey.push_back(pey);
		}

		Float_t DTcriticErrPPoint3Gs[nResponseCh5Point3Gs];
		Float_t DTcriticErrNPoint3Gs[nResponseCh5Point3Gs];

		// compute the errors of the critical DT

		for(Int_t i = 0; i < nResponseCh5Point3Gs; i++) {
			currentGraph = ((TGraphErrors*)responseGraphsPoint3GsList->At(i));
			x = currentGraph->GetX();
			graphPoints = currentGraph->GetN();
			DTcriticErrPPoint3Gs[i] = DTcriticPoint3Gs[i] - Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMeanPoint3Gs[i] + VoutputMeanErrPoint3Gs[i]);
			DTcriticErrNPoint3Gs[i] = fabs(DTcriticPoint3Gs[i] - Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMeanPoint3Gs[i] - VoutputMeanErrPoint3Gs[i]));
		}

		for(Int_t i = 0; i < nResponseCh5Point3Gs; ++i)
		{
			cout << "Critcal DT: " << DTcriticPoint3Gs[i] << " Positive error: " << DTcriticErrPPoint3Gs[i] << endl
				<< " Negative error: " << DTcriticErrNPoint3Gs[i] << " Difference between those errors: " << DTcriticErrPPoint3Gs[i] - DTcriticErrNPoint3Gs[i] << endl;
		}

		// The Positive Error is used because the difference between the positive and the negative is very small.

		TGraphErrors *gLinearityNPoint3Gs = new TGraphErrors(nResponseCh5Point3Gs, DTcriticPoint3Gs, &vy.at(0),DTcriticErrPPoint3Gs,&vey.at(0));
		gLinearityNPoint3Gs->SetMarkerStyle(20);
		gLinearityNPoint3Gs->SetMarkerSize(0.8);
		gLinearityNPoint3Gs->GetXaxis()->SetTitle("Critical DT (mV)");
		gLinearityNPoint3Gs->GetYaxis()->SetTitle("Nominal amplitude of the input signal (mV)");
		gLinearityNPoint3Gs->Fit("pol1", "");
		gLinearityNPoint3Gs->GetFunction("pol1")->SetLineWidth(1);

		TCanvas *cLinearityNPoint3Gs = new TCanvas("cLinearityNPoint3Gs", "Critical DT vs Nominal Input Amplitude 0.3 Gs/s", 800,600);
		cLinearityNPoint3Gs->SetGrid();
		gLinearityNPoint3Gs->Draw("APE");

		// The following graph shows the actual input amplitude after the shaper (measured with the oscilloscope) in function of
		// the critical threshold

		TGraphErrors *gLinearitySPoint3Gs = new TGraphErrors(nResponseCh5Point3Gs, DTcriticPoint3Gs, &vx.at(0),DTcriticErrPPoint3Gs,&vex.at(0));
		gLinearitySPoint3Gs->SetMarkerStyle(21);
		gLinearitySPoint3Gs->SetMarkerSize(0.8);
		gLinearitySPoint3Gs->SetMarkerColor(4);
		gLinearitySPoint3Gs->GetXaxis()->SetTitle("Critical DT (mV)");
		gLinearitySPoint3Gs->GetYaxis()->SetTitle("Amplitude of the input signal after the shaper (mV)");
		gLinearitySPoint3Gs->Fit("pol1", "");
		gLinearitySPoint3Gs->GetFunction("pol1")->SetLineWidth(1);
		gLinearitySPoint3Gs->GetFunction("pol1")->SetLineColor(2);

		TCanvas *cLinearitySPoint3Gs = new TCanvas("cLinearitySPoint3Gs", "Critical DT vs Input Amplitude after the shaper 0.3 Gs/s", 800,600);
		cLinearitySPoint3Gs->SetGrid();
		gLinearitySPoint3Gs->Draw("APE");
		
		// comparison between the results obtained with the 0.3Gs and the 1Gs signals.

		TMultiGraph *gLinearityS = new TMultiGraph("gLinearityS", "Collection of graphs for both 1Gs/s and 0.3Gs/s pulse");
		gLinearityS->Add(gLinearityS1Gs);
		gLinearityS->Add(gLinearitySPoint3Gs);

		TCanvas *cLinearityS = new TCanvas("cLinearityS", "Critical DT vs Input Amplitude after the shaper for 0.3 Gs/s and 1Gs/s pulses",800,600);
		cLinearityS->SetGrid();
		gLinearityS->Draw("APE");
		cLinearityS->Update();
		gLinearityS->GetXaxis()->SetTitle("Threshold (mV)");
		gLinearityS->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
		cLinearityS->Modified();

		break;

		case 6 :
		
		// Channel 6

		// These graphs show the V max output amplitude in function of the threshold.
		// The input signal was set at different values between 100 and 800 mV.
		
		cout << dataSet << endl;
		
		TList *responseGraphs1GsList = new TList();
		TGraphErrors *gResponse100mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse100mV1Gs);
		TGraphErrors *gResponse200mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse200mV1Gs);
		TGraphErrors *gResponse300mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse300mV1Gs);
		TGraphErrors *gResponse400mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse400mV1Gs);
		TGraphErrors *gResponse500mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse500mV1Gs);
		TGraphErrors *gResponse600mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse600mV1Gs);
		TGraphErrors *gResponse700mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse700mV1Gs);
		TGraphErrors *gResponse800mV1Gs = new TGraphErrors();
		responseGraphs1GsList->Add(gResponse800mV1Gs);

		TList *folderNameList = new TList();
		TObjString *objs100mV = new TObjString("100mV");
		folderNameList->Add(objs100mV);
		TObjString *objs200mV = new TObjString("200mV");
		folderNameList->Add(objs200mV);
		TObjString *objs300mV = new TObjString("300mV");
		folderNameList->Add(objs300mV);
		TObjString *objs400mV = new TObjString("400mV");
		folderNameList->Add(objs400mV);
		TObjString *objs500mV = new TObjString("500mV");
		folderNameList->Add(objs500mV);
		TObjString *objs600mV = new TObjString("600mV");
		folderNameList->Add(objs600mV);
		TObjString *objs700mV = new TObjString("700mV");
		folderNameList->Add(objs700mV);
		TObjString *objs800mV = new TObjString("800mV");
		folderNameList->Add(objs800mV);

		TString path = "Linearity/Channel 6/Response/";

		TGraphErrors *currentGraph;
		Double_t *x;

		for(Int_t i = 0; i < responseGraphs1GsList->GetEntries();i++) {

			nx = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/DT");
			ny = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/V_output");
			nex = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/DT_errors");
			ney = (TNtuple*)dataSet->FindObjectAny(path + ((TObjString*)folderNameList->At(i))->GetString().Data() + "/V_output_errors");

			nx->SetBranchAddress("values",&px); ny->SetBranchAddress("values",&py); nex->SetBranchAddress("values",&pex); ney->SetBranchAddress("values",&pey);

			graphPoints = nx->GetEntries();

			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));

			currentGraph->Set(graphPoints);

			for(Int_t n = 0; n < graphPoints; n++) {
				nx->GetEntry(n); vx.push_back(px);
				ny->GetEntry(n); vy.push_back(py);
				nex->GetEntry(n); vex.push_back(pex);
				ney->GetEntry(n); vey.push_back(pey);
				currentGraph->SetPoint(n,vx.at(n),vy.at(n));
				currentGraph->SetPointError(n,vex.at(n),vey.at(n));
			}

			currentGraph->SetTitle("Amplitude of the output signal in function of the threshold");
			currentGraph->SetMarkerStyle(20);
			currentGraph->SetMarkerSize(0.8);
			currentGraph->GetXaxis()->SetTitle("Threshold (mV)");
			currentGraph->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
			
			vx.clear(); vy.clear(); vex.clear(); vey.clear();
			nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

		}

		TMultiGraph *gResponse1Gs = new TMultiGraph("gResponse1Gs", "Collection of graphs for the 1Gs/s pulse");

		for(Int_t i = 0; i < responseGraphs1GsList->GetEntries();i++) {
			gResponse1Gs->Add((TGraphErrors*)responseGraphs1GsList->At(i));
		}

		TCanvas *cResponse1Gs = new TCanvas("cResponse1Gs", "Threshold vs Output all mV 1Gs/s", 800,600);
		gResponse1Gs->Draw("APEL");

		cResponse1Gs->Update();
		gResponse1Gs->GetXaxis()->SetTitle("Threshold (mV)");
		gResponse1Gs->GetYaxis()->SetTitle("Amplitude of the output signal (mV)");
		cResponse1Gs->Modified();

		// Graph of the input amplitude in function of the "critical" threshold.
		// "critical" means the threshold at which the output amplitude reaches the mean value between the max and the min output amplitudes.
		// To find the critical threshold the bisection method is used.
		// The equation to solve is:
		// "f(x) - mean = 0" where f(x) is the TSpline generated by root to interpolate lineary the graph points.
		// Error for the bisection method is set to 0.0001
		// The graph expected is a straight line

		const int nResponseCh6 = 8;

		Float_t DTcritic[nResponseCh6];
		Float_t VoutputMaxMean[nResponseCh6];
		Float_t VoutputMaxMeanErr[nResponseCh6];
		Float_t VoutputMinMean[nResponseCh6];
		Float_t VoutputMinMeanErr[nResponseCh6];
		Float_t VoutputMean[nResponseCh6];
		Float_t VoutputMeanErr[nResponseCh6]; 

		for(Int_t i = 0; i < nResponseCh6; i++) {
			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));

			x = currentGraph->GetX();

			currentGraph->Fit("pol0","Q0","",x[0],x[2]);

			VoutputMaxMean[i] = currentGraph->GetFunction("pol0")->GetParameter(0);
			VoutputMaxMeanErr[i] = currentGraph->GetFunction("pol0")->GetParError(0);

			graphPoints = currentGraph->GetN();

			currentGraph->Fit("pol0","Q0","",x[graphPoints - 2],x[graphPoints - 1]);

			VoutputMinMean[i] = currentGraph->GetFunction("pol0")->GetParameter(0);
			VoutputMinMeanErr[i] = currentGraph->GetFunction("pol0")->GetParError(0);
			VoutputMean[i] = (VoutputMaxMean[i] + VoutputMinMean[i])/2;
			VoutputMeanErr[i] = TMath::Sqrt(TMath::Power(VoutputMaxMeanErr[i],2) + TMath::Power(VoutputMinMeanErr[i],2));
		}

		for(Int_t i = 0; i < nResponseCh6; i++) {
			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));
			x = currentGraph->GetX();
			graphPoints = currentGraph->GetN();

			DTcritic[i] = Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMean[i]);
		}

		for(Int_t i = 1; i < nResponseCh6 + 1; i++) {
			cout << "DT critic " << i*100 << "mV = " << DTcritic[i - 1] << endl;
		}

		// Fetch data regarding V input after the shaper and V nominal imput given by the generator

		path = "Linearity/Channel 6/Response/V input/";

		vx.clear(); vy.clear(); vex.clear(); vey.clear();
		nx->ResetBranchAddresses(); ny->ResetBranchAddresses(); nex->ResetBranchAddresses(); ney->ResetBranchAddresses();

		nx = (TNtuple*)dataSet->FindObjectAny(path + "V_input_shaper");
		ny = (TNtuple*)dataSet->FindObjectAny(path + "V_nominal_input");
		nex = (TNtuple*)dataSet->FindObjectAny(path + "V_input_shaper_errors");
		ney = (TNtuple*)dataSet->FindObjectAny(path + "V_nominal_input_errors");

		nx->SetBranchAddress("values",&px); ny->SetBranchAddress("values",&py); nex->SetBranchAddress("values",&pex); ney->SetBranchAddress("values",&pey);

		for(Int_t n = 0; n < graphPoints; n++) {
			nx->GetEntry(n); vx.push_back(px);
			ny->GetEntry(n); vy.push_back(py);
			nex->GetEntry(n); vex.push_back(pex);
			ney->GetEntry(n); vey.push_back(pey);
		}

		Float_t DTcriticErrP[nResponseCh6];
		Float_t DTcriticErrN[nResponseCh6];

		// compute the errors of the critical DT

		for(Int_t i = 0; i < nResponseCh6; i++) {
			currentGraph = ((TGraphErrors*)responseGraphs1GsList->At(i));
			x = currentGraph->GetX();
			graphPoints = currentGraph->GetN();
			DTcriticErrP[i] = DTcritic[i] - Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMean[i] + VoutputMeanErr[i]);
			DTcriticErrN[i] = fabs(DTcritic[i] - Bisection((Float_t)x[0],(Float_t)x[graphPoints - 1],0.0001,currentGraph,VoutputMean[i] - VoutputMeanErr[i]));
		}

		for(Int_t i = 0; i < nResponseCh6; ++i)
		{
			cout << "Critcal DT: " << DTcritic[i] << " Positive error: " << DTcriticErrP[i] << endl
				<< " Negative error: " << DTcriticErrN[i] << " Difference between those errors: " << DTcriticErrP[i] - DTcriticErrN[i] << endl;
		} 

		// The Positive Error is used because the difference between the positive and the negative is very small.

		TGraphErrors *gLinearityN1Gs = new TGraphErrors(nResponseCh6, DTcritic, &vy.at(0),DTcriticErrP,&vey.at(0));
		gLinearityN1Gs->SetMarkerStyle(20);
		gLinearityN1Gs->SetMarkerSize(0.8);
		gLinearityN1Gs->SetTitle("Critical DT vs Nominal Input Amplitude");
		gLinearityN1Gs->GetXaxis()->SetTitle("Critical DT (mV)");
		gLinearityN1Gs->GetYaxis()->SetTitle("Nominal amplitude of the input signal (mV)");
		gLinearityN1Gs->Fit("pol1", "Q");
		gLinearityN1Gs->GetFunction("pol1")->SetLineWidth(1);

		TCanvas *cLinearityN1Gs = new TCanvas("cLinearityN1Gs", "Critical DT vs Nominal Input Amplitude", 800,600);
		cLinearityN1Gs->SetGrid();
		gLinearityN1Gs->Draw("APE");

		// The following graph shows the actual input amplitude after the shaper (measured with the oscilloscope) in function of
		// the critical threshold

		TGraphErrors *gLinearityS1Gs = new TGraphErrors(8, DTcritic, &vx.at(0),DTcriticErrP,&vex.at(0));
		gLinearityS1Gs->SetMarkerStyle(20);
		gLinearityS1Gs->SetMarkerSize(0.8);
		gLinearityS1Gs->SetTitle("Critical DT vs Input Amplitude after the shaper");
		gLinearityS1Gs->GetXaxis()->SetTitle("Critical DT (mV)");
		gLinearityS1Gs->GetYaxis()->SetTitle("Amplitude of the input signal after the shaper (mV)");
		gLinearityS1Gs->Fit("pol1", "");
		gLinearityS1Gs->GetFunction("pol1")->SetLineWidth(1);

		TCanvas *cLinearityS1Gs = new TCanvas("cLinearityS1Gs", "Critical DT vs Input Amplitude after the shaper", 800,600);
		cLinearityS1Gs->SetGrid();
		gLinearityS1Gs->Draw("APE");

		break;
	}

	f->Close();
} 
