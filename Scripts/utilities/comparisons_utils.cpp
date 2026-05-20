#include "comparisons_utils.h"

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

vector<pair<TFile*, string>> openFiles(string& filename, const string& analysis) {
    static const int BUF_SIZE = 512;

    vector<pair<TFile*, string>> file_list;

    if (filename.empty()) filename = "filelist13p6TeV_" + analysis + ".txt";
    
    // Open the file containing the datacards
    ifstream fin(filename, ios::in);
    if (!fin) {
        cerr << "Input File: " << filename << " could not be opened!" << endl;
        return file_list;
    }
    char buf[BUF_SIZE];
    while (fin.getline(buf, BUF_SIZE, '\n')) {  // Pops off the newline character
        string line(buf);
        if (line.empty()) continue;
        if (line.substr(0,2) == "//") continue;
        if (line.substr(0,1) == "#") continue;

        vector<string> tokens;
        tokenize(line, tokens, ":");
        assert(tokens.size() > 1);

        cout << "File: " << tokens[0] << " (" << tokens[1] << ") " << endl;

        TFile* f = TFile::Open(tokens[0].c_str());
        file_list.push_back(make_pair(f, tokens[1].c_str()));
    }
    fin.close();
    return file_list;
}

vector<string> readHistograms(const string& filename) {
    static const int BUF_SIZE = 512;
    vector<string> v;

    // Open the file containing the datacards
    ifstream fin(filename, ios::in);    
    if (!fin) {
        cerr << "Input File: " << filename << " could not be opened!" << endl;
        return v;
    }
    char buf[BUF_SIZE];
    while (fin.getline(buf, BUF_SIZE, '\n')) {  // Pops off the newline character
        string line(buf);
        if (line.empty()) continue;
        if (line.substr(0,2) == "//") continue;
        if (line.substr(0,1) == "#") continue;

        cout << "Histogram: " << line << endl;
        v.push_back(line);
    }
    fin.close();

    return v;
}

