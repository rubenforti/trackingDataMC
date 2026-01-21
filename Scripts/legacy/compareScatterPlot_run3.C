
//#ifndef __CINT__
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TLine.h"
#include "TText.h"
#include "TPaveText.h"
#include "TString.h"
#include "CMS_lumi.h"
//#endif
#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include "./tdrstyle.C"

using namespace std;
typedef unsigned int uint;

void tokenize(const string& str, vector<string>& tokens, const string& delimiters=" ");
void openFiles(vector<TFile*>& v, vector<string>& l, const char* f, const string& yr);
void closeFiles(vector<TFile*>& v);
void readHistograms(vector<string>& v, const char* filename="highpurityScatter_run3.txt");
void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, const string& yr); 
void compareHisto(const vector<TFile*>& v, const char* folder, const char* hname); 
void compareScatterPlot_run3(string year) {
  vector<TFile*> filelist;
  vector<string> lglist;
  const char* filename;
  openFiles(filelist, lglist, filename, year);
  cout << ">>> # of Root files: " << filelist.size() << endl;

  vector<string> hlist;
  readHistograms(hlist);
  cout << ">>> # of histograms: " << hlist.size() << endl;
  TCanvas* canvas = new TCanvas("canvas", "canvas");
  canvas->SetCanvasSize(1080, 720);
  canvas->Divide(2,1);

  for (uint i = 0; i < hlist.size(); ++i) {
    string line(hlist[i]);
    vector<string> tokens;
    tokenize(line, tokens, ":");
    assert(tokens.size() > 1);
    compareHisto(canvas, filelist, lglist, tokens, year);
  }
  closeFiles(filelist);
}

void compareHisto(const vector<TFile*>& v, const char* folder, const char* hname) 
{
  for (uint i = 0; i < v.size(); ++i) {
    TFile* f = v[i];
    if (i==0) f->cd("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/highPurityTracks/");
    if (i==1) f->cd("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/highPurityTracks/");
    if (i==2 || i==3 || i==4) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/highPurityTracks/");

//    if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/highPurityTracks/");
    TH2D *h = dynamic_cast<TH2D*>(gDirectory->Get(hname));
    if (!h) continue;
    h->SetMarkerColor(i);
    h->Draw(i ? "SAME" : "");
  }
}
void textonplot(double x1, double y1, double x2, double y2, double txtfont, TString s){
  TPaveText* tText1 = new TPaveText(x1, y1, x2, y2, "brNDC");
  tText1->SetBorderSize(0);
  tText1->SetFillColor(0);
  tText1->SetFillStyle(0);
  TText *t1 = tText1->AddText(s);
  tText1->SetTextSize(0.045);
  tText1->SetTextFont(txtfont);
  tText1->Draw();
}

