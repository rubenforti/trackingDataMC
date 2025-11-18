
//#ifndef __CINT__
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TProfile.h"
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
void readHistograms(vector<string>& v, const char* filename="lambdaProfile_run3_monitoring.txt");
void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, const string& yr); 
void compareHisto(const vector<TFile*>& v, const char* folder, const char* hname); 
void compareprofile_run3(string year) {
  vector<TFile*> filelist;
  vector<string> lglist;
  const char* filename;
  openFiles(filelist, lglist, filename, year);
  cout << ">>> # of Root files: " << filelist.size() << endl;

  vector<string> hlist;
  readHistograms(hlist);
  cout << ">>> # of histograms: " << hlist.size() << endl;
  TCanvas* canvas = new TCanvas("canvas", "canvas");
  canvas->SetCanvasSize(800, 800);

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
    if (i==0) f->cd("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==1) f->cd("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==2 || i==3 || i==4) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");

//    if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    TProfile *h = dynamic_cast<TProfile*>(gDirectory->Get(hname));
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
    luminfo    = "#sqrt{s} = 13 TeV, L = 0.296 fb^{-1}";
  }
  else if (yr == "2016post") {
    datafolder = "DQMData/Run 278808/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "2016";
    luminfo    = "#sqrt{s} = 13 TeV, L = 0.280 fb^{-1}";
  }
  else if (yr == "2017") {
    datafolder = "DQMData/Run 999999/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "#Lambda (ZeroBias)";
    luminfo    = "13.6 TeV"; //Modify to show the correct integrated luminosity
  }
  else if (yr == "2018") {
    datafolder = "DQMData/Run 317626/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/";
    yearinfo   = "2018";
    luminfo    = "#sqrt{s} = 13 TeV, L = 0.527 fb^{-1}";
  }


  string folder(tokens[0]);
  string hname(tokens[1]);

  /*
  TPaveText* tText1 = new TPaveText(0.65, 0.90, 0.80, 0.96, "brNDC");
  tText1->SetBorderSize(0);
  tText1->SetFillColor(0);
  //tText1->SetFillStyle(0);
  TText *t1 = tText1->AddText("#sqrt{s} = 13 TeV, L = 0.296 fb^{-1}");
  tText1->SetTextSize(0.03);
  tText1->Draw(); 
  //==========================================================
  TPaveText* tText2 = new TPaveText(0.15, 0.90, 0.27, 0.96, "brNDC");
  tText2->SetBorderSize(0);
  tText2->SetFillColor(0);
  //tText2->SetFillStyle(0);
  TText *t2 = tText2->AddText("CMS #it{Preliminary}");
  tText2->SetTextSize(0.03);
  tText2->Draw();
  */
  //TPaveText* tText3 = new TPaveText(0.13, 0.92, 0.25, 0.98, "brNDC");
  //tText2->SetBorderSize(0);
  //tText2->SetFillColor(0);
  //  TText *t3 = tText3->AddText("Preliminary");
  //tText3->SetTextSize(0.03);
  //float extraTextFont = 52;

  canvas->cd();
  //  CMS_lumi();

