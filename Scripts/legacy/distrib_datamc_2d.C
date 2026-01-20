//#ifndef __CINT__
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TLine.h"
#include "TLatex.h"
#include "TPaveStats.h"
#include "CMS_lumi.h"
#include "TString.h"
//#endif
#include <iostream>
using namespace std;

void distrib_datamc_2d() {

	string yr1="2024", era1="I";
	string yr2="2025", era2="C";
	
	gStyle->SetOptStat(0);                                                                                                                   
	string data1 = "/eos/home-r/rforti/trackingPOG/analyses_2024/data/ZeroBias/Run2024I_golden_cmssw-14-0-21/DQM_V0001_R000999999__Global__CMSSW_X_Y_Z__RECO.root";

	string data2 = "/eos/home-r/rforti/trackingPOG/analyses_2025/data/ZeroBias/Run2025C_golden_cmssw-15-0-10-p3/DQM_V0001_R000999999__Global__CMSSW_X_Y_Z__RECO.root";

	string mc1 = "/eos/home-r/rforti/trackingPOG/analyses_2024/MC/ZeroBias/SingleNeutrino_Par-E-10-gun/nvtx_reweighted_2024I_golden/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root"; 
	
	string mc2 = "/eos/home-r/rforti/trackingPOG/analyses_2024/MC/ZeroBias/SingleNeutrino_Par-E-10-gun/nvtx_reweighted_2025C_golden/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root";


	TFile* file1 = TFile::Open(data1.c_str());
        TFile* file2 = TFile::Open(data2.c_str());
        TFile* filemc1 = TFile::Open(mc1.c_str());
	TFile* filemc2 = TFile::Open(mc2.c_str());
	
	//TH2F* histo1=(TH2F*)file1->Get("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/highPurityTracks/nTracks2D")->Clone("histo1");
        TH2F* histo2=(TH2F*)file2->Get("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/highPurityTracks/nTracks2D")->Clone("histo2");
        //TH2F* histomc1=(TH2F*)filemc1->Get("DQMData/Run 1/StandaloneTrackMonitor/Run summary/highPurityTracks/nTracks2D")->Clone("histomc1");
        TH2F* histomc2=(TH2F*)filemc2->Get("DQMData/Run 1/StandaloneTrackMonitor/Run summary/highPurityTracks/nTracks2D")->Clone("histomc2");
        //TH2F* histo3=(TH2F*)histo1->Clone("histo3");
        TH2F* histo4=(TH2F*)histo2->Clone("histo4");
        //TH2F* histo5=(TH2F*)histo1->Clone("histo5");
        //histomc1->Scale(histo1->Integral()/histomc1->Integral());
        histomc2->Scale(histo2->Integral()/histomc2->Integral());
  	
	string title1 = "Data " + yr1 + era1;
	string title2 = "Data " + yr2 + era2;
	string titlemc1 = "MC " + yr1 + era1;
        string titlemc2 = "MC " + yr2 + era2;
	
	string ratio_dd = title1 + " / " + title2;
	string ratio_d1m = title1 + " / MC";
	string ratio_d2m = title2 + " / MC";


	//histo1->SetTitle(title1.c_str());
        histo2->SetTitle(title2.c_str());
     	//histomc1->SetTitle(titlemc1.c_str());
        histomc2->SetTitle(titlemc2.c_str());
        //histo3->SetTitle(ratio_d1m.c_str());
        histo4->SetTitle(ratio_d2m.c_str());
        //histo5->SetTitle(ratio_dd.c_str());
        //histo1->GetXaxis()->SetTitle("Track #eta");
        //histo1->GetYaxis()->SetTitle("Track #phi");
        histo2->GetXaxis()->SetTitle("Track #eta");
        histo2->GetYaxis()->SetTitle("Track #phi");
        //histomc1->GetXaxis()->SetTitle("Track #eta");
        //histomc1->GetYaxis()->SetTitle("Track #phi");
        histomc2->GetXaxis()->SetTitle("Track #eta");
        histomc2->GetYaxis()->SetTitle("Track #phi");
        //histo3->GetXaxis()->SetTitle("Track #eta");
        //histo3->GetYaxis()->SetTitle("Track #phi");
	histo4->GetXaxis()->SetTitle("Track #eta");
        histo4->GetYaxis()->SetTitle("Track #phi");
        /*
	histo5->GetXaxis()->SetTitle("Track #eta");
        histo5->GetYaxis()->SetTitle("Track #phi");
	histo3->Divide(histomc1);
        histo3->GetZaxis()->SetRangeUser(0.4,1.6);
	*/
	histo4->Divide(histomc2);
        histo4->GetZaxis()->SetRangeUser(0.4,1.6);
	//histo5->Divide(histo2);
        /*
	TCanvas *c1=new TCanvas("c1","c1");
        c1->cd();
        c1->Draw();
        histo1->Draw("colz");
	c1->SaveAs("Data2024I.png");
        c1->SaveAs("Data2024I.pdf");
        */
	TCanvas *c2=new TCanvas("c2","c2");
        c2->cd();
        c2->Draw();
        histo2->Draw("colz");
        c2->SaveAs("Data2025C.png");
        c2->SaveAs("Data2025C.pdf");
        /*
	TCanvas *cmc1=new TCanvas("cmc1","cmc1");
        cmc1->cd();
        cmc1->Draw();
        histomc1->Draw("colz");
        cmc1->SaveAs("MC2024I.png");
        cmc1->SaveAs("MC2024I.pdf");
        */
	TCanvas *cmc2=new TCanvas("cmc2","cmc2");
        cmc2->cd();
        cmc2->Draw();
        histomc2->Draw("colz");
        cmc2->SaveAs("MC2025C.png");
        cmc2->SaveAs("MC2025C.pdf");
	/*
	TCanvas *c3=new TCanvas("c3","c3");
        c3->cd();
        c3->Draw();
        histo3->Draw("colz");
        c3->SaveAs("Data2024I_MC.png");
        c3->SaveAs("Data2024I_MC.pdf");
        */
	TCanvas *c4=new TCanvas("c4","c4");
        c4->cd();
        c4->Draw();
        histo4->Draw("colz");
        c4->SaveAs("Data2025C_MC.png");
        c4->SaveAs("Data2025C_MC.pdf");
        /*
	TCanvas *c5=new TCanvas("c5","c5");
        c5->cd();
        c5->Draw();
        histo5->Draw("colz");
        c5->SaveAs("Data_2025Cvs2024I.png");
        c5->SaveAs("Data_2025Cvs2024I.pdf");
	*/
}