void closeFiles(vector<pair<TFile*, string>>& v) {
    for (uint i = 0; i < v.size(); ++i) {
        TFile* f = v[i].first;
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

string parseEtaCut(const string& input) {
    
    string out_cut = "";
    size_t pos = input.rfind("Eta");
    string cutVal = (pos != string::npos) ? input.substr(pos) : input;
    smatch match;

    if (regex_match(cutVal, match, regex(R"(EtaOver([0-9p]+))"))) {
        string val = match[1];
        replace(val.begin(), val.end(), 'p', '.');
        out_cut = "|#eta| > " + val;
    } else if (regex_match(cutVal, match, regex(R"(Eta([0-9p]+)to([0-9p]+))"))) {
        string val1 = match[1];
        string val2 = match[2];
        replace(val1.begin(), val1.end(), 'p', '.');
        replace(val2.begin(), val2.end(), 'p', '.');
        out_cut = val1 + " < |#eta| < " + val2;
    } else if (regex_match(cutVal, match, regex(R"(EtaUnder([0-9p]+))"))) {
        string val = match[1];
        replace(val.begin(), val.end(), 'p', '.');
        out_cut = "|#eta| < " + val;
    } else {
        out_cut = input;
    }

    return out_cut;
}


vector<int> getStyleInfo(const int& cmpType, const int& index, const bool& isRatio=false) {
    
    int bulletNum = 1;  // default (dummy, will be overridden for data histograms)
    int colorNum = 1;   // default to black
    int lineNum = 1;    // default to solid line

    if (cmpType == 0 || cmpType == 1) { // Data-vs-MC or Data1-vs-Data2
        if (index == 0) {
            bulletNum = 20;
            colorNum = 1; //(!isRatio) ? 1 : 2;
        } else {
            colorNum = 2;
            lineNum = 1; 
        }
    } else if (cmpType == 2) { // Data-vs-MC1-vs-MC2
        if (index == 0) {
            // Data
            bulletNum = 20;
            colorNum = 1; //(!isRatio) ? 1 : 2;
        } else if (index == 1) {
            // MC nominal
            colorNum = 2;
            lineNum = 1; 
        } else {
            // MC alternative
            colorNum = 806;
            lineNum = 9; // dashed
        }
    } else if (cmpType == 3) { // Data1-vs-Data2-vs-MC
        if (index == 0) {   
            // Data1
            bulletNum = 20; 
            colorNum = 1;
        } else if (index == 1) { 
            // Data2
            bulletNum = 21; 
            colorNum = 855;
        } else {
            // MC
            colorNum = 2;
            lineNum = 1;
        }
    } /*
      else if (cmpType == 4) { // Data1 vs Data2 vs MC1 vs MC2
        if (index == 0) {
            bulletNum = (!isRatio) ? true : false; // Data1
            colorNum = 1;
        } else if (index == 1) {
            bulletNum = (!isRatio) ? true : false; // Data2
            colorNum = 851;
        } else if (index == 2) {
            bulletNum = false; // MC nominal
            colorNum = 2;
        } else {
            bulletNum = false; // MC alternative
            colorNum = 806;
        } 
    } */

    vector<int> styleVec = {bulletNum, colorNum, lineNum};
    return styleVec;
}

void setRatioPad(TH1* h, const string& xName, const string& yName, const double& rrange_low, const double& rrange_high) {
    h->SetTitle("");
    h->SetStats(0);
    
    TH1* hline = (TH1*)h->Clone();
    hline->SetStats(0);
    //hline->Divide(h, h, 1, 1, "B");
    for (int ib=1; ib<=hline->GetNbinsX(); ++ib) {
        hline->SetBinContent(ib, 1.0);
    }

    hline->GetXaxis()->SetTitle(xName.c_str());
    hline->GetYaxis()->SetTitle(yName.c_str());
    hline->SetMaximum(rrange_high);
    hline->SetMinimum(rrange_low);
    hline->GetXaxis()->SetLabelSize(0.1);
    hline->GetYaxis()->SetLabelSize(0.1);
    hline->GetXaxis()->SetTitleSize(0.12);
    if (yName.length() <= 8) {
        hline->GetYaxis()->SetTitleSize(0.12);
        hline->GetYaxis()->SetTitleOffset(0.41);
    } else {
        hline->GetYaxis()->SetTitleSize(0.08);
        hline->GetYaxis()->SetTitleOffset(0.52);
    }
    hline->GetXaxis()->SetTitleOffset(1.15);
    hline->GetXaxis()->SetLabelOffset(0.03);
    hline->GetXaxis()->SetTickSize(0.08);
    hline->GetYaxis()->SetNdivisions(208);
    hline->SetLineWidth(3);
    hline->SetLineStyle(1);
    hline->SetLineColor(2);
    hline->Draw("HIST ][");
}

void plotHisto(TH1* h, 
               const string& yName, 
               const bool& isPrimaryHist, 
               vector<int>& styleOpt) 
{
    h->SetTitle("");

    if (isPrimaryHist) {
        h->SetTitleSize(0.040);
        h->SetTitleOffset(0.105);
        h->GetYaxis()->SetMaxDigits(4);
        h->GetYaxis()->SetTitle(yName.c_str());
        h->GetXaxis()->SetTitleSize(0.05);
        h->GetYaxis()->SetTitleSize(0.05);
        h->GetXaxis()->SetTitleOffset(0.77);
        h->GetYaxis()->SetTitleOffset(1.02);
    } else {
        gPad->RedrawAxis();
    }

    h->SetMarkerSize(2.0);
    h->SetMarkerStyle(styleOpt[0]);
    h->SetMarkerColor(styleOpt[1]);
    h->SetLineColor(styleOpt[1]);
    h->SetLineWidth(3);
    h->SetLineStyle(styleOpt[2]); // dashed for alternative MC

    TPaveStats *hstats = new TPaveStats(0.99, 0.99, 0.99, 0.99, "brNDC");
    hstats->SetTextColor(1);
    hstats->SetOptStat(1111);
    hstats->Draw();
    h->GetListOfFunctions()->Add(hstats);
    hstats->SetParent(h);

    string option = isPrimaryHist ? "E" : ((styleOpt[0]>10) ? "E SAME" : "HIST SAME");

    TGaxis::SetExponentOffset(-0.07, 0, "y");
    TGaxis::SetMaxDigits(3);
    h->GetYaxis()->SetLabelSize(0.045);
    h->GetXaxis()->SetLabelSize(0.0);
    h->Draw(option.c_str());
}


void fit_and_plot_trackPCA(vector<pair<TH1*, bool>> fitsPCA_info, TLegend* legend) {

    vector<pair<double, double>> fitResults; // to store fit results for trackPCA

    for (const auto& fitInfo : fitsPCA_info) {
        TH1* h = fitInfo.first;
        bool isData = fitInfo.second;

        TF1* fitFunc = new TF1((isData ? "fa1" : "fa2"), 
                               "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])+[3])", 
                               -8., 8.);

        fitFunc->SetParameters(0, 250000000);
        fitFunc->SetParameters(1, 0.);
        fitFunc->SetParameters(2, 4);
        fitFunc->SetParameters(3, 0.);
        fitFunc->SetLineColor((isData) ? kBlue : kGreen);
        fitFunc->SetLineStyle((isData) ? 5     : 7     );
        
        h->Fit(fitFunc,"Q","SAME",-8.,8.);
        fitFunc->Draw("SAME");
        legend->AddEntry(fitFunc, (isData ? "Data fit" : "MC fit"), "L");
        cout << "Sigma : " << fitFunc->GetParameter(2) << " +- " << fitFunc->GetParError(2) << endl;
        fitResults.push_back(make_pair(fitFunc->GetParameter(1), abs(fitFunc->GetParameter(2))));
    }

    legend->Draw();

    ostringstream datamu, datasigma, mcmu, mcsigma;
    datamu.precision(2);
    datasigma.precision(2);
    mcmu.precision(2);
    mcsigma.precision(2);
    datamu << std::fixed << fitResults[0].first;
    datasigma << std::fixed << fitResults[0].second;
    mcmu << std::fixed << fitResults[1].first;
    mcsigma << std::fixed << fitResults[1].second;
    
    // Print fit results
    TLegend *legend12 = new TLegend(0.62, 0.4, 0.7, 0.64);
    legend12->SetHeader("Fit results:");
    legend12->AddEntry((TObject*)0, ((string("Data #mu=")+datamu.str()+string(" cm")).c_str()), "");
    legend12->AddEntry((TObject*)0, ((string("Data #sigma=")+datasigma.str()+string(" cm")).c_str()), "");
    legend12->AddEntry((TObject*)0, ((string("MC #mu=")+mcmu.str()+string(" cm")).c_str()), "");
    legend12->AddEntry((TObject*)0, ((string("MC #sigma=")+mcsigma.str()+string(" cm")).c_str()), "");
    legend12->SetTextSize(0.035);
    legend12->SetBorderSize(0);
    legend12->SetFillStyle(0);
    legend12->Draw();
}

void compareHisto(TCanvas* canvas, 
                  const vector<pair<TFile*, string>>& data_list, 
                  const string& analysis_folder,
                  const bool& cmpData, 
                  const bool& profiles,
                  const vector<string>& hist_tokens,
                  const vector<string>& print_info, 
                  const vector<double>& rrange,
                  const string& outfolder)
{
    cout << "Compare Histo Start" << endl;

    string pluginName = "StandaloneTrackMonitor";
    string folderName = analysis_folder;
    string hname(hist_tokens[0]);
    
    if (print_info[0]=="Z#rightarrowee" && hname=="ZInvMass") {
        pluginName = "ZEEDetails";
    } else if ((hname.rfind("n_vs_",0)==0 || hname.rfind("v0_",0)==0)) {
        if (print_info[0]=="ZeroBias_Lambda")  folderName = "V0Monitoring/Lambda";
        else if (print_info[0]=="ZeroBias_K0") folderName = "V0Monitoring/Ks";
    }

    string datafolder = "DQMData/Run 999999/" + pluginName + "/Run summary/" + folderName + "/";
    string mcfolder   = "DQMData/Run 1/"      + pluginName + "/Run summary/" + folderName + "/";

    canvas->cd();

    TString h_title = hist_tokens[1];
    gStyle->SetLineWidth(3);
    gStyle->SetOptStat(0);

    // Main pad
    TPad* pad11 = new TPad("pad11", "pad11", 0, 0.30, 1.0, 1.0); // 0.27
    pad11->SetBottomMargin(0.025);
    pad11->SetTickx();
    pad11->SetTicky();
    pad11->SetFrameLineWidth(3);
    pad11->Draw();
    pad11->cd();

    double hmax = -1;
    double nentriesdata = 0;
    vector<TH1*> hists(data_list.size());

    int cmpType;
    if      (data_list.size()==2 && !cmpData) cmpType = 0; // Data vs MC
    else if (data_list.size()==2 && cmpData)  cmpType = 1; // Data1 vs Data2
    else if (data_list.size()==3 && !cmpData) cmpType = 2; // Data vs MC1 vs MC2
    else if (data_list.size()==3 && cmpData)  cmpType = 3; // Data1 vs Data2 vs MC
    // else if (data_list.size()==4 && cmpData)  cmpType = 4; // Data1 vs Data2 vs MC1 vs MC2
    else {
        cerr << "Invalid number of files for comparison: " << data_list.size() << endl;
        return;
    }

    vector<pair<TH1*, bool>> fitsPCA_info; // to store fit results for trackPCA

    vector<uint> idx(data_list.size());
    for (uint i = 0; i < idx.size(); ++i) {
        idx[i] = i;
    }
    if (cmpType == 3) std::swap(idx[0], idx[1]);  // when comparing two data and one MC, the first data in the list is plotted after the second one to be more visible.

    bool setMainPanel = true; // flag to set the main panel properties only for the first plotted histogram

    // DRAWING PLOTS
    for (uint i : idx) {
        
        TFile* f = data_list[i].first;
        if (i==0 || (i==1 && cmpData)) f->cd(datafolder.c_str());
        else {
            //TDirectory* dir = (TDirectory*)f->Get(mcfolder.c_str());
            if (f->GetDirectory(mcfolder.c_str())) {
                f->cd(mcfolder.c_str());
            } else {
                f->cd(datafolder.c_str());
            }
        }
        
        TH1 *h = nullptr;
        if (!profiles) {
            h = dynamic_cast<TH1*>(gDirectory->Get(hname.c_str()));
        } else {
            TProfile *h_profile = dynamic_cast<TProfile*>(gDirectory->Get(hname.c_str()));            
            if (h_profile) h = h_profile->ProjectionX((hname+"_px_"+to_string(i)).c_str(), "e");
        }
        assert(h);
        h->Sumw2();
        hists[i] = h;

        TAxis* xaxis = h->GetXaxis();
        double xmin = (
            hist_tokens.size() > 5 && hist_tokens[5] != " " && !hist_tokens[5].empty()
            ) ? stod(hist_tokens[5]) : xaxis->GetXmin();

        double xmax = (
            hist_tokens.size() > 6 && hist_tokens[6] != " " && !hist_tokens[6].empty()
            ) ? stod(hist_tokens[6]) : xaxis->GetXmax();

        if (hist_tokens.size() > 7) h->Rebin(stoi(hist_tokens[7]));
    
        xaxis->SetRangeUser(xmin, xmax);
        int nbinsx = h->GetNbinsX();        
        if (i==0) nentriesdata = h->Integral("width");
        
        string yTitle = hist_tokens[2];        

        if (!profiles) {
            double norm = 1.0;
            if (i>0 && (cmpType<=2)) { 
                norm = nentriesdata / h->Integral("width"); 
            }
            else if (cmpType>=3) {
                norm = 1.0 / h->Integral("width");
                size_t pos = hist_tokens[2].find("Number");
                if (pos != std::string::npos) {
                    yTitle.replace(pos, 6, "Fraction");
                }
            }
            h->Scale(norm);
        }
        
        if (h->GetMaximum() > hmax) hmax = h->GetMaximum();

        vector<int> styleInfo = getStyleInfo(cmpType, i);

        // cout << "Plotting hist num. " << i << " with bullet: " << styleInfo[0] << ", color: " << styleInfo[1] << ", line: " << styleInfo[2] << endl;
        plotHisto(h, yTitle, setMainPanel, styleInfo);
        setMainPanel = false;

        if (hname == "beamSpotZpos") {
            fitsPCA_info.push_back(make_pair(h, (i==0 || (i==1 && cmpData))));
        }
    }

    TLegend *legend11 = new TLegend(0.52, 0.69, 0.92, 0.86);
    legend11->SetTextSize(0.042);
    legend11->SetBorderSize(0);
    legend11->SetFillStyle(0);
    for (uint i=0; i<data_list.size(); ++i) {
        TH1* h_leg = (TH1*)hists.at(i)->Clone();
        h_leg->SetMarkerSize(1.0);
        legend11->AddEntry(h_leg, data_list[i].second.c_str(), ((i==0 || (i==1 && cmpType==3)) ? "LEP" : "L"));
    }
    if (data_list.size()==2 && hname!="beamSpotZpos") legend11->AddEntry((TObject*)0, "", "");


    TH1* h = (cmpType!=3) ? hists.at(0) : hists.at(1);
    if (!profiles) {
        double fct = (hist_tokens.size()>4 && hist_tokens[4]=="log") ? 11 : 1.4;
        h->SetMaximum(fct * hmax);
    } else {
        h->SetMinimum(min(0.0, 1.5*h->GetMinimum()));
        h->SetMaximum(1.5*hmax);
    }

    // Add eta cut info, if any. Assumes folder_postfix starts with "Eta".
    string varCutInfo = parseEtaCut(print_info[2]);

    textonplot(0.16, 0.83, 0.28, 0.85, 42, 0.058, print_info[0]);  // analysis info
    textonplot(0.53, 0.83, 0.28, 0.85, 42, 0.04, varCutInfo.c_str());  // variable cut info
    textonplot(0.52, 0.875, 0.84, 1.0, 42, 0.058, print_info[1]);  // lumi info
    textonplot(0.10, 0.875, 0.18, 1.0, 62, 0.058, "CMS");
    textonplot(0.24, 0.860, 0.34, 1.0, 52, 0.058, "  Preliminary");
    rmdot(0.98, 0.97, 1.0, 1.0, 52, "             ");
    
    if (hist_tokens.size()>3 && hist_tokens[3]=="log") pad11->SetLogx();
    if (hist_tokens.size()>4 && hist_tokens[4]=="log") pad11->SetLogy();

    if (hname == "beamSpotZpos") {
        fit_and_plot_trackPCA(fitsPCA_info, legend11);
    } else {
        legend11->Draw();
    }

    pad11->Update();
    pad11->Modified();
    
    cout << "Main pad drawn successfully" << endl; 

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

    vector<int> idxs_ref; // reference histogram indexes for the ratio plot
    if (cmpType<3) idxs_ref.push_back(1);
    if (cmpType>1) idxs_ref.push_back(2);
    // if (cmpType>3) idxs_ref.push_back(3);


    for (uint j=0; j<idxs_ref.size(); ++j) {
        
        int ir = idxs_ref[j];
        // cout << "Preparing ratio with hist num. " << ir << " as reference" << endl;

        TH1 *h_ref = hists.at(ir);
        
        if (j==0) {
            double ratio_low, ratio_high;
            if (hist_tokens.size() > 8) {
                size_t pos_sep = hist_tokens[8].find("-");
                ratio_low = std::stod(hist_tokens[8].substr(0, pos_sep));
                ratio_high = std::stod(hist_tokens[8].substr(pos_sep+1));
            } else {
                ratio_low = rrange[0];
                ratio_high = rrange[1];
            }
            setRatioPad(h_ref, hist_tokens[1], 
                        (cmpType!=3) ? "Data/MC" : "Ratio to earliest data",
                        ratio_low, ratio_high
            );
        }

        // DRAWING RATIO
        for (uint i : idx) {
            if (ir==int(i) || find(idxs_ref.begin(), idxs_ref.end(), int(i))!=idxs_ref.end()) continue;
            // cout << "Drawing ratio of hist num. " << i << " over hist num. " << ir << endl;
            
            TH1 *h2 = hists.at(i);
            TH1* h_ratio = (TH1*)h_ref->Clone();

            h_ratio->Divide(h2, h_ref, 1, 1, "B");

            vector<int> styleInfo = getStyleInfo(cmpType, i, true); 

            /*
            if (cmpType < 3) {
                styleInfo = getStyleInfo(cmpType, ir, false); // use style of denominator hist
            } else {
                styleInfo = getStyleInfo(cmpType, i, true); // only exception, since the MC is black in the plot
            } */
            plotHisto(h_ratio, string(""), false, styleInfo);

        }
    }
   
    if (hist_tokens.size()>3 && hist_tokens[3]=="log") pad21->SetLogx();
    rmdot(0.95, 0.95, 1.0, 1.0, 52, "             ");    
    pad21->Update();
    pad21->Modified();

    cout << "Ratio pad drawn successfully" << endl;

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
