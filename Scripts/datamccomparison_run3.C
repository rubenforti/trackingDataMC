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
#include <regex>

using namespace std;
typedef unsigned int uint;

void tokenize(const string& str, vector<string>& tokens, const string& delimiters=" ");
void openFiles(vector<TFile*>& v, vector<string>& l, string& f, const string& analysis);
void closeFiles(vector<TFile*>& v);
void readHistograms(vector<string>& v, const string& hists_fname);
void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, const bool& profiles, const string& analysis, const string& yr, const bool& cmpData, const string& outfolder, const string& folder_postfix); 
void textonplot(double x1, double y1, double x2, double y2, double txtfont, double txtsize, TString s);
void rmdot(double x1, double y1, double x2, double y2, double txtfont, TString s);
void parseEtaCut(const string& input);

void datamccomparison_run3(string analysis, string year="",
                           bool cmpData=false, bool doProfile=false, string folder_postfix="",
                           string altFileName="", string outfolder="./") {
    
    cout << "Doing comparisons for " << analysis.c_str()  << " " << year.c_str() << " analysis" << endl;

    vector<TFile*> filelist;
    vector<string> lglist;
    
    string filename = (altFileName.empty()) ? "" : altFileName;
    
    openFiles(filelist, lglist, filename, analysis); 
    cout << ">>> # of Root files: " << filelist.size() << endl;

    if (filelist.size() > 3) {
        cout << ">>> Max 3 files are allowed for comparison" << endl;
        return;
    }
    
    vector<string> hlist_compare, hlist_profile;
    string comparison_hfname, profile_hfname, scatter_hfname, analysis_folder, analysis_info;

    if (analysis == "ZeroBias") {
        comparison_hfname = "highpurityHistolist_run3.txt";
        profile_hfname = "highpurityProfile_run3.txt";
        scatter_hfname = "highpurityScatter_run3.txt";
        analysis_folder = "highPurityTracks/"+folder_postfix;
        anaysis_info = "ZeroBias";
    }
    else if (analysis == "ZEE") {
        comparison_hfname = "ElectronHistolist_run3.txt";
        profile_hfname = "ElectronProfile_run3.txt";
        analysis_folder = "ElectronTracks/"+folder_postfix;
        analysis_info = "Z#rightarrowee";
    }
    else if (analysis == "ZMM") {
        comparison_hfname = "MuonHistolist_run3.txt";
        profile_hfname = "MuonProfile_run3.txt";
        analysis_folder = "MuonTracks/"+folder_postfix;
        analysis_info = "Z#rightarrow#mu#mu";
    }
    else if (analysis == "ZeroBias_K0") {
        comparison_hfname = "highpurity_K0_Histolist_run3.txt";
        profile_hfname = "highpurity_K0_Profile_run3.txt";
    }
    else if (analysis == "ZeroBias_Lambda") {
        comparison_hfname = "highpurity_Lambda_Histolist_run3.txt";
        profile_hfname = "highpurity_Lambda_Profile_run3.txt";
    }
    else {
        cout << ">>> Analysis " << analysis.c_str() << " not recognized!" << endl;
        return;
    }

    readHistograms(hlist_compare, comparison_hfname);
    cout << ">>> # of histograms compare: " << hlist_compare.size() << endl;

    string yearinfo, luminfo;

    if (year == "") {
        yearinfo   = ""; 
        luminfo    = "13.6 TeV";
    } 
    else if (year == "2024C") {
        yearinfo   = ""; 
        luminfo    = "6.86 fb^{-1} (2024, 13.6 TeV)";
    }
    else if (year == "2024F") {
        yearinfo   = ""; 
        luminfo    = "27.8 fb^{-1} (2024, 13.6 TeV)";
    }
    else if (year == "2024F-before") {
        yearinfo   = ""; 
        luminfo    = "9.17 fb^{-1} (2024, 13.6 TeV)";
    }
    else if (year == "2024F-after") {
        yearinfo   = ""; 
        luminfo    = "18.7 fb^{-1} (2024, 13.6 TeV)";
    }
    else if (year == "2024I") {
        yearinfo   = ""; 
        luminfo    = "11.5 fb^{-1} (2024, 13.6 TeV)";
    }

    
    TCanvas* canvas = new TCanvas("canvas", "canvas");
    canvas->SetCanvasSize(800, 800);
    canvas->SetLeftMargin(0.11);

    for (uint i = 0; i < hlist_compare.size(); ++i) {
        string line(hlist_compare[i]);
        vector<string> tokens;
        tokenize(line, tokens, ":");
        assert(tokens.size() > 1);
        compareHisto(canvas, filelist, lglist, tokens, analysis_folder, analysis_info, 
                     cmpData, false, yearinfo, luminfo, outfolder);
    }
    
    if (doProfile) {
        readHistograms(hlist_profile, profile_hfname);
        cout << ">>> # of histograms profile: " << hlist_profile.size() << endl;
        
        for (uint i = 0; i < hlist_profile.size(); ++i) {
            string line(hlist_profile[i]);
            vector<string> tokens;
            tokenize(line, tokens, ":");
            assert(tokens.size() > 1);
            compareHisto(canvas, filelist, lglist, tokens, analysis_folder, analysis_info, 
                         cmpData, true, yearinfo, luminfo, outfolder);
        }
    }

    closeFiles(filelist);
}

