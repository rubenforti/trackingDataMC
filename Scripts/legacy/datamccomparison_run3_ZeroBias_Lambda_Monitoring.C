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
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;
typedef unsigned int uint;

void tokenize(const string& str, vector<string>& tokens, const string& delimiters=" ");
void openFiles(vector<TFile*>& v, vector<string>& l, const char* f, const string& yr);
//void openFiles(vector<TFile*>& v, vector<string>& l, const char* filename="filelist_align_18.txt");
void closeFiles(vector<TFile*>& v);
void readHistograms(vector<string>& v, const char* filename="lambdaHistolist_run3_monitoring.txt");
void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, const string& yr); 
void compareHisto(const vector<TFile*>& v, const char* folder, const char* hname); 
void datamccomparison_run3(string year) {
  vector<TFile*> filelist;
  vector<string> lglist;
  const char* filename;
  //  openFiles(filelist, lglist);
  
  openFiles(filelist, lglist, filename, year); 
  cout << ">>> # of Root files: " << filelist.size() << endl;

  vector<string> hlist;
  readHistograms(hlist);
  cout << ">>> # of histograms: " << hlist.size() << endl;
  
  TCanvas* canvas = new TCanvas("canvas", "canvas");
  canvas->SetCanvasSize(800, 800);
  canvas->SetLeftMargin(0.11);

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
    //f->cd(folder);
    if (i==0) f->cd("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==1) f->cd("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==2 || i==3 || i==4) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    //if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TH1F *h = dynamic_cast<TH1F*>(gDirectory->Get(hname));
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
  tText1->SetTextSize(0.058); //
  tText1->SetTextFont(txtfont);
  tText1->Draw();
}
void rmdot(double x1, double y1, double x2, double y2, double txtfont, TString s){
  TPaveText* tText1 = new TPaveText(x1, y1, x2, y2, "brNDC");
  tText1->SetBorderSize(0);
  tText1->SetFillColor(0);
  tText1->SetFillStyle(1001);
  TText *t1 = tText1->AddText(s);
  tText1->SetTextSize(0.058); //                                                                                                                      
  tText1->SetTextFont(txtfont);
  tText1->Draw();
}