//  TString h_title = "Run 275376 : " + tokens[3] + " Vs " + tokens[2];
// TString h_title = "2016B_preVFP Run 278509 : " + tokens[3] + " Vs " + tokens[2];

  TPad* pad1 = new TPad("pad1", "pad1",0,0,1,1);
  pad1->Draw();
  pad1->cd();

  TLegend *legend1 = new TLegend(0.40,0.83,0.60,0.90);

  double hmax = -1;
  double nentriesdata = 0;
  //std::cout << "v size : " << v.size() << std::endl;
  for (uint i = 0; i < v.size(); ++i) {
    if (!(i==0 || i==2)) continue;
    TFile* f = v[i];
    if (i==0 ) f->cd(datafolder.c_str());
    //if ( i==1) f->cd("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==2) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");

  //  if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    //f->cd(folder.c_str());
    TProfile *h = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));
    //TH1* h = gDirectory->Get(hname.c_str());
    if (!h) continue;


    //if (i == 0) nentriesdata = h->Integral("width");
    //else h->(nentriesdata/h->Integral("width"));
    //if (i == 0) h->Scale(0.896042);
    //if (i == 1) h->Scale(1130*71.163*1.05*(5153./10000)*(1./101967));
    //cout << "i = " << i << ", Integral = " << h->Integral() << endl;
    if (h->GetMaximum() > hmax) hmax = h->GetMaximum();
    
    string option=("E");
    if (i==0) {
      h->SetTitle("");
      //h->SetTitle(h_title);
      h->SetTitleSize(0.03);
      h->SetTitleOffset(0.80);
      if (tokens.size() > 2) h->GetXaxis()->SetTitle(tokens[2].c_str());
      if (tokens.size() > 3) h->GetYaxis()->SetTitle(tokens[3].c_str());
      h->GetXaxis()->SetLabelSize(0.018);
      h->GetYaxis()->SetLabelSize(0.018);
      h->GetXaxis()->SetTitleSize(0.03);
      h->GetYaxis()->SetTitleSize(0.03);
      h->GetXaxis()->SetTitleOffset(1.25);
      h->GetYaxis()->SetTitleOffset(1.25);
      h->SetMinimum(0.0);

      h->SetMarkerSize(0.8);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);

      TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
      hstats->SetTextColor(i+1);
      hstats->SetOptStat(1111);
      hstats->Draw();
      h->GetListOfFunctions()->Add(hstats);
      hstats->SetParent(h);
    }
    else if (i==2){
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
    }

    /*    if (i==1){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);
      h->SetLineColor(1);
      }*/
    if (i==2){
      h->SetMarkerSize(0.8);
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


    h->Draw(option.c_str());
    legend1->AddEntry(h, lglist[i].c_str(), ((i == 2) ? "L" : "P"));
    legend1->SetTextSize(0.028);
    legend1->SetBorderSize(0);
    legend1->SetFillStyle(0);

  }
  // correct maximum height
  TFile* f = v.at(0);
  f->cd(datafolder.c_str());
  //f->cd(folder.c_str());
  TProfile *h = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));
  assert(h);

  double fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  h->SetMaximum(fct * hmax);
  

    if (yr == "2016pre" || yr == "2016post")  textonplot(0.27, 0.75, 0.40, 0.83, 132, yearinfo);
  else textonplot(0.17, 0.80, 0.29, 0.87, 132, yearinfo);
  textonplot(0.63, 0.918, 0.80, 1.0, 22, luminfo);
  textonplot(0.10, 0.918, 0.18, 1.0, 22, "CMS");
  textonplot(0.22, 0.908, 0.39, 1.0, 12, "             Work in progress");
  rmdot(0.98, 0.97, 1.0, 1.0, 52, "             ");

  if (tokens.size() > 5 && tokens[5] == "log") pad1->SetLogx();
  if (tokens.size() > 6 && tokens[6] == "log") pad1->SetLogy();
  legend1->Draw();
  //  tText1->Draw();
  //tText2->Draw();
  pad1->Update();
  pad1->Modified();
  
  std::cout << "pad1 drawn successfully" << std::endl; 

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

#if 0
  canvas->cd();


  //TPaveText* tText3 = new TPaveText(0.13, 0.92, 0.25, 0.98, "brNDC");
  //tText2->SetBorderSize(0);
  //tText2->SetFillColor(0);
  //  TText *t3 = tText3->AddText("Preliminary");
  //tText3->SetTextSize(0.03);
  //float extraTextFont = 52;


  //  CMS_lumi();
