#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TLine.h"
#include "TPaveStats.h"
#include "TAxis.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;
typedef unsigned int uint;

void zptreweight() {

    // DATA
    //string fname_data = "/eos/home-r/rforti/trackingPOG/analyses_2025/data/ZeroBias/Run2025G_golden_cmssw-15-0-15/DQM_V0001_R000999999__Global__CMSSW_X_Y_Z__RECO.root";
    string fname_data = "/eos/home-r/rforti/trackingPOG/analyses_2025/data/ZEE/Run2025C_golden_cmssw-15-0-15/DQM_V0001_R000999999__Global__CMSSW_X_Y_Z__RECO.root";

    // MC
    //string fname_mc = "/eos/home-r/rforti/trackingPOG/analyses_2024/MC/ZeroBias/SingleNeutrino_Par-E-10-gun/nvtx_reweighted_2025G_golden/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root";
    string fname_mc = "/eos/home-r/rforti/trackingPOG/analyses_2024/MC/ZEE/DYto2E-4Jets_Bin-MLL-50_RunIIISummer24DRPremix/nvtx_reweighted_2025C_golden/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root";
    // Out
    string fname_out = "../MC/ZEE/ZptScaleFactor_999999_Run2025C_golden.root";

    TFile* file1 = TFile::Open(fname_data.c_str());  
    file1->ls();  
    TFile* file2 = TFile::Open(fname_mc.c_str());
    file2->ls(); 

    TH1* h1 = dynamic_cast<TH1*>(file1->Get("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/ElectronTracks/Zpt"));	
    h1->SetName("Zpt_data");
    cout << "1st histo " << h1->GetName() << " opened successfully" << endl;

    TH1* h2 = dynamic_cast<TH1*>(file2->Get("DQMData/Run 1/StandaloneTrackMonitor/Run summary/ElectronTracks/Zpt"));
    h2->SetName("Zpt_mc");
    cout << "2nd histo " << h2->GetName() << " opened successfully" << endl;

    cout << h1->Integral() << ", " << h2->Integral() << endl;

    h1->Scale(1.0/h1->Integral("width"));
    h2->Scale(1.0/h2->Integral("width"));  

    cout << "After Scaling" << h1->Integral() << ", " << h2->Integral() << endl;

    TAxis* xaxis1 = h1->GetXaxis();
    int nbinsx1 = h1->GetNbinsX();
    double xmin1 = xaxis1->GetXmin();
    double xmax1 = xaxis1->GetXmax();

    cout << nbinsx1 << ";" << xmin1 << ";" << xmax1 << endl;

    TAxis* xaxis2 = h2->GetXaxis();
    int nbinsx2 = h2->GetNbinsX();
    double xmin2 = xaxis2->GetXmin();
    double xmax2 = xaxis2->GetXmax();

    cout << nbinsx2 << ";" << xmin2 << ";" << xmax2 << endl;

    TH1F* hn = new TH1F("ptweight", "Pt Weight Factor", nbinsx2, xmin2, xmax2);
    for (int j = 1; j <= nbinsx1; ++j) {
        double v1 = h1->GetBinContent(j);
        double v2 = h2->GetBinContent(j);
        float rat = (v2 > 0) ? v1/v2 : 0;
        cout << "bin: " << j << " " << v1 << " " << v2 << " " << rat << endl;
        hn->SetBinContent(j,rat);
    }

    TFile* f1 = TFile::Open(fname_out.c_str(), "RECREATE");
    hn->Write();
    f1->Write();
    f1->Close();

    file1->Close();
}