void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, const string& yr) 
{
  std::cout<<"ComparemHisto Start"<<std::endl;

  string datafolder, yearinfo, luminfo;
  if (yr == "2016pre") {
    datafolder = "DQMData/Run 278509/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "2016 Old APV settings"; 
    luminfo    = "0.30 fb^{-1} (13 TeV)";
  } 
  else if (yr == "2016post") {
    datafolder = "DQMData/Run 278808/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "2016 New APV settings"; 
    luminfo    = "0.28 fb^{-1} (13 TeV)";
  } 
  else if (yr == "2017") {
    datafolder = "DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "#Lambda (ZeroBias)"; 
    luminfo    = "13.6 TeV"; //Modify to show the correct integrated luminosity
  } 
  else if (yr == "2018") {
    datafolder = "DQMData/Run 317626/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "2018"; 
    luminfo    = "0.53 fb^{-1} (13 TeV)";
  } 

  string folder(tokens[0]);
  string hname(tokens[1]);
  /*
  TPaveText* tText1 = new TPaveText(0.63, 0.91, 0.80, 1.0, "brNDC");
  tText1->SetBorderSize(0);
  tText1->SetFillColor(0);
  //tText1->SetFillStyle(0);                                                                                                                         
  TText *t1 = tText1->AddText("#sqrt{s} = 13 TeV, L = 0.296 fb^{-1}");
  tText1->SetTextSize(0.05);
  tText1->Draw();
  //==========================================================                                                                                       
  TPaveText* tText2 = new TPaveText(0.19, 0.91, 0.31, 1.0, "brNDC");
  tText2->SetBorderSize(0);
  tText2->SetFillColor(0);
  //tText2->SetFillStyle(0);
  TText *t2 = tText2->AddText("CMS #it{Preliminary}");
  tText2->SetTextSize(0.05);
  tText2->Draw();*/
  //TText *t3 = tText2->AddText("#");
  //tText2->SetTextSize(0.032);
  //tText2->Draw();

  canvas->cd();
//  TString h_title = "Run 275376 : " + tokens[2];
  //TString h_title = "Run 999999 : " + tokens[2];
  TString h_title = tokens[2];
  gStyle->SetLineWidth(3);
  gStyle->SetOptStat(0);
  /*  TLatex *texL;
  texL->SetTextSize(0.03);
  texL->DrawLatexNDC(0.11,0.91,"#scale[1.5]{CMS} Phase-2 Simulation");

  TLatex *texR;
  texR->SetTextSize(0.035);
  texR->SetTextAlign(31);
  TString toDisplay = "14 TeV, 200 PU";
  texR->DrawLatexNDC(0.90,0.91,toDisplay.Data());*/

  TPad* pad11 = new TPad("pad11", "pad11",0,0.30,1.0,1); // 0.27
  //pad1->SetBottomMargin(0.01);
  pad11->SetTickx();
  pad11->SetTicky();
  pad11->SetFrameLineWidth(3);
  pad11->Draw();
  pad11->cd();
  

  TLegend *legend11;
  if (yr == "2016pre" ) legend11 = new TLegend(0.40,0.66,0.70,0.78);
  else if (yr == "2016post") legend11 = new TLegend(0.40,0.66,0.70,0.78);
  else legend11 = new TLegend(0.63,0.73,0.92,0.85);

  double hmax = -1;
  double nentriesdata = 0;
  for (uint i = 0; i < v.size(); ++i) {
    if (!(i==0 || i==2)) continue;
    TFile* f = v[i];
    //f->cd(folder.c_str());
    if (i==0) f->cd(datafolder.c_str()); // UL data
    //if (i==1) f->cd("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/"); // EOY data
    if (i==2 ) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/"); // UL & EOY MC
    //if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TH1F *h = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
    if (hname == "ip3dToPV" || hname == "iperr3dToPV" ) h->Rebin(1.); // 5 
    if (hname == "nTracLambda") h->Rebin(10);
    if (hname == "sip3dToPV") h->Rebin(5); // 10
    if (hname == "DistanceOfClosestApproachToPV") h->Rebin(10);
    if ((std::string(hname).find(std::string("to10cm"))!=std::string::npos)||(std::string(hname).find(std::string("to100"))!=std::string::npos)) h->Rebin(10);
    if ( hname == "sip2dToPV") h->Rebin(10);
    if ( hname == "trackChi2bynDOF") h->Rebin(2);
    //    if (hname == "nTracLambda") h->Rebin();
    //TH1* h = gDirectory->Get(hname.c_str());
    if (!h) continue;
    h->Sumw2();   

    if (hname == "iperr3dToPV") h->GetXaxis()->SetRangeUser(0.,1.0);
    if (hname == "sip3dToPV") h->GetXaxis()->SetRangeUser(-5.0,5.0);
    if ( hname == "trackChi2bynDOF") h->GetXaxis()->SetRangeUser(0.,5.0);;

    //cout << "Maximum of before scaling " << h->GetMaximum() << endl;
    if (i == 0) nentriesdata = h->Integral("width");
    else h->Scale(nentriesdata/h->Integral("width"));
    //if (i == 0) h->Scale(0.896042);
    //if (i == 1) h->Scale(1130*71.163*(5153./10000)*(1./101967.));
    //h->Scale(1.0/h->Integral("width"));

    if (h->GetMaximum() > hmax) hmax = h->GetMaximum();
    h->SetTitle("");
    string option=("E");
    if (i==0) {
      //h->SetTitle(h_title);
      h->SetTitleSize(0.040);
      h->SetTitleOffset(0.105);
      if (tokens.size() > 2) h->GetXaxis()->SetTitle(tokens[2].c_str());
      if (tokens.size() > 3) h->GetYaxis()->SetTitle(tokens[3].c_str());
      h->GetXaxis()->SetLabelSize(0.045);
      h->GetYaxis()->SetLabelSize(0.045);
      h->GetXaxis()->SetTitleSize(0.05);
      h->GetYaxis()->SetTitleSize(0.05);
      h->GetXaxis()->SetTitleOffset(0.77);
      h->GetYaxis()->SetTitleOffset(0.99);
      //h->SetMinimum(0.0);

      h->SetMarkerSize(2.0);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);
      h->SetLineColor(1);
      h->SetLineWidth(3);

      TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
      hstats->SetTextColor(i+1);
      hstats->SetOptStat(1111);
      hstats->Draw();
      h->GetListOfFunctions()->Add(hstats);
      hstats->SetParent(h);
      
    }
    else if (i==2){
      option = "HISTSAMES";
      gPad->RedrawAxis();
      h->SetLineWidth(3);
      h->SetLineStyle(1);
      h->SetLineColor(i+1);

      TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
      hstats->SetTextColor(i+1);
      hstats->SetOptStat(1111);
      hstats->Draw();
      h->GetListOfFunctions()->Add(hstats);
      hstats->SetParent(h);

    }
    
    /*if (i==1){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);
      h->SetLineColor(1);
      }*/
    if (i==2){
      h->SetMarkerSize(1.);
      h->SetMarkerStyle(22);
      h->SetMarkerColor(2);
      h->SetLineColor(2);
    }
    /*else if (i==3){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(22);
      h->SetMarkerColor(2);
      h->SetLineColor(2);
    }
    else if (i==4){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(26);
      h->SetMarkerColor(4);
      h->SetLineColor(4);
      }*/

    TGaxis::SetExponentOffset(-0.07,0,"y");
    h->GetYaxis()->SetLabelSize(0.045);
    h->GetXaxis()->SetLabelSize(0.045);
    h->Draw(option.c_str());
    //hstats->Draw();
    legend11->AddEntry(h, lglist[i].c_str(), ((i == 2) ? "L" : "PL"));
    //    legend11->AddEntry(h, lglist[i].c_str(), "P");
    legend11->SetTextSize(0.035);
    legend11->SetBorderSize(0);
    legend11->SetFillStyle(0);
    //    pad11->Update();
    //pad11->Modified();
  }

  // correct maximum height
  TFile* f = v.at(0);
  //  TFile* f_eoyd = v.at(1);
  //f->cd(folder.c_str());
  f->cd(datafolder.c_str());
  
  TH1F *h = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
  assert(h);

  double fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  h->SetMaximum(fct * hmax);

  //f_eoyd->cd("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");  
  //TH1F *h_eoyd = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
  //assert(h_eoyd);

  //  fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  //h_eoyd->SetMaximum(fct * hmax);

  //  if (yr == "2016pre" || yr == "2016post")  textonplot(0.208, 0.80, 0.33, 0.87, 132, yearinfo);
  textonplot(0.16, 0.83, 0.28, 0.85, 42,  yearinfo);
  textonplot(0.66, 0.875, 0.83, 1.0, 42, luminfo);
  textonplot(0.10, 0.875, 0.18, 1.0, 62, "CMS");
  textonplot(0.24, 0.860, 0.34, 1.0, 52, "            Work in progress");
  rmdot(0.98, 0.97, 1.0, 1.0, 52, "             ");
  
  if (tokens.size() > 5 && tokens[5] == "log") pad11->SetLogx();
  if (tokens.size() > 6 && tokens[6] == "log") pad11->SetLogy();
  legend11->Draw();
  //  texL->Draw("same");
  //texR->Draw("same");
  //  tText1->Draw();
  //tText2->Draw();
  pad11->Update();
  pad11->Modified();
  
  std::cout << "pad11 drawn successfully" << std::endl; 

  /*    TPaveText* tText12 = new TPaveText(0.63, 0.91, 0.80, 1.0, "brNDC");
  tText12->SetBorderSize(0);
  tText12->SetFillColor(0);
  //tText1->SetFillStyle(0);                                                                                                                         
  TText *t12 = tText12->AddText("#sqrt{s} = 13 TeV, L = 0.296 fb^{-1}");
  tText12->SetTextSize(0.05);
  tText12->Draw();
  //==========================================================                                                                                       
  TPaveText* tText22 = new TPaveText(0.19, 0.91, 0.31, 1.0, "brNDC");
  tText22->SetBorderSize(0);
  tText22->SetFillColor(0);
  //tText2->SetFillStyle(0);
  TText *t22 = tText22->AddText("CMS #it{Preliminary}");
  tText22->SetTextSize(0.05);
  tText22->Draw();*/

  /*
  canvas->cd();

  TPad* pad12 = new TPad("pad12", "pad12",0.5,0.50,1,1);
  //pad1->SetBottomMargin(0.01);                                                                                                                      
  pad12->Draw();
  pad12->cd();

  TLegend *legend12 = new TLegend(0.70,0.78,0.90,0.90);

  hmax = -1;
  nentriesdata = 0;
  for (uint i = 0; i < v.size(); ++i) {
    if (!(i==1 || i==4)) continue;
    TFile* f = v[i];
    //f->cd(folder.c_str());
    //if (i==0) f->cd("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/"); // UL data
    if (i==1) f->cd(datafolder.c_str()); // EOY data
    if (i==4) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/"); // UL & EOY MC
    //if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TH1F *h = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
    if (hname == "ip3dToPV" || hname == "iperr3dToPV") h->Rebin(5);
    if (hname == "nTracLambda" ) h->Rebin(10);
    //TH1* h = gDirectory->Get(hname.c_str());
    if (!h) continue;
    h->Sumw2();   

    //cout << "Maximum of before scaling " << h->GetMaximum() << endl;
    if (i == 1) nentriesdata = h->Integral("width");
    else h->Scale(nentriesdata/h->Integral("width"));
    //if (i == 0) h->Scale(0.896042);
    //if (i == 1) h->Scale(1130*71.163*(5153./10000)*(1./101967.));
    //h->Scale(1.0/h->Integral("width"));

    if (h->GetMaximum() > hmax) hmax = h->GetMaximum();
    
    string option=("E");//("E");
    //if (!i) {
    if (i==1) {
      h->SetTitle("");
      //h->SetTitle(h_title);
      h->SetTitleSize(0.040);
      h->SetTitleOffset(0.105);
      if (tokens.size() > 2) h->GetXaxis()->SetTitle(tokens[2].c_str());
      if (tokens.size() > 3) h->GetYaxis()->SetTitle(tokens[3].c_str());
      h->GetXaxis()->SetLabelSize(0.032);
      h->GetYaxis()->SetLabelSize(0.032);
      h->GetXaxis()->SetTitleSize(0.045);
      h->GetYaxis()->SetTitleSize(0.05);
      h->GetXaxis()->SetTitleOffset(1.0);
      h->GetYaxis()->SetTitleOffset(0.82);
      //h->SetMinimum(0.0);

      h->SetMarkerSize(1.);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);

      TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
      hstats->SetTextColor(i+1);
      hstats->SetOptStat(1111);
      hstats->Draw();
      h->GetListOfFunctions()->Add(hstats);
      hstats->SetParent(h);
      
    }
    else {
      option = "HISTSAMES";
      h->SetLineWidth(2);
      h->SetLineStyle(1);
      h->SetLineColor(i+1);

      TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
      hstats->SetTextColor(i+1);
      hstats->SetOptStat(1111);
      hstats->Draw();
      h->GetListOfFunctions()->Add(hstats);
      hstats->SetParent(h);

      }*/
    
    /*
    if (i==1){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);
      h->SetLineColor(1);
      }
    if (i==2){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(22);
      h->SetMarkerColor(3);
      h->SetLineColor(3);
    }
    
    else if (i==3){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(22);
      h->SetMarkerColor(2);
      h->SetLineColor(2);
      }*/
  /*    if (i==4){
      h->SetMarkerSize(1.);
      h->SetMarkerStyle(26);
      h->SetMarkerColor(4);
      h->SetLineColor(4);
    }
    
    
    h->Draw(option.c_str());
    //hstats->Draw();
    legend12->AddEntry(h, lglist[i].c_str(), ((i ==4 ) ? "L" : "P"));
    //legend12->AddEntry(h, lglist[i].c_str(), "P");
    legend12->SetTextSize(0.04);
    legend12->SetBorderSize(0);
    legend12->SetFillStyle(0);
  }

  // correct maximum height
  //TFile* f = v.at(0);
  TFile* f_eoyd = v.at(1);
  //f->cd(folder.c_str());
  f_eoyd->cd(datafolder.c_str());
  
  //  TH1F *h = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
  //assert(h);

  //double fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  //h->SetMaximum(fct * hmax);

  //f_eoyd->cd("DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");  
  TH1F *h_eoyd = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
  assert(h_eoyd);

  fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  h_eoyd->SetMaximum(fct * hmax);

  textonplot(0.19, 0.80, 0.40, 0.87, 132, yearinfo);
  textonplot(0.63, 0.918, 0.80, 1.0, 22, luminfo);
  textonplot(0.10, 0.918, 0.18, 1.0, 22, "CMS");
  textonplot(0.185, 0.908, 0.35, 1.0, 12, "Preliminary");

  if (tokens.size() > 5 && tokens[5] == "log") pad12->SetLogx();
  if (tokens.size() > 6 && tokens[6] == "log") pad12->SetLogy();
  legend12->Draw();
  //tText12->Draw();
  //tText22->Draw();
  pad12->Update();
  pad12->Modified();
  
  std::cout << "pad12 drawn successfully" << std::endl; 
  */

  canvas->cd();
  TPad* pad21 = new TPad("pad21","pad21",0,0.01,1.0,0.30);
  //pad2->SetTopMargin(0);
  pad21->SetTickx();
  pad21->SetTicky();
  pad21->SetFrameLineWidth(3);
  pad21->Draw();
  pad21->cd();

  TLegend *legend21 = new TLegend(0.87,0.78,0.95,0.90);                  //0.42,0.60,0.62,0.95);
  TAxis* xaxis = h->GetXaxis();
  int nbinsx = h->GetNbinsX();
  double xmin = xaxis->GetXmin();
  double xmax = xaxis->GetXmax();

  if (hname == "iperr3dToPV") {
    xmin = 0.0;
    xmax = 1.0;
  }
  if (hname == "sip3dToPV") {
    xmin = -5.;
    xmax =  5.;
  }
  if ( hname == "trackChi2bynDOF") {
    xmin = 0.0;
    xmax = 5.0;
  }

  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineWidth(3);
  line->SetLineStyle(9);
  
  //TRatioPlot *hratio;
  hmax = -1;
  double hmin = 999;
  vector<TH1F*> histv;
  //vector<TGraphAsymmErrors*> histv;
  char nhname[32];
  //TLegend *legend1 = new TLegend(0.55,0.99-lglist.size()*0.07,0.95,0.99);
	std::cout<<v.size()<<"\n";
  for (uint i = 2; i < v.size(); ++i) {
    if (i!= 2) continue;
    TFile* file = v[i];
    //file->cd(folder.c_str());
    //here i=0 will never be opened    //if (i==0) f->cd("DQMData/Run 251143/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==2) file->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/"); 
    //if (i==1) file->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TH1F *h2 = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
    if (!h2) continue;

    sprintf(nhname, "hn_%d", i);
    TH1F* hn = new TH1F(*h2);
    TH1F* hmc = new TH1F(*h2);
    hn->SetStats(0);
    //    hmc->Sumw2();
    hmc->SetStats(0);
    //TGraphAsymmErrors* hn;
    //    hratio = new TRatioPlot(h, h2);
    if (i==2 ) hn->Divide(h, h2,1,1,"B");
    if (i==2 ) hmc->Divide(h2, h2,1,1,"B");

    hn->Sumw2();
    hmc->Sumw2();
    //if (i==2 ) hn->Divide(h, h2,"pois midp");
    //if (i==2 ) hn->Divide(h2, h,1,1,"B"); // UL MC for now //& EOY MC , PU wrt UL data
    //   if (i==4) hn->Divide(h2, h_eoyd,1,1,"B"); // EOY MC,  PU wrt eoy data
    
    histv.push_back(hn);
    histv.push_back(hmc);
 
    if (hn->GetMaximum() > hmax) hmax = hn->GetMaximum();
    if (hn->GetMinimum() < hmin) hmin = hn->GetMinimum();

    

    //    hn->SetMarkerSize(1.);
    //hn->SetMarkerStyle(20);
    //hn->SetMarkerColor(i+1);

    hn->SetMarkerSize(2.0);
    hn->SetMarkerStyle(20);
    hn->SetMarkerColor(2);
    hn->SetLineWidth(3);

    
    hmc->SetMarkerColor(1);
    hmc->SetLineWidth(3);
    hmc->SetLineColor(1);
    hmc->SetMarkerStyle(22);
    hmc->SetMarkerSize(2.0);
    hmc->SetLineStyle(9);
    //    hmc->SetFillStyle(3005);
    //hmc->SetFillColor(kGray+3);

    hmc->SetTitle("");   
    //cout << ">>>pad2 i=" << i << endl;
    //string lineoption("ESAME");
    string option("E");
    if (i==2) {
      //      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hmc->GetYaxis()->SetTitle("Data/MC");
      hmc->GetXaxis()->SetLabelSize(0.12);
      hmc->GetYaxis()->SetLabelSize(0.12);
      hmc->GetXaxis()->SetTitleSize(0.12);
      hmc->GetYaxis()->SetTitleSize(0.15);
      hmc->GetYaxis()->SetTitleOffset(0.30);
      hmc->GetXaxis()->SetTitleOffset(0.30);
      

      //      hmc->GetXaxis()->SetLabelOffset(0.0005);
      //hn->SetMarkerColor(2); // data UL/MC UL (dark green points)
    }
    /*
    else if (i==4){
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(MC/Data)");
      hn->GetXaxis()->SetLabelSize(0.1);
      hn->GetYaxis()->SetLabelSize(0.1);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      hn->SetMarkerColor(12); // data EOY/MC EOY (dark gray points)
      option = "PSAME";
    }
    else if (i==3){
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(MC/Data)");
      hn->GetXaxis()->SetLabelSize(0.1);
      hn->GetYaxis()->SetLabelSize(0.1);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      hn->SetMarkerColor(38); //data UL / MC EOY (dark blue
      option = "PSAME";
      }*/
    else {
      option = "ESAME";
    }
    //    line->Draw();
    hmc->Draw(option.c_str());
    ////hn->Draw(option.c_str());
    hn->Draw("ESAME");
    //    hratio->Draw("SAME");
    //    legend2->AddEntry(h, lglist[i].c_str(), ((i > 0) ? "L" : "P"));
    legend21->AddEntry(hn, lglist[i].c_str(), "P");
    legend21->SetTextSize(0.08);

    //pad21->Update();
    //pad21->Modified();

  }
  // correct maximum height
	std::cout<<"AAAA\n";
  TH1F* hn = histv.at(1);
  
  if (hn) {
    hn->SetMaximum(1.9);
    double ermax = -1.0;
    for (int k = 1; k < nbinsx; ++k) {
      if (hn->GetBinError(k) > ermax) ermax = hn->GetBinError(k);
    }
    fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
    hmin = (hmin > 0 ) ? 0.9 * hmin : 0.0;
    hn->SetMinimum(hmin);
    }
  double hmaxval, hminval;
  //  if (hname == "iperr3dToPV" ) {
  // hmaxval = 
  //  } 
  hn->GetYaxis()->SetLabelSize(0.1);
  hn->GetXaxis()->SetLabelSize(0.1);
  hn->GetYaxis()->SetTitleSize(0.15);
  hn->GetXaxis()->SetTitleSize(0.15);
  hn->SetMaximum(1.6);
  hn->SetMinimum(0.4);
  hn->GetXaxis()->SetTickSize(0.08);
  hn->GetYaxis()->SetNdivisions(208);
      
  if (tokens.size() > 5 && tokens[5] == "log") pad21->SetLogx();
  //if (tokens.size() > 6 && tokens[6] == "log") pad2->SetLogy();
  //legend2->Draw();
  pad21->Update();
  pad21->Modified();

  std::cout << "pad21 drawn successfully" << std::endl;

  /*
  canvas->cd();
  TPad* pad22 = new TPad("pad22","pad22",0.5,0.25,1,0.50);
  //pad2->SetTopMargin(0);
  pad22->Draw();
  pad22->cd();
  
  

  TLegend *legend22 = new TLegend(0.42,0.60,0.62,0.95);
  xaxis = h_eoyd->GetXaxis();
  nbinsx = h_eoyd->GetNbinsX();
  xmin = xaxis->GetXmin();
  xmax = xaxis->GetXmax();

  hmax = -1;
  hmin = 999;
  vector<TH1F*> histv_eoy;
  char nhname_eoy[32];
  //TLegend *legend1 = new TLegend(0.55,0.99-lglist.size()*0.07,0.95,0.99);
  for (uint i = 2; i < v.size(); ++i) {
    if (i!=4) continue;
    TFile* file = v[i];
    //file->cd(folder.c_str());
    //here i=0 will never be opened    //if (i==0) f->cd("DQMData/Run 251143/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==4) file->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/"); 
    //if (i==1) file->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TH1F *h2 = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
    if (!h2) continue;

    sprintf(nhname_eoy, "hn_%d", i);
    TH1F* hn = new TH1F(*h2);
    hn->SetStats(0);
    //if (i==2 || i==3) hn->Divide(h2, h,1,1,"B"); // UL MC & EOY MC , PU wrt UL data
    //    if (i==4) hn->Divide(h2, h_eoyd,1,1,"B"); // EOY MC,  PU wrt eoy data
    if (i==4) hn->Divide(h_eoyd, h2,1,1,"B"); // EOY MC,  PU wrt eoy data

    histv_eoy.push_back(hn);
 
    if (hn->GetMaximum() > hmax) hmax = hn->GetMaximum();
    if (hn->GetMinimum() < hmin) hmin = hn->GetMinimum();
 
    //hn->SetMarkerSize(1.);
    //hn->SetMarkerStyle(20);
    //hn->SetMarkerColor(i+1);
    hn->SetMarkerSize(0.7);
    hn->SetMarkerStyle(20);
    hn->SetMarkerColor(4);

    //cout << ">>>pad2 i=" << i << endl;
    string option("P");*/
    /*
    if (i==2) {
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(MC/Data)");
      hn->GetXaxis()->SetLabelSize(0.1);
      hn->GetYaxis()->SetLabelSize(0.1);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      hn->SetMarkerColor(30); // data UL/MC UL (dark green points)
      }*//*
    if (i==4){
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(Data/MC)");
      hn->GetXaxis()->SetLabelSize(0.06);
      hn->GetYaxis()->SetLabelSize(0.06);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      //hn->SetMarkerColor(12); // data EOY/MC EOY (dark gray points)
      option = "PSAME";
      }*/
    /*
    else if (i==3){
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(MC/Data)");
      hn->GetXaxis()->SetLabelSize(0.1);
      hn->GetYaxis()->SetLabelSize(0.1);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      hn->SetMarkerColor(38); //data UL / MC EOY (dark blue
      option = "PSAME";
      }*//*
    else {
      option = "PSAME";
    }
    hn->SetTitle("");   
    hn->Draw(option.c_str());
    TLine *line = new TLine(xmin,1,xmax,1);
    line->Draw();

    //    legend2->AddEntry(h, lglist[i].c_str(), ((i > 0) ? "L" : "P"));
    legend22->AddEntry(hn, lglist[i].c_str(), "P");
    legend22->SetTextSize(0.04);

  }
  // correct maximum height
  TH1F* hn_eoy = histv_eoy.at(0);
  if (hn_eoy) {
    hn_eoy->SetMaximum(1.9);
    double ermax = -1.0;
    for (int k = 1; k < nbinsx; ++k) {
      if (hn_eoy->GetBinError(k) > ermax) ermax = hn_eoy->GetBinError(k);
    }
    fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
    hmin = (hmin > 0 ) ? 0.9 * hmin : 0.0;
    hn_eoy->SetMinimum(hmin);
  }

  if (tokens.size() > 5 && tokens[5] == "log") pad22->SetLogx();
  //if (tokens.size() > 6 && tokens[6] == "log") pad2->SetLogy();
  //legend22->Draw();
  pad22->Update();
  pad22->Modified();

  std::cout << "pad22 drawn successfully" << std::endl;

  //// pad 3 (UL/EOY)

  canvas->cd();
  TPad* pad3 = new TPad("pad3","pad3",0,0.,1,0.25);
  //pad3->SetTopMargin(0);
  pad3->Draw();
  pad3->cd();

  TLegend *legend3 = new TLegend(0.70,0.70,0.90,0.90);
  xaxis = h->GetXaxis();
  nbinsx = h->GetNbinsX();
  xmin = xaxis->GetXmin();
  xmax = xaxis->GetXmax();

  TFile* f_ulm = v.at(2);
  //f->cd(folder.c_str());                                                                                                                           
  f_ulm->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");

  TH1F *h_ulm = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
  assert(h_ulm);

  //fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  //h_ulm->SetMaximum(fct * hmax);

  vector<string> lglist_new;
  lglist_new.push_back("a0");
  lglist_new.push_back("data UL/data EOY");
  lglist_new.push_back("a2");
  lglist_new.push_back("a3");
  lglist_new.push_back("MC UL/MC EOY");

  hmax = -1;
  hmin = 999;
  vector<TH1F*> histv3;
  //nhname[32];
  //TLegend *legend1 = new TLegend(0.55,0.99-lglist.size()*0.07,0.95,0.99);
  for (uint i = 0; i < v.size(); ++i) {
    TFile* file = v[i];
    if (!(i==1 || i==4)) continue;
    //file->cd(folder.c_str());
    //here i=0 will never be opened    //if (i==0) f->cd("DQMData/Run 251143/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==1) file->cd(datafolder.c_str());
    if (i==4) file->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    //if (i==1) file->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TH1F *h2 = dynamic_cast<TH1F*>(gDirectory->Get(hname.c_str()));
    if (!h2) continue;

    sprintf(nhname, "hn_%d", i);
    TH1F* hn = new TH1F(*h2);
    hn->SetStats(0);
    if (i==1)hn->Divide(h, h2,1,1,"B"); // data EOY/data UL
    if (i==4)hn->Divide(h_ulm, h2,1,1,"B"); // MC EOY/MC UL
    //    if (i==1)hn->Divide(h2, h,1,1,"B"); // data EOY/data UL
    //if (i==4)hn->Divide(h2, h_ulm,1,1,"B"); // MC EOY/MC UL

    histv3.push_back(hn);
 
    if (hn->GetMaximum() > hmax) hmax = hn->GetMaximum();
    if (hn->GetMinimum() < hmin) hmin = hn->GetMinimum();
 
    hn->SetMarkerSize(0.5);
    hn->SetMarkerStyle(20);
    //    hn->SetMarkerColor(i+1);
    //hn->SetLineColor()

    //cout << ">>>pad2 i=" << i << endl;
    string option("P");
    if (i==1) {
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(UL/EOY)");
      hn->GetXaxis()->SetLabelSize(0.08);
      hn->GetYaxis()->SetLabelSize(0.08);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      hn->SetMarkerColor(1);
      hn->SetLineColor(1);
      }
      else if (i==4) {
      if (tokens.size() > 2) hn->GetXaxis()->SetTitle(tokens[2].c_str());
      hn->GetYaxis()->SetTitle("Ratio(UL/EOY)");
      hn->GetXaxis()->SetLabelSize(0.08);
      hn->GetYaxis()->SetLabelSize(0.08);
      hn->GetXaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleSize(0.1);
      hn->GetYaxis()->SetTitleOffset(0.42);
      hn->SetMarkerColor(3);
      hn->SetLineColor(3);
      option = "PSAME";
      }
      else {
      option = "PSAME";
      }
      hn->SetTitle("");   
      hn->Draw(option.c_str());
      TLine *line = new TLine(xmin,1,xmax,1);
      line->Draw();
      
      //    legend3->AddEntry(hn, lglist[i].c_str(), "P");
      legend3->AddEntry(hn, lglist_new[i].c_str(), "P");
      legend3->SetTextSize(0.08);
      legend3->SetFillStyle(0);
      legend3->SetBorderSize(0);
      }
      // correct maximum height
      TH1F* hn3 = histv3.at(0);
      if (hn3) {
      hn3->SetMaximum(1.9);
      double ermax = -1.0;
      for (int k = 1; k < nbinsx; ++k) {
      if (hn3->GetBinError(k) > ermax) ermax = hn3->GetBinError(k);
      }
      fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
      hmin = (hmin > 0 ) ? 0.9 * hmin : 0.0;
      hn3->SetMinimum(hmin);
      }
      
      if (tokens.size() > 5 && tokens[5] == "log") pad3->SetLogx();
      //if (tokens.size() > 6 && tokens[6] == "log") pad3->SetLogy();
      legend3->Draw();
      pad3->Update();
      pad3->Modified();
      
      std::cout << "pad3 drawn successfully" << std::endl;
	 */
  canvas->cd();
  canvas->Update();
  canvas->Modified();
  
  char fname[256];
    sprintf(fname, "%s.png", hname.c_str());
  canvas->Print(fname);
    sprintf(fname, "%s.pdf", hname.c_str());
  canvas->Print(fname);
  cout << "Histo " << fname << " printed successfully" << endl;
  canvas->Clear();
}
//void openFiles(vector<TFile*>& v1, vector<string>& v2, const char* filename) {
void openFiles(vector<TFile*>& v1, vector<string>& v2, const char* filename, const string& yr ) {
  static const int BUF_SIZE = 300;

  //  if (yr == "2016pre") filename = "filelist_align_16pre.txt";
  if (yr == "2016pre") filename = "filelist_align_16pre_sm.txt";
  else if (yr == "2016post") filename = "filelist_align_16post.txt";
  //else if (yr == "2017") filename = "filelist_align_17.txt";
  else if (yr == "2017") filename = "filelist13p6TeV_ZeroBias_Lambda.txt";
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
  static const int BUF_SIZE = 300;

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