void compareHisto(TCanvas* canvas, const vector<TFile*>& v, const vector<string>& lglist, const vector<string>& tokens, 
                  const string& analysis_folder, const string& analysis_info, const bool& cmpData, const bool& profiles,
                  const string& yearinfo, const string& luminfo, const string& outfolder)
{
    std::cout<<"Compare Histo Start"<<std::endl;
    
    //string folder(tokens[0]);
    string datafolder = "DQMData/Run 999999/StandaloneTrackMonitor/Run summary/" + analysis_folder;
    string mcfolder   = "DQMData/Run 1/StandaloneTrackMonitor/Run summary/" + analysis_folder;
    string hname(tokens[1]);
    
    if (analysis_info=="ZEE" && hname=="ZInvMass") {
        datafolder = "DQMData/Run 999999/ZEEDetails/Run summary/ElectronTracks/";
	    mcfolder = "DQMData/Run 1/ZEEDetails/Run summary/ElectronTracks/";
    }
    
    canvas->cd();

    TString h_title = tokens[2];
    gStyle->SetLineWidth(3);
    gStyle->SetOptStat(0);

    // Main pad
    TPad* pad11 = new TPad("pad11", "pad11", 0, 0.30, 1.0, 1); // 0.27
    pad11->SetBottomMargin(0.01);
    pad11->SetTickx();
    pad11->SetTicky();
    pad11->SetFrameLineWidth(3);
    pad11->Draw();
    pad11->cd();

    TLegend *legend11;
    if (yr=="2016pre") {
        legend11 = new TLegend(0.40, 0.66, 0.70, 0.78);
    }
    else if (yr=="2016post") {
        legend11 = new TLegend(0.40, 0.66, 0.70, 0.78);
    }
    else {
        legend11 = new TLegend(0.63, 0.73, 0.92, 0.85);
    }

    double hmax = -1;
    double nentriesdata = 0;
    string folder_compare = (!cmpData) ? mcfolder : datafolder;

    // DRAWING PLOTS
    for (uint i=0; i<v.size(); ++i) {

        if (!(i==0 || i==1 || i==2)) continue;
        TFile* f = v[i];
        if (i==0) f->cd(datafolder.c_str()); // UL data
        else f->cd(folder_compare.c_str());
        
        TH1 *h = nullptr;
        if (!profiles) {
            h = dynamic_cast<TH1*>(gDirectory->Get(hname.c_str()));
        } else {
            TProfile *h_profile = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));            
            if (h_profile) h = h_profile->ProjectionX((hname+"_px_"+to_string(i)).c_str(), "e");
        }
        if (!h) continue;

        h->Sumw2();  

        // specific options (customize if needed)
        if (hname == "ip3dToPV") {
            h->Rebin(1.); // 5
        }
        if (hname == "iperr3dToPV") {
            h->Rebin(1.); // 5 
            h->GetXaxis()->SetRangeUser(0., 1.0);
        }
        if (hname == "nTracks") {
            h->Rebin(10);
        }
        if (hname == "sip3dToPV") {
            h->Rebin(5);
            h->GetXaxis()->SetRangeUser(-5.0, 5.0);
        }
        if (hname == "DistanceOfClosestApproachToPV") {
            h->Rebin(10);
        }
        if (hname == "sip2dToPV") {
            h->Rebin(10);
        }
        if (hname == "trackChi2bynDOF") {
            h->Rebin(2);
            h->GetXaxis()->SetRangeUser(0., 5.0);;
        }
        if (hname=="DistanceOfClosestApproachToPVZoomed") {
            if (!(analysis=="ZeroBias")) h->Rebin(10);
            h->GetXaxis()->SetRangeUser(-0.02, 0.02);
	}

        
        if (i==0) nentriesdata = h->Integral("width");
        //else if (!cmpData) h->Scale(nentriesdata/h->Iintegral("width"));
        else if (!profiles) h->Scale(nentriesdata/h->Integral("width"));
        
        if (h->GetMaximum() > hmax) hmax = h->GetMaximum();

        h->SetTitle("");
        string option=("E");
        if (i==0) {
            //h->SetTitle(h_title);
            h->SetTitleSize(0.040);
            h->SetTitleOffset(0.105);
            if (tokens.size()>3) h->GetYaxis()->SetTitle(tokens[3].c_str());
            h->GetXaxis()->SetLabelSize(0.045);
            h->GetYaxis()->SetLabelSize(0.045);
            h->GetXaxis()->SetTitleSize(0.05);
            h->GetYaxis()->SetTitleSize(0.05);
            h->GetXaxis()->SetTitleOffset(0.77);
            h->GetYaxis()->SetTitleOffset(0.99);
            if (profiles) {
                h->SetMinimum(0.0);
                h->SetMaximum(1.5*h->GetMaximum());
            }            

            h->SetMarkerSize(2.0);
            h->SetMarkerStyle(20);
            h->SetMarkerColor(1);
            h->SetLineColor(1);
            h->SetLineWidth(3);

            TPaveStats *hstats = new TPaveStats(0.99, 0.99, 0.99, 0.99, "brNDC");
            hstats->SetTextColor(i+1);
            hstats->SetOptStat(1111);
            hstats->Draw();
            h->GetListOfFunctions()->Add(hstats);
            hstats->SetParent(h);
        }
        else if (i==1) {  // MC nominal
            option = "HISTSAMES";
            gPad->RedrawAxis();
            h->SetLineWidth(3);
            h->SetLineStyle(1);
            h->SetLineColor(2);
            h->SetMarkerSize(1.);
            h->SetMarkerStyle(22);
            h->SetMarkerColor(2);

            TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
            hstats->SetTextColor(i+1);
            hstats->SetOptStat(1111);
            hstats->Draw();
            h->GetListOfFunctions()->Add(hstats);
            hstats->SetParent(h);
        }
        else if (i==2) {  // MC alternative, optional
            option = "HISTSAMES";
            gPad->RedrawAxis();
            h->SetLineWidth(3);
            h->SetLineStyle(9);
            h->SetLineColor(806);
            h->SetMarkerSize(1.);
            h->SetMarkerStyle(1);
            h->SetMarkerColor(806);

            TPaveStats *hstats = new TPaveStats(0.99,0.99,0.99,0.99,"brNDC");
            hstats->SetTextColor(i+1);
            hstats->SetOptStat(1111);
            hstats->Draw();
            h->GetListOfFunctions()->Add(hstats);
            hstats->SetParent(h);

        }

        TGaxis::SetExponentOffset(-0.07, 0, "y");
        h->GetYaxis()->SetLabelSize(0.045);
        h->GetXaxis()->SetLabelSize(0.045);
        h->Draw(option.c_str());
        legend11->AddEntry(h, lglist[i].c_str(), ((i==1 || i==2) ? "L" : "PL"));
        legend11->SetTextSize(0.035);
        legend11->SetBorderSize(0);
        legend11->SetFillStyle(0);
    }

    std::cout << hname.c_str() << std::endl;
    std::cout << tokens[2] << " " << tokens[3] << " " << tokens[4] << std::endl;
    std::cout << ((tokens.size()>5) ? tokens[5] : "-") << " " << ((tokens.size()>6) ? tokens[6] : "-") << std::endl;    

    TFile* f = v.at(0);
    f->cd(datafolder.c_str());
    
    TH1 *h = nullptr;
    if (!profiles) { 
        h = dynamic_cast<TH1*>(gDirectory->Get(hname.c_str()));
    } else {
        TProfile *h_profile = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));
        h = h_profile->ProjectionX((hname+"_px_fct").c_str(), "e");
    }    
    assert(h);
    
    double fct = tokens.size()>6 && tokens[6]=="log" ? 6 : 1.25;  // doesn't work on profiles, in these cases maximum is set above
    h->SetMaximum(fct * hmax);
    
    if (!yearinfo.empty()) analysis_info = analysis_info + " - " + yearinfo;

    // Add eta cut info, if any. Assumes folder_postfix starts with "Eta".
    string cut="";
    parseEtaCut(folder_postfix, out_cut);

    textonplot(0.16, 0.83, 0.28, 0.85, 42, 0.058, analysis_info);
    textonplot(0.53, 0.83, 0.28, 0.85, 42, 0.04, out_cut);
    textonplot(0.55, 0.875, 0.84, 1.0, 42, 0.058, luminfo);
    textonplot(0.10, 0.875, 0.18, 1.0, 62, 0.058, "CMS");
    textonplot(0.24, 0.860, 0.34, 1.0, 52, 0.058, "  Preliminary");
    rmdot(0.98, 0.97, 1.0, 1.0, 52, "             ");
    
    if (tokens.size()>5 && tokens[5]=="log") pad11->SetLogx();
    if (tokens.size()>6 && tokens[6]=="log") pad11->SetLogy();
    legend11->Draw();
    pad11->Update();
    pad11->Modified();
    
    std::cout << "pad11 drawn successfully" << std::endl; 

    // Ratio plot pad
    canvas->cd();
    TPad* pad21 = new TPad("pad21","pad21", 0, 0.01, 1.0, 0.30);
    pad21->SetTopMargin(0.05);
    pad21->SetBottomMargin(0.3);
    pad21->SetTickx();
    pad21->SetTicky();
    pad21->SetFrameLineWidth(3);
    pad21->Draw();
    pad21->cd();

    //TLegend *legend21 = new TLegend(0.87, 0.78, 0.95, 0.90); //0.42,0.60,0.62,0.95);
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
    if (hname == "trackChi2bynDOF") {
        xmin = 0.0;
        xmax = 5.0;
    }
    
    
    TLine *line = new TLine(xmin,1,xmax,1);
    line->SetLineWidth(3);
    line->SetLineStyle(9);

    hmax = -1;
    double hmin = 999;
    vector<TH1*> histv;
    bool draw_hline = true; 
    cout << v.size() << endl;

    // DRAWING RATIO
    for (uint i=1; i<v.size(); ++i) {
        if (!(i==1 || i==2)) continue;
	    TFile* file = v[i];
        file->cd(folder_compare.c_str()); 
        
        TH1 *h2 = nullptr;
        if (!profiles) {
            h2 = dynamic_cast<TH1*>(gDirectory->Get(hname.c_str()));
        } else {
            TProfile *h2_profile = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));
            if (h2_profile) h2 = h2_profile->ProjectionX((hname+"_px").c_str(), "e");
        }
        if (!h2) continue;

        if (draw_hline) {
            // draw a black line at y=1 before every other histogram
            TH1* hmc = (TH1*)h2->Clone();
            hmc->SetStats(0);
            hmc->Divide(h2, h2, 1, 1, "B");
            hmc->Sumw2();
            histv.push_back(hmc);
            hmc->SetMarkerSize(2.0);
            hmc->SetMarkerStyle(22);
            hmc->SetMarkerColor(1);
            hmc->SetLineWidth(3);
            hmc->SetLineStyle(1);
            hmc->SetLineColor(1);
            hmc->SetTitle("");
            hmc->GetXaxis()->SetTitle(tokens[2].c_str());
            hmc->GetYaxis()->SetTitle((!cmpData) ? "Data/MC" : "Data1/Data2");
            hmc->GetXaxis()->SetLabelSize(0.12);
            hmc->GetYaxis()->SetLabelSize(0.12);
            hmc->GetXaxis()->SetTitleSize(0.11);
            hmc->GetYaxis()->SetTitleSize(0.13);
            hmc->GetYaxis()->SetTitleOffset(0.41);
            hmc->GetXaxis()->SetTitleOffset(1.15);
            hmc->GetXaxis()->SetLabelOffset(0.03);
            hmc->Draw("E");
	        
            draw_hline = false;
        }

        TH1* hn = (TH1*)h2->Clone();
        hn->SetStats(0);
        hn->Divide(h, h2, 1, 1, "B");
        hn->Sumw2();
        histv.push_back(hn);
        
        cout << "Integrals before ratio: " << h->Integral() << "  " << h2->Integral() << endl;

        if (hn->GetMaximum() > hmax) hmax = hn->GetMaximum();
        if (hn->GetMinimum() < hmin) hmin = hn->GetMinimum();

        if (i==1) {
            hn->SetMarkerSize(2.0);
            hn->SetMarkerStyle(20);
            hn->SetMarkerColor(2);
            hn->SetLineWidth(3);
        }
        else {
            hn->SetMarkerSize(1.0);
            hn->SetMarkerStyle(1);
            hn->SetLineStyle(1);
            hn->SetLineWidth(3);
        }

        string option = (i==1) ? "ESAME" : "HISTSAMES";
        hn->Draw(option.c_str());
    
    }
    // correct maximum height

    TH1* hmc = histv.at(0);
    hmc->SetMaximum(1.6);
    hmc->SetMinimum(0.4);
    hmc->GetXaxis()->SetLabelSize(0.1);
    hmc->GetYaxis()->SetLabelSize(0.1);
    hmc->GetXaxis()->SetTitleSize(0.12);
    hmc->GetYaxis()->SetTitleSize(0.12);
    hmc->GetXaxis()->SetTickSize(0.08);
    hmc->GetYaxis()->SetNdivisions(208);
        
    if (tokens.size()>5 && tokens[5]=="log") pad21->SetLogx();
    pad21->Update();
    pad21->Modified();

    std::cout << "Ratio pad drawn successfully" << std::endl;

    canvas->cd();
    canvas->Update();
    canvas->Modified();
    
    string fname_png = outfolder + "/" + hname + ".png";
    string fname_pdf = outfolder + "/" + hname + ".pdf";

    canvas->Print(fname_png.c_str());
    canvas->Print(fname_pdf.c_str());
    cout << "Histo " << hname.c_str() << " printed successfully" << endl;
    canvas->Clear();
}