void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, const string& yr) 
{
  std::cout<<"ComparemHisto Start"<<std::endl;

  string datafolder, yearinfo, luminfo;
  if (yr == "2016pre") {
    datafolder = "DQMData/Run 278509/StandaloneTrackMonitor/Run summary/highPurityTracks/";
    yearinfo   = "2016 Old APV settings";
    luminfo    = "#sqrt{s} = 13 TeV, L = 0.296 fb^{-1}";
  }
  else if (yr == "2016post") {
    datafolder = "DQMData/Run 278808/StandaloneTrackMonitor/Run summary/highPurityTracks/";
    yearinfo   = "2016";
    luminfo    = "#sqrt{s} = 13 TeV, L = 0.280 fb^{-1}";
  }
  else if (yr == "2017") {
    datafolder = "DQMData/Run 999999/StandaloneTrackMonitor/Run summary/highPurityTracks/";
    yearinfo   = "Run III";
    luminfo    = "#sqrt{s} = 900 GeV, L = 1014 #mub^{-1}";
  }
  else if (yr == "2018") {
    datafolder = "DQMData/Run 317626/StandaloneTrackMonitor/Run summary/highPurityTracks/";
    yearinfo   = "2018";
    luminfo    = "#sqrt{s} = 13 TeV, L = 0.527 fb^{-1}";
  }

  
  string folder(tokens[0]);
  string hname(tokens[1]);

  canvas->cd(1);
  gPad->SetLogz();
  //TPad* pad1 = new TPad("pad1", "pad1",0,0,1,1);
  //pad1->Draw();
  // pad1->cd();

  TLegend *legend1 = new TLegend(0.40,0.83,0.60,0.90);

  double hmax = -1;
  double nentriesdata = 0;
  //std::cout << "v size : " << v.size() << std::endl;
  for (uint i = 0; i < v.size(); ++i) {
    //    if (!(i==0 || i==2)) continue;
    if (!(i==0)) continue;
    TFile* f = v[i];
    if (i==0 ) f->cd(datafolder.c_str());
    //f->cd(folder.c_str());
    TH2F *h = dynamic_cast<TH2F*>(gDirectory->Get(hname.c_str()));
    //TH1* h = gDirectory->Get(hname.c_str());
    //    if (!h) std::cout << "histo not found in data " << std::endl;
    if (!h) continue;

    double xmin1 = h->GetXaxis()->GetFirst();
    double xmax1 = h->GetXaxis()->GetLast();
    double ymin1 = h->GetYaxis()->GetFirst();
    double ymax1 = h->GetYaxis()->GetLast();
    double hinteg = h->Integral(xmin1, xmax1, ymin1, ymax1); 

    //#    h->Scale(1./hinteg);
    //    h->Scale(1./h->Integral("width"));
    //    nentriesdata = h->Integral();

    //std::cout << "data integral : " << h->Integral();
    /* std::vector<double> xbincontent;
    for (int ib = 0; ib < h->GetNbinsX(); ib++){
      //std::cout<< "GetNbinsX() : " << h->GetNbinsX() << std::endl;
      //      double hgb_tot = h->GetBinContent(ib+1, 2);
      // std::cout<< "hgb_tot : " << hgb_tot << std::endl;
      double hgb_tot = 0.;
      for (int jb = 0; jb < h->GetNbinsY(); jb++){
	hgb_tot += h->GetBinContent(ib+1, jb+1);
      }
      //      std::cout << "bin content of xbins : " << hgb_tot << std::endl;
      xbincontent.push_back(hgb_tot);
    } 

    for (int ib = 0; ib < h->GetNbinsX(); ib++){
      for (int jb = 0; jb < h->GetNbinsY(); jb++){
	h->GetBinContent(ib+1, jb+1) == 0 ? h->SetBinContent(ib+1, jb+1, 0) : h->SetBinContent(ib+1, jb+1, h->GetBinContent(ib+1, jb+1)/xbincontent[ib]);
      }
      }*/


    //if (i == 0) nentriesdata = h->Integral("width");
    //else h->(nentriesdata/h->Integral("width"));
    //if (i == 0) h->Scale(0.896042);
    //if (i == 1) h->Scale(1130*71.163*1.05*(5153./10000)*(1./101967));
    //cout << "i = " << i << ", Integral = " << h->Integral() << endl;
    //    if (h->GetMaximum() > hmax) hmax = h->GetMaximum();
    
    std::vector<TString> StopReasonName;
    StopReasonName.push_back("UNINITIALIZED");
    StopReasonName.push_back("MAX_HITS");
    StopReasonName.push_back("MAX_LOST_HITS");
    StopReasonName.push_back("MAX_CONSECUTIVE_LOST_HITS");
    StopReasonName.push_back("LOST_HIT_FRACTION");
    StopReasonName.push_back("MIN_PT");
    StopReasonName.push_back("CHARGE_SIGNIFICANCE");
    StopReasonName.push_back("LOOPER");
    StopReasonName.push_back("MAX_CCC_LOST_HITS");
    StopReasonName.push_back("NO_SEGMENTS_FOR_VALID_LAYERS");
    StopReasonName.push_back("SEED_EXTENSION");
    StopReasonName.push_back("NOT_STOPPED");

    std::vector<TString> AlgoName;
    AlgoName.push_back("undefAlgorithm");
    AlgoName.push_back("ctf");
    AlgoName.push_back("duplicateMerge");
    AlgoName.push_back("cosmics");
    AlgoName.push_back("initialStep");
    AlgoName.push_back("lowPtTripletStep");
    AlgoName.push_back("pixelPairStep");
    AlgoName.push_back("detachedTripletStep");
    AlgoName.push_back("mixedTripletStep");
    AlgoName.push_back("pixelLessStep");
    AlgoName.push_back("tobTecStep");
    AlgoName.push_back("jetCoreRegionalStep");
    AlgoName.push_back("conversionStep");
    AlgoName.push_back("muonSeededStepInOut");
    AlgoName.push_back("muonSeededStepOutIn");
    AlgoName.push_back("outInEcalSeededConv");
    AlgoName.push_back("inOutEcalSeededConv");
    AlgoName.push_back("nuclInter");
    AlgoName.push_back("standAloneMuon");
    AlgoName.push_back("globalMuon");
    AlgoName.push_back("cosmicStandAloneMuon");
    AlgoName.push_back("cosmicGlobalMuon");
    AlgoName.push_back("highPtTripletStep");
    AlgoName.push_back("lowPtQuadStep");
    AlgoName.push_back("detachedQuadStep");
    AlgoName.push_back("displacedGeneralStep");
    AlgoName.push_back("reservedForUpgrades2");
    AlgoName.push_back("bTagGhostTracks");
    AlgoName.push_back("beamhalo");
    AlgoName.push_back("gsf");
    AlgoName.push_back("hltPixel");
    AlgoName.push_back("hltIter0");
    AlgoName.push_back("hltIter1");
    AlgoName.push_back("hltIter2");
    AlgoName.push_back("hltIter3");
    AlgoName.push_back("hltIter4");
    AlgoName.push_back("hltIterX");
    AlgoName.push_back("hiRegitMuInitialStep");
    AlgoName.push_back("hiRegitMuLowPtTripletStep");
    AlgoName.push_back("hiRegitMuPixelPairStep");
    AlgoName.push_back("hiRegitMuDetachedTripletStep");
    AlgoName.push_back("hiRegitMuMixedTripletStep");
    AlgoName.push_back("hiRegitMuPixelLessStep");
    AlgoName.push_back("hiRegitMuTobTecStep");
    AlgoName.push_back("hiRegitMuMuonSeededStepInOut");
    AlgoName.push_back("hiRegitMuMuonSeededStepOutIn");

    string option=("colz");
    // if (i==0) {
    h->SetTitle("Data");
    //h->SetTitle(h_title);
    h->SetTitleSize(0.03);
    h->SetTitleOffset(0.80);
    if (tokens.size() > 2) h->GetXaxis()->SetTitle(tokens[2].c_str());
    if (tokens.size() > 3) h->GetYaxis()->SetTitle(tokens[3].c_str());
    h->GetXaxis()->SetLabelSize(0.022);
    h->GetYaxis()->SetLabelSize(0.018);
    h->GetXaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.60);
    h->GetYaxis()->SetTitleOffset(1.25);
    //h->SetMinimum(0.0);
    
    if (hname == "trackChi2probVsStoppingSource2D") {
      for (size_t ibin=0; ibin<StopReasonName.size(); ibin++) {
	h->GetXaxis()->SetBinLabel(ibin+1,StopReasonName[ibin]);
      }
    }
    
    if (hname == "trackChi2probVsAlgo2D" || hname == "trackChi2probVsOrigAlgo2D") {
      for (size_t ibin=0; ibin<AlgoName.size(); ibin++) {
	h->GetXaxis()->SetBinLabel(ibin+1,AlgoName[ibin]);
      }
	h->GetXaxis()->SetRangeUser(0.,26.0);
      }

      h->SetMarkerSize(0.8);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);

      TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
      hstats->SetTextColor(i+1);
      hstats->SetOptStat(1111);
      hstats->Draw();
      h->GetListOfFunctions()->Add(hstats);
      hstats->SetParent(h);
      //}

    h->Draw(option.c_str());
    h->GetXaxis()->SetLabelSize(0.025);
    h->GetYaxis()->SetLabelSize(0.025);
    h->GetZaxis()->SetLabelSize(0.025);
    

  }


  canvas->cd(2);
  gPad->SetLogz();
  //TPad* pad1 = new TPad("pad1", "pad1",0,0,1,1);
  //pad1->Draw();
  // pad1->cd();

  TLegend *legend2 = new TLegend(0.40,0.83,0.60,0.90);

  // double hmax = -1;
  //double nentriesdata = 0;
  //std::cout << "v size : " << v.size() << std::endl;
  for (uint i = 0; i < v.size(); ++i) {
    //    if (!(i==0 || i==2)) continue;
    if (!(i==2)) continue;
    TFile* f = v[i];
    if (i==2 ) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/highPurityTracks/");
    //f->cd(folder.c_str());
    TH2F *h = dynamic_cast<TH2F*>(gDirectory->Get(hname.c_str()));
    //TH1* h = gDirectory->Get(hname.c_str());
    //    if (!h) std::cout << "histo not found in mc " << std::endl;
    if (!h) continue;
    //    std::cout << "mc integral : " << h->Integral();
    //    h->Scale(1./h->Integral("width")); 
  
    double xmin2 = h->GetXaxis()->GetFirst();
    double xmax2 = h->GetXaxis()->GetLast();
    double ymin2 = h->GetYaxis()->GetFirst();
    double ymax2 = h->GetYaxis()->GetLast();
    double hinteg2 = h->Integral(xmin2, xmax2, ymin2, ymax2);

    //#    h->Scale(1./hinteg2);


    //h->Scale(nentriesdata/h->Integral());
    //std::cout << "mc integral : " << h->Integral();
    // normalizing each bin separately to unity
    /*    std::vector<double> xbincontent;
    for (int ib = 0; ib < h->GetNbinsX(); ib++){
      //std::cout<< "GetNbinsX() : " << h->GetNbinsX() << std::endl;
      //      double hgb_tot = h->GetBinContent(ib+1, 2);
      // std::cout<< "hgb_tot : " << hgb_tot << std::endl;
      double hgb_tot = 0.;
      for (int jb = 0; jb < h->GetNbinsY(); jb++){
	hgb_tot += h->GetBinContent(ib+1, jb+1);
       }
      //     std::cout << "bin content of xbins : " << hgb_tot << std::endl;
      xbincontent.push_back(hgb_tot);
    } 

    for (int ib = 0; ib < h->GetNbinsX(); ib++){
      for (int jb = 0; jb < h->GetNbinsY(); jb++){
	h->GetBinContent(ib+1, jb+1) == 0 ? h->SetBinContent(ib+1, jb+1, 0) : h->SetBinContent(ib+1, jb+1, h->GetBinContent(ib+1, jb+1)/xbincontent[ib]);
      }
      }*/


    //if (i == 0) nentriesdata = h->Integral("width");
    //else h->(nentriesdata/h->Integral("width"));
    //if (i == 0) h->Scale(0.896042);
    //if (i == 1) h->Scale(1130*71.163*1.05*(5153./10000)*(1./101967));
    //cout << "i = " << i << ", Integral = " << h->Integral() << endl;
    //    if (h->GetMaximum() > hmax) hmax = h->GetMaximum();
    
    std::vector<TString> StopReasonName;
    StopReasonName.push_back("UNINITIALIZED");
    StopReasonName.push_back("MAX_HITS");
    StopReasonName.push_back("MAX_LOST_HITS");
    StopReasonName.push_back("MAX_CONSECUTIVE_LOST_HITS");
    StopReasonName.push_back("LOST_HIT_FRACTION");
    StopReasonName.push_back("MIN_PT");
    StopReasonName.push_back("CHARGE_SIGNIFICANCE");
    StopReasonName.push_back("LOOPER");
    StopReasonName.push_back("MAX_CCC_LOST_HITS");
    StopReasonName.push_back("NO_SEGMENTS_FOR_VALID_LAYERS");
    StopReasonName.push_back("SEED_EXTENSION");
    StopReasonName.push_back("NOT_STOPPED");

    std::vector<TString> AlgoName;
    AlgoName.push_back("undefAlgorithm");
    AlgoName.push_back("ctf");
    AlgoName.push_back("duplicateMerge");
    AlgoName.push_back("cosmics");
    AlgoName.push_back("initialStep");
    AlgoName.push_back("lowPtTripletStep");
    AlgoName.push_back("pixelPairStep");
    AlgoName.push_back("detachedTripletStep");
    AlgoName.push_back("mixedTripletStep");
    AlgoName.push_back("pixelLessStep");
    AlgoName.push_back("tobTecStep");
    AlgoName.push_back("jetCoreRegionalStep");
    AlgoName.push_back("conversionStep");
    AlgoName.push_back("muonSeededStepInOut");
    AlgoName.push_back("muonSeededStepOutIn");
    AlgoName.push_back("outInEcalSeededConv");
    AlgoName.push_back("inOutEcalSeededConv");
    AlgoName.push_back("nuclInter");
    AlgoName.push_back("standAloneMuon");
    AlgoName.push_back("globalMuon");
    AlgoName.push_back("cosmicStandAloneMuon");
    AlgoName.push_back("cosmicGlobalMuon");
    AlgoName.push_back("highPtTripletStep");
    AlgoName.push_back("lowPtQuadStep");
    AlgoName.push_back("detachedQuadStep");
    AlgoName.push_back("displacedGeneralStep");
    AlgoName.push_back("reservedForUpgrades2");
    AlgoName.push_back("bTagGhostTracks");
    AlgoName.push_back("beamhalo");
    AlgoName.push_back("gsf");
    AlgoName.push_back("hltPixel");
    AlgoName.push_back("hltIter0");
    AlgoName.push_back("hltIter1");
    AlgoName.push_back("hltIter2");
    AlgoName.push_back("hltIter3");
    AlgoName.push_back("hltIter4");
    AlgoName.push_back("hltIterX");
    AlgoName.push_back("hiRegitMuInitialStep");
    AlgoName.push_back("hiRegitMuLowPtTripletStep");
    AlgoName.push_back("hiRegitMuPixelPairStep");
    AlgoName.push_back("hiRegitMuDetachedTripletStep");
    AlgoName.push_back("hiRegitMuMixedTripletStep");
    AlgoName.push_back("hiRegitMuPixelLessStep");
    AlgoName.push_back("hiRegitMuTobTecStep");
    AlgoName.push_back("hiRegitMuMuonSeededStepInOut");
    AlgoName.push_back("hiRegitMuMuonSeededStepOutIn");

    string option=("colz");
    // if (i==0) {
    h->SetTitle("MC");
    //h->SetTitle(h_title);
    h->SetTitleSize(0.03);
    h->SetTitleOffset(0.80);
    if (tokens.size() > 2) h->GetXaxis()->SetTitle(tokens[2].c_str());
    if (tokens.size() > 3) h->GetYaxis()->SetTitle(tokens[3].c_str());
    h->GetXaxis()->SetLabelSize(0.022);
    h->GetYaxis()->SetLabelSize(0.018);
    h->GetXaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.60);
    h->GetYaxis()->SetTitleOffset(1.25);
    //h->SetMinimum(0.0);
    
    if (hname == "trackChi2probVsStoppingSource2D") {
      for (size_t ibin=0; ibin<StopReasonName.size(); ibin++) {
	h->GetXaxis()->SetBinLabel(ibin+1,StopReasonName[ibin]);
      }
    }
    
    if (hname == "trackChi2probVsAlgo2D" || hname == "trackChi2probVsOrigAlgo2D") {
      for (size_t ibin=0; ibin<AlgoName.size(); ibin++) {
	h->GetXaxis()->SetBinLabel(ibin+1,AlgoName[ibin]);
      }
      h->GetXaxis()->SetRangeUser(0.,26.0);
    }
    
    h->SetMarkerSize(0.8);
    h->SetMarkerStyle(20);
    h->SetMarkerColor(1);
    
    TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
    hstats->SetTextColor(i+1);
    hstats->SetOptStat(1111);
    hstats->Draw();
    h->GetListOfFunctions()->Add(hstats);
    hstats->SetParent(h);
    //}
    
    h->Draw(option.c_str());
    h->GetXaxis()->SetLabelSize(0.025);
    h->GetYaxis()->SetLabelSize(0.025);
    h->GetZaxis()->SetLabelSize(0.025);

  }
  // correct maximum height

  /*  TFile* f = v.at(0);
  f->cd(datafolder.c_str());
  //f->cd(folder.c_str());
  TH2D *h = dynamic_cast<TH2D*>(gDirectory->Get(hname.c_str()));
  assert(h);

  double fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  //  h->SetMaximum(fct * hmax);
  

  if (yr == "2016pre" || yr == "2016post")  textonplot(0.27, 0.75, 0.40, 0.83, 132, yearinfo);
  else textonplot(0.17, 0.80, 0.29, 0.87, 132, yearinfo);
  textonplot(0.63, 0.918, 0.80, 1.0, 22, luminfo);
  textonplot(0.10, 0.918, 0.18, 1.0, 22, "CMS");
  textonplot(0.22, 0.908, 0.39, 1.0, 12, "Preliminary");

  if (tokens.size() > 5 && tokens[5] == "log") pad1->SetLogx();
  if (tokens.size() > 6 && tokens[6] == "log") pad1->SetLogy();
  legend1->Draw();
  //  tText1->Draw();
  //tText2->Draw();
  pad1->Update();
  pad1->Modified();*/
  
  //  std::cout << "pad1 drawn successfully" << std::endl; 

  /*  TPaveText* tText11 = new TPaveText(0.65, 0.90, 0.80, 0.96, "brNDC");
  tText11->SetBorderSize(0);
  tText11->SetFillColor(0);
  //tText1->SetFillStyle(0);
  TText *t11 = tText11->AddText("#sqrt{s} = 13 TeV, L = 0.296 fb^{-1}");
  tText11->SetTextSize(0.03);
  tText11->Draw(); 
  //==========================================================
  TPaveText* tText12 = new TPaveText(0.15, 0.90, 0.27, 0.96, "brNDC");
  tText12->SetBorderSize(0);
  tText12->SetFillColor(0);
  //tText2->SetFillStyle(0);
  TText *t12 = tText12->AddText("CMS #it{Preliminary}");
  tText12->SetTextSize(0.03);
  tText12->Draw();
  */
  
  canvas->Update();
  canvas->Modified();
  char fname[256];
  sprintf(fname, "%s.png", hname.c_str());
  canvas->Print(fname);
  cout << "Profile printed successfully" << endl;
  //  canvas->Clear();
  return;
}
void openFiles(vector<TFile*>& v1, vector<string>& v2, const char* filename, const string& yr) {
  static const int BUF_SIZE = 256;

  if (yr == "2016pre") filename = "filelist_align_16pre.txt";
  else if (yr == "2016post") filename = "filelist_align_16post.txt";
  else if (yr == "2017") filename = "filelist_run3.txt";
  else if (yr == "2018") filename = "filelist_align_18.txt";

  // Open the file containing the datacards
  ifstream fin(filename, ios::in);
  if (!fin) {
    cerr << "Input File: " << filename << " could not be opened!" << endl;
    return;
  }
  char buf[BUF_SIZE];
  while (fin.getline(buf, BUF_SIZE, '\n')) {  // Pops off the newline character
    string line(buf);
    if (line.empty()) continue;
    if (line.substr(0,2) == "//") continue;
    if (line.substr(0,1) == "#") continue;
    cout << "file: " << line << endl;

    vector<string> tokens;
    tokenize(line, tokens, ":");
    assert(tokens.size() > 1);

    TFile* f = TFile::Open(tokens[0].c_str());
    v1.push_back(f);
    v2.push_back(tokens[1].c_str());
  }
  fin.close();
}
void closeFiles(vector<TFile*>& v) {
  for (uint i = 0; i < v.size(); ++i) {
    TFile* f = v[i];
    f->Close();
  }
}
void readHistograms(vector<string>& v, const char* filename) {
  static const int BUF_SIZE = 512;

  // Open the file containing the datacards
  ifstream fin(filename, ios::in);    
  if (!fin) {
    cerr << "Input File: " << filename << " could not be opened!" << endl;
    return;
  }
  char buf[BUF_SIZE];
  while (fin.getline(buf, BUF_SIZE, '\n')) {  // Pops off the newline character
    string line(buf);
    if (line.empty()) continue;
    if (line.substr(0,2) == "//") continue;
    if (line.substr(0,1) == "#") continue;

    cout << "histogram: " << line << endl;
    v.push_back(line);
  }
  fin.close();
}
void tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);

  // Find first "non-delimiter".
  string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);

    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}
