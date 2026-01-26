#ifndef comparisons_utils_run3_h
#define comparisons_utils_run3_h

#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TPaveStats.h"
#include "CMS_lumi.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <regex>

using namespace std;
typedef unsigned int uint;

void tokenize(const string& str, vector<string>& tokens, const string& delimiters=" ");

vector<pair<TFile*, string>> openFiles(string& filename, const string& analysis);

void closeFiles(vector<pair<TFile*, string>>& v);

vector<string> readHistograms(const string& hists_fname);

void textonplot(double x1, double y1, double x2, double y2, double txtfont, double txtsize, TString s);

void rmdot(double x1, double y1, double x2, double y2, double txtfont, TString s);

string parseEtaCut(const string& input);

pair<bool, int> getStyleInfo(const int& cmpType, const int& index, const bool& isRatio);

void setRatioPad(TH1* h, const string& xName, const string& yName, const double& rrange_low, const double& rrange_high);

void plotHisto(TH1* h, 
               const string& yName, 
               const bool& isPrimaryHist, 
               const bool& isBullet, 
               const int& colorNum);

void fit_and_plot_VertexZpos(vector<pair<TH1*, bool>> fitsVtxZ_info, TLegend* legend);

void compareHisto(TCanvas* canvas, 
                  const vector<pair<TFile*, string>>& data_list, 
                  const string& analysis_folder,
                  const bool& cmpData, 
                  const bool& profiles,
                  const vector<string>& hist_tokens,
                  const vector<string>& print_info, 
                  const vector<double>& rrange,
                  const string& outfolder); 


#endif