//  TString h_title = "Run 275376 : " + tokens[3] + " Vs " + tokens[2];
// TString h_title = "2016B_preVFP Run 278509 : " + tokens[3] + " Vs " + tokens[2];

  TPad* pad12 = new TPad("pad12", "pad12",0.5,0,1,1);
  pad12->Draw();
  pad12->cd();

  TLegend *legend2 = new TLegend(0.70,0.83,0.85,0.90);

  hmax = -1;
  nentriesdata = 0;
  //std::cout << "v size : " << v.size() << std::endl;
  for (uint i = 0; i < v.size(); ++i) {
    if (!(i==1 || i==4)) continue;
    TFile* f = v[i];
    if (i==1 ) f->cd(datafolder.c_str());
    //if ( i==1) f->cd("DQMData/Run 278509/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    if (i==4) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");

  //  if (i==1) f->cd("DQMData/Run 1/StandaloneTrackMonitor/Run summary/V0Monitoring/Lambda/");
    //f->cd(folder.c_str());
    TProfile *h = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));
    //TH1* h = gDirectory->Get(hname.c_str());
    if (!h) continue;


    //if (i == 0) nentriesdata = h->Integral("width");
    //else h->(nentriesdata/h->Integral("width"));
    //if (i == 0) h->Scale(0.896042);
    //if (i == 1) h->Scale(1130*71.163*1.05*(5153./10000)*(1./101967));
    //cout << "i = " << i << ", Integral = " << h->Integral() << endl;
    if (h->GetMaximum() > hmax) hmax = h->GetMaximum();
    
    string option=("E");
    if (i==1) {
      h->SetTitle("");
      //h->SetTitle(h_title);
      h->SetTitleSize(0.03);
      h->SetTitleOffset(0.80);
      if (tokens.size() > 2) h->GetXaxis()->SetTitle(tokens[2].c_str());
      if (tokens.size() > 3) h->GetYaxis()->SetTitle(tokens[3].c_str());
      h->GetXaxis()->SetLabelSize(0.018);
      h->GetYaxis()->SetLabelSize(0.018);
      h->GetXaxis()->SetTitleSize(0.03);
      h->GetYaxis()->SetTitleSize(0.03);
      h->GetXaxis()->SetTitleOffset(1.25);
      h->GetYaxis()->SetTitleOffset(1.25);
      h->SetMinimum(0.0);

      h->SetMarkerSize(0.8);
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
    }

    /*    if (i==1){
      h->SetMarkerSize(1.3);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(1);
      h->SetLineColor(1);
      }*/
    if (i==4){
      h->SetMarkerSize(0.8);
      h->SetMarkerStyle(22);
      h->SetMarkerColor(4);
      h->SetLineColor(4);
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

    h->Draw(option.c_str());
    legend2->AddEntry(h, lglist[i].c_str(), ((i == 4) ? "L" : "P"));
    legend2->SetTextSize(0.025);
    legend2->SetBorderSize(0);
    legend2->SetFillStyle(0);

  }
  // correct maximum height
  TFile* f_eoyd = v.at(1);
  f_eoyd->cd(datafolder.c_str());
  //f->cd(folder.c_str());
  TProfile *h_eoy = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));
  assert(h_eoy);

  fct = (tokens.size() > 6 && tokens[6] == "log") ? 6 : 1.25;
  h_eoy->SetMaximum(fct * hmax);
  
  if (yr == "2016pre" || yr == "2016post") textonplot(0.19, 0.75, 0.40, 0.83, 132, "2016");
  textonplot(0.61, 0.918, 0.78, 1.0, 22, luminfo);
  textonplot(0.10, 0.918, 0.18, 1.0, 22, "CMS");
  textonplot(0.22, 0.908, 0.39, 1.0, 12, "Preliminary");

  if (tokens.size() > 5 && tokens[5] == "log") pad12->SetLogx();
  if (tokens.size() > 6 && tokens[6] == "log") pad12->SetLogy();
  legend2->Draw();
  //  tText11->Draw();
  //tText12->Draw();
  pad12->Update();
  pad12->Modified();
  
  std::cout << "pad12 drawn successfully" << std::endl; 
#endif
  
  canvas->cd();
  canvas->Update();
  canvas->Modified();
  char fname[256];
  sprintf(fname, "%s.png", hname.c_str());
  canvas->Print(fname);
  sprintf(fname, "%s.pdf", hname.c_str());
  canvas->Print(fname);
  cout << "Profile printed successfully" << endl;
  canvas->Clear();
  return;
}
void openFiles(vector<TFile*>& v1, vector<string>& v2, const char* filename, const string& yr) {
  static const int BUF_SIZE = 512;

  if (yr == "2016pre") filename = "filelist_align_16pre.txt";
  else if (yr == "2016post") filename = "filelist_align_16post.txt";
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
