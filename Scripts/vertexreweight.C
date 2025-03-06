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

void pileupreweight() {
//  TFile* file1 = TFile::Open("/afs/cern.ch/work/p/ppalit/public/Data_MC/forgitpush/CMSSW_10_6_2/src/DQM/TrackingMonitorSource/Jobs/Run2017_UL/data/SingleMu/DQM_V0001_R000278509__Global__CMSSW_X_Y_Z__RECO.root");
  TFile* file1 = TFile::Open("/afs/cern.ch/work/d/dbruschi/TrackingPOG/Copy/CMSSW_13_0_11/src/DQM/TrackingMonitorSource/test/13p6_Run2023C/DQM_V0001_R000999999__Global__CMSSW_X_Y_Z__RECO.root");  
  file1->ls();  
//  TFile* file2 = TFile::Open("/afs/cern.ch/work/p/ppalit/public/Data_MC/forgitpush/CMSSW_10_6_2/src/DQM/TrackingMonitorSource/Jobs/Run2017_UL/data/ZtoMM/nopuroot/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root");
  TFile* file2 = TFile::Open("/afs/cern.ch/work/d/dbruschi/TrackingPOG/CMSSW_13_0_12/src/DQM/TrackingMonitorSource/test/MC/RelValSingleNu/C/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root");
  file2->ls(); 
 
  TH1F* h1 = dynamic_cast<TH1F*>(file1->Get("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/highPurityTracks/nVertex"));	
//  TH1F* h1 = dynamic_cast<TH1F*>(file1->Get("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/ElectronTracks/nVertex"));
  h1->SetName("nvertex_data");
  cout << "1st histo " << h1->GetName() << " opened successfully" << endl;

  TH1F* h2 = dynamic_cast<TH1F*>(file2->Get("DQMData/Run 1/StandaloneTrackMonitor/Run summary/highPurityTracks/nVertex"));
  //TH1F* h2 = dynamic_cast<TH1F*>(file2->Get("DQMData/Run 1/StandaloneTrackMonitor/Run summary/ElectronTracks/nVertex"));
  h2->SetName("nvertex_mc");
  cout << "2nd histo " << h2->GetName() << " opened successfully" << endl;

  cout << h1->Integral() << ", " << h2->Integral() << endl;

  //h2->Scale(h1->Integral("width")/h2->Integral("width"));
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

  TH1F* hn = new TH1F("nvertexweight", "Reco::Vertex Weight Factor", 120, 0, 120);
  for (int j = 1; j <= nbinsx1; ++j) {
    double v1 = h1->GetBinContent(j);
    double v2 = h2->GetBinContent(j);
    float rat = (v2 > 0) ? v1/v2 : 0;
    cout << "bin: " << j << " " << v1 << " " << v2 << " " << rat << endl;
    hn->SetBinContent(j,rat);
  } 
 
  TFile* f1 = TFile::Open("/afs/cern.ch/work/d/dbruschi/TrackingPOG/CMSSW_13_0_12/src/DQM/TrackingMonitorSource/test/MC/RelValSingleNu/C/PileupScaleFactor_999999_wrt_nVertex_Run2023C.root", "RECREATE");
  hn->Write();
  f1->Write();
  f1->Close();
  
  file1->Close();
}
