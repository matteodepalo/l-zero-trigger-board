// -----------------------
//  Level 0 Trigger Board Project
//  analyze waveforms saved with the oscilloscope
//
//  Input:
//   - waveforms file
//  Output:
//   -  plots regarding with waveform analysis
//
//   Structure:
//   1. read waveforms file
//   2. fill trees with data
//   3. waveforms analysis
//
//
// m.depalo jul.2010
//

// You must first run the WaveformsFileMaker.C macro to generate the Waveforms.root file

// This is the only file where i'm not using the Data.xml data because there are few vectors to use

#include <vector>

void AnalyzeWaveforms(char *WaveformsFile = "Waveforms.root", const int nAddedChannels = 5) {
	
	//try to access waveforms file and in case of failure return
	if(gSystem->AccessPathName(WaveformsFile,kFileExists)) {
		cout << "Error: file " << WaveformsFile << " does not exsist. Run .x WaveformsFileMaker.C to create it" << endl;
		return;
	}
	//	gStyle->SetOptFit(111);
	//	gStyle->SetStatFormat("1.3E");
	//	gStyle->SetFitFormat("1.3E");
	
	// fetch the list of trees contained in the waveforms file
	// for every tree generate a waveform graph

	TFile *f = TFile::Open(WaveformsFile);
	TList *listOfKeys = f->GetListOfKeys();
	Int_t numberOfKeys = listOfKeys->GetEntries();
	TList *listOfGraphs = new TList();
	
	// if the waveform file name begins with the string "comparator" it goes in this list
	TList *listOfCompWaves = new TList();
	// if the waveform file name begins with the string "sum output" it goes in this list
	TList *listOfAdderWaves = new TList();

	for(Int_t i = 0; i < numberOfKeys; i++) {
		TString *keyName = new TString(listOfKeys->At(i)->GetName());
		TTree *tree = (TTree*)f->Get(keyName->Data());
		Float_t x = 0;
		Float_t y = 0;
		tree->SetBranchAddress("x",&x);
		tree->SetBranchAddress("y",&y);
		Int_t nentries = tree->GetEntries();

		TString *gName = new TString(keyName->Data());
		gName->Append(" graph");
		TGraphErrors *gWave = new TGraphErrors(nentries);
		gWave->SetName(gName->Data());
		gWave->SetTitle(gName->Data());
		gWave->GetXaxis()->SetTitle("Time");
		gWave->GetYaxis()->SetTitle("Voltage");
	//	gWave->SetBit(TH1::kCanRebin);

		for (Int_t j = 0; j < nentries; j++) {
			tree->GetEntry(j);
			gWave->SetPoint(j,x,y);
		}

		listOfGraphs->Add(gWave);

		if(keyName->BeginsWith("comparator"))
			listOfCompWaves->Add(gWave);

		if(keyName->BeginsWith("sum output"))
			listOfAdderWaves->Add(gWave);

	/*	TString *cName = new TString(keyName->Data());
		cName->Append(" canvas");
		TCanvas *cy = new TCanvas(cName->Data(),cName->Data(),800,600);
		gWave->Draw("AL"); */
	}
	
	cout << listOfAdderWaves->GetEntries() << endl;

	// analysis for waves with no delay

	// global variables

	Double_t xMin,xMax,yStart,yEnd;
	Int_t graphPoints;
	Double_t step;

	// comparator outputs waves sum

	TGraphErrors *gFirstCompWave = (TGraphErrors *)listOfCompWaves->First();

	graphPoints = gFirstCompWave->GetN();
	gFirstCompWave->GetPoint(0,xMin,yStart);
	gFirstCompWave->GetPoint(graphPoints - 1,xMax,yEnd); 

	step = (xMax - xMin)/graphPoints;
	cout << gFirstCompWave->GetName() << endl;
	cout << "xMin = " << xMin << "  xMax = " << xMax << "  graphPoints = " << graphPoints << endl;

	TGraphErrors *gCompSum = new TGraphErrors(graphPoints);
	gCompSum->SetLineColor(kBlue);
	gCompSum->SetLineWidth(2);
	gCompSum->SetName("Comparator Outputs Sum");
	gCompSum->SetTitle("Comparator Outputs Sum");
	Int_t nCompWaves = listOfCompWaves->GetEntries();
	Float_t gx,gy = 0;
	
	// Alpha coefficiens are now written "hard coded" here
	Float_t alphaArray[3] = {0.199,0.201,0.197};
	
	// Deleays coming from the multiplexer are written "hard coded" here
	Float_t muxDelayArray[3] = {0,77.14E-12,192.01E-12};

	for(Int_t i = 0; i < graphPoints; i++) {
		for(Int_t j = 0; j < nCompWaves; j++) {
			TGraphErrors *gCompWave = (TGraphErrors *)listOfCompWaves->At(j);
			gy += (gCompWave->Eval(xMin + i*step + muxDelayArray[j]))*alphaArray[j];
		}
		gCompSum->SetPoint(i,xMin + i*step,gy);
		gy = 0;
	}
	
	// note that there is a manual correction on the x axis for the comparator waves sum to compare them better in the multigraph
	
	Double_t *xArray = gCompSum->GetX();
	for(Int_t i = 0; i < graphPoints; i++) {
		xArray[i] += 5.6E-9;
	}

//	TCanvas *cCompSum = new TCanvas("cCompSum","Comparator Outputs Sum",800,600);
//	gCompSum->Draw("AL");

	// adder outputs waves sum
	
	// THE ANALYSIS FOR THE ADDER OUTPUT WITH NOT DELAY AND 3 CHANNELS ADDED IS DEPRECATED.
	// THERE WAS A MISTAKE IN THE DATA TAKING
	// I'LL KEEP THE CODE HERE FOR FURTHER REFERENCE

/*
	TGraphErrors *gFirstAdderWave = (TGraphErrors *)listOfAdderWaves->First();

	graphPoints = gFirstAdderWave->GetN();
	gFirstAdderWave->GetPoint(0,xMin,yStart);
	gFirstAdderWave->GetPoint(graphPoints - 1,xMax,yEnd);

	step = (xMax - xMin)/graphPoints;
	cout << gFirstAdderWave->GetName() << endl;
	cout << "xMin : " << xMin << "  xMax : " << xMax << endl;

	TGraphErrors *gAdderSum = new TGraphErrors(graphPoints);
	gAdderSum->SetLineWidth(2);
//	gAdderSum->SetLineColor(kGreen);
	gAdderSum->SetLineStyle(9);
	gAdderSum->SetName("Sum of the adder outputs with no delay");
	gAdderSum->SetTitle("Sum of the adder outputs with no delay");
	Int_t nAdderWaves = listOfAdderWaves->GetEntries();
	gy = 0;

	for(Int_t i = 0; i < graphPoints; i++) {
		for(Int_t j = 0; j < nAdderWaves; j++) {
			TGraphErrors *gAdderWave = (TGraphErrors *)listOfAdderWaves->At(j);
			gy += gAdderWave->Eval(xMin + i*step);
		} 
		gAdderSum->SetPoint(i,xMin + i*step,gy);
		gy = 0;
	}

//	TCanvas *cAdderSum = new TCanvas("cAdderSum","Sum of the adder outputs",800,600);
//	gAdderSum->Draw("AL"); 
	
	TGraphErrors *g3ChannelsSumNoDelay = listOfGraphs->FindObject("sum_output_3_channels_(3) graph");
	g3ChannelsSumNoDelay->SetLineColor(kRed);
	g3ChannelsSumNoDelay->SetLineWidth(2);
	
	// note that there is a manual correction on the x axis for the adder output to compare it better in the multigraph
	
	Double_t *xArray = g3ChannelsSumNoDelay->GetX();
	graphPoints = g3ChannelsSumNoDelay->GetN();
	
	for(Int_t i = 0; i < graphPoints; i++) {
		xArray[i] -= 600E-12;
	}
	
	// comparison among the computed adder output ,the real one and the sum of the comparator outputs with alpha coefficients correction
	
	TMultiGraph *mgSumNoDelay = new TMultiGraph();
	mgSumNoDelay->Add(g3ChannelsSumNoDelay);
	mgSumNoDelay->Add(gAdderSum);
	mgSumNoDelay->Add(gCompSum);
	mgSumNoDelay->SetTitle("Collection of graphs for 3 channels sum with no delay");
	
	TCanvas *cmgSumNoDelay = new TCanvas("cmgSumNoDelay", "Collection of graphs for 3 channels sum with no delay", 1200,800);
	cmgSumNoDelay->Update();
	mgSumNoDelay->Draw("AL");
	
	legend = new TLegend(0.6,0.78,0.99,0.99);
	legend->AddEntry(g3ChannelsSumNoDelay, "Actual sum done by the adder", "lp");
	legend->AddEntry(gAdderSum, "Sum of the adder outputs", "lp");
	legend->AddEntry(gCompSum, "Sum of the comparator outputs","lp");
	legend->Draw();
	
	mgSumNoDelay->GetXaxis()->SetTitle("Seconds");
	mgSumNoDelay->GetYaxis()->SetTitle("Volts");
	TPaveText *title = (TPaveText*)cmgSumNoDelay->GetPrimitive("title");
	title->SetX1NDC(0.009);
	title->SetY1NDC(0.94);
	title->SetX2NDC(0.56);
	title->SetY2NDC(0.99);
	cmgSumNoDelay->Modified();
*/
	
	
	// analysis for delayed adder outputs
	
	Int_t nAdderWaves = listOfAdderWaves->GetEntries();

	TGraphErrors *g3ChannelsSumDelay = listOfGraphs->FindObject("sum_output_3_channels_(1-1-1) graph");
	g3ChannelsSumDelay->SetLineColor(kRed);
	g3ChannelsSumDelay->SetLineWidth(2);

	// Delay coming from cables are written "hard coded" here
	Float_t cablesDelayArray[3] = {0,1.45E-9,3.21E-9};
	
	graphPoints = g3ChannelsSumDelay->GetN();
	//g3ChannelsSumDelay->GetPoint(0,xMin,yStart);
	//g3ChannelsSumDelay->GetPoint(graphPoints - 1,xMax,yEnd);
	xMin = 6E-9;
	xMax = 16E-9;
	step = (xMax - xMin)/graphPoints;
	cout << "Sum of the adder outputs with delay" << endl;
	cout << "xMin : " << xMin << "  xMax : " << xMax << endl;

	TGraphErrors *gAdderSumDelay = new TGraphErrors(graphPoints);
	gAdderSumDelay->SetLineColor(kBlue);
	gAdderSumDelay->SetLineWidth(2);
	gAdderSumDelay->SetName("Sum of the adder outputs with delay");
	gAdderSumDelay->SetTitle("Sum of the adder outputs with delay");

	gy = 0;
	
	// note that there is a manual correction on the delay of 500E-12 to compare the adder output better in the multigraph

	for(Int_t i = 0; i < graphPoints; i++) {
		for(Int_t j = 0; j < nAdderWaves; j++) {
			TGraphErrors *gAdderWave = (TGraphErrors *)listOfAdderWaves->At(j);
			gy += gAdderWave->Eval(xMin + i*step + cablesDelayArray[j] - 500E-12);
		} 
		gAdderSumDelay->SetPoint(i,xMin + i*step,gy);
		gy = 0;
	}

	//	TCanvas *cSumSumDelay = new TCanvas("cSumSumDelay","Sum of the sum outputs with delay",800,600);
	//	gAdderSumDelay->Draw("APEL");

	TMultiGraph *mg3ChannelsSumDelay = new TMultiGraph();
	mg3ChannelsSumDelay->Add(gAdderSumDelay);
	mg3ChannelsSumDelay->Add(g3ChannelsSumDelay);
	mg3ChannelsSumDelay->SetTitle("Collection of graphs for 3 channels sum with delay");

	TCanvas *cmg3ChannelsSumDelay = new TCanvas("cmg3ChannelsSumDelay", "Collection of graphs for 3 channels sum with delay", 1200, 800);
	cmg3ChannelsSumDelay->Update();
	
	legend = new TLegend(0.6,0.78,0.99,0.99);
	legend->AddEntry(g3ChannelsSumDelay, "Actual sum done by the adder", "lp");
	legend->AddEntry(gAdderSumDelay, "Sum of the delayed adder outputs", "lp");
	mg3ChannelsSumDelay->Draw("AL");
	legend->Draw();
	
	mg3ChannelsSumDelay->GetXaxis()->SetTitle("Seconds");
	mg3ChannelsSumDelay->GetYaxis()->SetTitle("Volts");
	
	TPaveText *title = (TPaveText*)cmg3ChannelsSumDelay->GetPrimitive("title");
	title->SetX1NDC(0.009);
	title->SetY1NDC(0.94);
	title->SetX2NDC(0.56);
	title->SetY2NDC(0.99);
	
	cmg3ChannelsSumDelay->Modified();

/*
	// delay test just to try a different method

	TMultiGraph *mgDelayTest = new TMultiGraph();
	Double_t gxTest,gyTest;

	TGraphErrors *gDelayTest0 = new TGraphErrors(((TGraphErrors *)listOfAdderWaves->At(0))->GetN());
	for(Int_t j = 0; j < gDelayTest0->GetN(); j++) {
		((TGraphErrors *)listOfAdderWaves->At(0))->GetPoint(j,gxTest,gyTest);
		gDelayTest0->SetPoint(j,gxTest - cablesDelayArray[0],gyTest);
	}
	mgDelayTest->Add(gDelayTest0);

	TGraphErrors *gDelayTest1 = new TGraphErrors(((TGraphErrors *)listOfAdderWaves->At(1))->GetN());
	for(Int_t j = 0; j < gDelayTest1->GetN(); j++) {
		((TGraphErrors *)listOfAdderWaves->At(1))->GetPoint(j,gxTest,gyTest);
		gDelayTest1->SetPoint(j,gxTest - cablesDelayArray[1],gyTest);
	}
	mgDelayTest->Add(gDelayTest1);

	TGraphErrors *gDelayTest2 = new TGraphErrors(((TGraphErrors *)listOfAdderWaves->At(2))->GetN());
	for(Int_t j = 0; j < gDelayTest2->GetN(); j++) {
		((TGraphErrors *)listOfAdderWaves->At(2))->GetPoint(j,gxTest,gyTest);
		gDelayTest2->SetPoint(j,gxTest - cablesDelayArray[2],gyTest);
	}
	mgDelayTest->Add(gDelayTest2);

//	TCanvas *cmgDelayTest = new TCanvas("cmgDelayTest", "cmgDelayTest", 800,600);
//	mgDelayTest->Draw("APEL");

	graphPoints = 12000;
	xMin = 2E-9;
	xMax = 22E-9;
	step = (xMax - xMin)/graphPoints;
	cout << "xMin : " << xMin << "  xMax : " << xMax << endl;

	Double_t delayTest0xMin,delayTest0xMax,delayTest1xMin,delayTest1xMax,delayTest2xMin,delayTest2xMax;
	gDelayTest0->GetPoint(0,delayTest0xMin,gyTest);
	gDelayTest0->GetPoint(gDelayTest0->GetN() - 1,delayTest0xMax,gyTest);
	gDelayTest1->GetPoint(0,delayTest1xMin,gyTest);
	gDelayTest1->GetPoint(gDelayTest1->GetN() - 1,delayTest1xMax,gyTest);
	gDelayTest2->GetPoint(0,delayTest2xMin,gyTest);
	gDelayTest2->GetPoint(gDelayTest2->GetN() - 1,delayTest2xMax,gyTest);

	cout << delayTest0xMin << endl;

	TGraphErrors *gAdderSumDelayV2 = new TGraphErrors(graphPoints);
	for(Int_t i = 0; i < graphPoints; i++) {
		gyTest = gDelayTest0->Eval(xMin + i*step);
		gyTest += gDelayTest1->Eval(xMin + i*step);
		gyTest += gDelayTest2->Eval(xMin + i*step);
		gAdderSumDelayV2->SetPoint(i,xMin + i*step+ 500E-12,gyTest);
	} 
	
	gAdderSumDelayV2->SetLineColor(kGreen);
	gAdderSumDelayV2->SetLineWidth(2);
	mg3ChannelsSumDelay->Add(gAdderSumDelayV2);
	c3ChannelsSumDelay->Modified();

	TCanvas *cAdderSumDelayV2 = new TCanvas("cAdderSumDelayV2","cAdderSumDelayV2",800,600);
	gAdderSumDelayV2->Draw("APL");
*/

	// List of waves representing the adder output with 5,4,3,2,1 channels in input

	TGraphErrors *gAdder5Channels = listOfGraphs->FindObject("adder output 5 channels v2 graph");
	TGraphErrors *gAdder4Channels = listOfGraphs->FindObject("adder output 4 channels v2 graph");
	TGraphErrors *gAdder3Channels = listOfGraphs->FindObject("adder output 3 channels v2 graph");
	TGraphErrors *gAdder2Channels = listOfGraphs->FindObject("adder output 2 channels v2 graph");
	TGraphErrors *gAdder1Channel = listOfGraphs->FindObject("adder output 1 channel v2 graph");
	
	TGraphErrors *gAdderSingleCh0 = listOfGraphs->FindObject("single sum output ch 0 input 500 mV DT 100 mV graph");
	TGraphErrors *gAdderSingleCh1 = listOfGraphs->FindObject("single sum output ch 1 input 500 mV DT 100 mV graph");
	TGraphErrors *gAdderSingleCh4 = listOfGraphs->FindObject("single sum output ch 4 input 500 mV DT 100 mV graph");
	TGraphErrors *gAdderSingleCh5 = listOfGraphs->FindObject("single sum output ch 5 input 500 mV DT 100 mV graph");
	TGraphErrors *gAdderSingleCh7 = listOfGraphs->FindObject("single sum output ch 7 input 500 mV DT 100 mV graph");
	
	TMultiGraph *mgAdder54321Channels = new TMultiGraph();
	mgAdder54321Channels->SetTitle("Collection of graphs for different numbers of added channels");
	mgAdder54321Channels->Add(gAdder5Channels);
	mgAdder54321Channels->Add(gAdder4Channels);
	mgAdder54321Channels->Add(gAdder3Channels);
	mgAdder54321Channels->Add(gAdder2Channels);
	mgAdder54321Channels->Add(gAdder1Channel);
	
	TCanvas *cmgAdder54321Channels = new TCanvas("cmgAdder54321Channels", "Collection of graphs for different numbers of added channels",1200,800);
	mgAdder54321Channels->Draw("AL");
	cmgAdder54321Channels->Update();
	mgAdder54321Channels->GetXaxis()->SetTitle("seconds");
	mgAdder54321Channels->GetYaxis()->SetTitle("Volts");	
	cmgAdder54321Channels->Modified();
	
	// analysis of the linearity of the adder
	
	Double_t addedVMax[nAddedChannels];
	addedVMax[0] = TMath::MaxElement(gAdder1Channel->GetN(), gAdder1Channel->GetY());
	addedVMax[1] = TMath::MaxElement(gAdder2Channels->GetN(), gAdder2Channels->GetY());
	addedVMax[2] = TMath::MaxElement(gAdder3Channels->GetN(), gAdder3Channels->GetY());
	addedVMax[3] = TMath::MaxElement(gAdder4Channels->GetN(), gAdder4Channels->GetY());
	addedVMax[4] = TMath::MaxElement(gAdder5Channels->GetN(), gAdder5Channels->GetY());
	
	Double_t singleVMax[nAddedChannels];
	singleVMax[0] = TMath::MaxElement(gAdderSingleCh0->GetN(), gAdderSingleCh0->GetY());
	singleVMax[1] = TMath::MaxElement(gAdderSingleCh1->GetN(), gAdderSingleCh1->GetY());
	singleVMax[2] = TMath::MaxElement(gAdderSingleCh4->GetN(), gAdderSingleCh4->GetY());
	singleVMax[3] = TMath::MaxElement(gAdderSingleCh5->GetN(), gAdderSingleCh5->GetY());
	singleVMax[4] = TMath::MaxElement(gAdderSingleCh7->GetN(), gAdderSingleCh7->GetY());
	
	vector<double> expectedVMax(nAddedChannels);
	Double_t previousVmax = 0;
	for(Int_t i = 0; i < expectedVMax.size(); i++) {
		expectedVMax[i] = singleVMax[i] + previousVmax;
		previousVmax = expectedVMax[i];
		cout << "singleVMax[" <<i <<"] = " << singleVMax[i] <<  endl;
		cout << "addedVMax[" <<i <<"] = " << addedVMax[i] <<  endl;
		cout << "expectedVMax[" <<i <<"] = " << expectedVMax[i] <<  endl;
	}
	
	TGraph *gAdderLinearity = new TGraph(expectedVMax.size(),&expectedVMax[0],addedVMax);
	gAdderLinearity->SetTitle("Linearity of the adder");
	gAdderLinearity->GetXaxis()->SetTitle("Expected amplitude value (V)");
	gAdderLinearity->GetYaxis()->SetTitle("Actual amplitude value (V)");
	gAdderLinearity->GetYaxis()->SetTitleOffset(1.3);
	
	TCanvas *cgAdderLinearity = new TCanvas("cgAdderLinearity","Linearity of the adder",800,600);
	cgAdderLinearity->SetGrid();
	cgAdderLinearity->Update();
	gAdderLinearity->SetMarkerStyle(20);
	gAdderLinearity->SetMarkerSize(0.8);
	//gAdderLinearity->Fit("pol1","Q+","",expectedVMax[0],expectedVMax[2]);
	TF1 *line = new TF1("line","x",expectedVMax.back(),expectedVMax.front());
	gAdderLinearity->Draw("APEL");
	line->Draw("SAME");
	//gAdderLinearity->GetFunction("pol1")->SetRange(expectedVMax.back(),expectedVMax.front());;
	cgAdderLinearity->SetLeftMargin(0.13);
	cgAdderLinearity->Modified();
	
	f->Close();


}