void textonplot(double x1, double y1, double x2, double y2, double txtfont, double txtsize=0.058, TString s="")
{
    TPaveText* tText1 = new TPaveText(x1, y1, x2, y2, "brNDC");
    tText1->SetBorderSize(0);
    tText1->SetFillColor(0);
    tText1->SetFillStyle(0);                                                                                                                         
    TText *t1 = tText1->AddText(s);
    tText1->SetTextSize(txtsize); //
    tText1->SetTextFont(txtfont);
    tText1->Draw();
}

void rmdot(double x1, double y1, double x2, double y2, double txtfont, TString s)
{
    TPaveText* tText1 = new TPaveText(x1, y1, x2, y2, "brNDC");
    tText1->SetBorderSize(0);
    tText1->SetFillColor(0);
    tText1->SetFillStyle(1001);
    TText *t1 = tText1->AddText(s);
    tText1->SetTextSize(0.058); //                                                                                                                      
    tText1->SetTextFont(txtfont);
    tText1->Draw();
}

void openFiles(vector<TFile*>& v1, vector<string>& v2, string& filename, const string& analysis)
{
    static const int BUF_SIZE = 512;

    if (filename.empty()) filename = "filelist13p6TeV_" + analysis + ".txt";
    
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

void readHistograms(vector<string>& v, const string& filename) {
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

void closeFiles(vector<TFile*>& v) {
    for (uint i = 0; i < v.size(); ++i) {
        TFile* f = v[i];
        if (f) f->Close();
    }
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

string parseEtaCut(const string& input, string& out_cut) {
    smatch match;

    if (regex_match(input, match, regex(R"(EtaOver([0-9p]+))"))) {
        string val = match[1];
        replace(val.begin(), val.end(), 'p', '.');
        out_cut = "|#eta| > " + val;
    }

    if (regex_match(input, match, regex(R"(Eta([0-9p]+)to([0-9p]+))"))) {
        string val1 = match[1];
        string val2 = match[2];
        replace(val1.begin(), val1.end(), 'p', '.');
        replace(val2.begin(), val2.end(), 'p', '.');
        out_cut = val1 + " < |#eta| < " + val2;
    }

    if (regex_match(input, match, regex(R"(EtaUnder([0-9p]+))"))) {
        string val = match[1];
        replace(val.begin(), val.end(), 'p', '.');
        out_cut = "|#eta| < " + val;
    }

}
