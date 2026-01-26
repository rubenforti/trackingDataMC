
import os
import argparse
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

current_path = os.path.dirname(os.path.abspath(__file__))
cpp_path = os.path.join(current_path, 'utilities', 'comparisons_utils.cpp')

ROOT.gSystem.CompileMacro('utilities/comparisons_utils.cpp', opt="ks")

from utilities.cmp_info import yr_info_base, yr_info_zerobias, yr_info_lepton, an_info


def hist_idxs(filename, histNames):
    idxs = []
    count = 0
    with open(filename, "r", encoding="utf-8") as f:
        lines = f.readlines()
        for line in lines:

            if line.startswith("##"):
                continue

            if any(line.startswith(f"{h}:") for h in histNames):
                idxs.append(count)
            
            count += 1
        
        if len(idxs) == 0:
            print(f">>> WARNING: No matching hist names found in {filename}")

    return idxs


def data_mc_comparison_run3(analysis, 
                            year="", 
                            folder_postfix="",
                            histNames=[], 
                            cmpData=False,
                            doProfiles=False,
                            inputFilename="",
                            outfolder="./",
                            rrange=[0.4, 1.6]):
    
    print(f"Doing comparisons for {analysis} {year} analysis")

    data_list = ROOT.openFiles(ROOT.std.string(inputFilename), ROOT.std.string(analysis))

    print(f">>> # of Root files: {len(data_list)}")
    
    if len(data_list) > 3:
        print(">>> Max 3 files are allowed for comparison")
        return
    
    hlist_compare = ROOT.std.vector('string')()
    hlist_profile = ROOT.std.vector('string')()

    yr_dict = yr_info_zerobias if analysis=="ZeroBias" else yr_info_lepton if analysis in ["ZEE", "ZMM"] else yr_info_base


    print_info = ROOT.std.vector('string')([
        an_info[analysis]["printAnInfo"],
        yr_dict.get(year, ""),
        folder_postfix
    ])

    an_folder = ROOT.std.string(an_info[analysis]["folder"] + folder_postfix)

    hlist_compare = ROOT.readHistograms(ROOT.std.string(an_info[analysis]["filenames"][0]))
    print(f">>> # of histograms compare: {len(hlist_compare)}")

    if len(histNames) > 0:
        hist_idxs_list = hist_idxs(an_info[analysis]["filenames"][0], histNames)
        hlist_compare_filtered = ROOT.std.vector('string')()
        for i, hName in enumerate(hlist_compare):
            if i in hist_idxs_list:
                hlist_compare_filtered.push_back(hName)
        hlist_compare = hlist_compare_filtered
        print(f">>> # of histograms after filtering: {hlist_compare.size()}")

    if not os.path.exists(outfolder): 
        os.makedirs(outfolder)
    
    canvas = ROOT.TCanvas("canvas", "canvas")
    canvas.SetCanvasSize(800, 800)
    canvas.SetLeftMargin(0.11)

    for hcmp_line in hlist_compare:
        tokens = str(hcmp_line).split(":")
        assert len(tokens) > 1
        ROOT.compareHisto(canvas, 
                          data_list, an_folder,
                          cmpData, False, 
                          tokens, print_info, ROOT.std.vector('double')({rrange[0], rrange[1]}),
                          ROOT.std.string(outfolder))

    
    if doProfiles:
        hlist_profile = ROOT.readHistograms(ROOT.std.string(an_info[analysis]["filenames"][1]))
        print(f">>> # of histograms profile: {len(hlist_profile)}")
        
        for hprof_line in hlist_profile:
            tokens = str(hprof_line).split(":")
            assert len(tokens) > 1
            ROOT.compareHisto(canvas, data_list, an_folder, 
                               cmpData, True, 
                               tokens, print_info, 
                               ROOT.std.string(outfolder))

    ROOT.closeFiles(data_list)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Data-MC comparison for Run 3 analyses')
    parser.add_argument('-an', '--analysis', 
                        type=str, 
                        required=True,
                        choices=['ZeroBias', 'ZEE', 'ZMM', 'K0', 'Lambda'],
                        help='Analysis to run (e.g., ZeroBias, ZEE, ZMM)'
                        )
    parser.add_argument('-y', '--year', 
                        type=str, 
                        default="",
                        help='Year or period of data taking (e.g., 2024C, 2025E)'
                        )
    parser.add_argument('-fp', '--folder_postfix', 
                        type=str, 
                        default="",
                        help='Postfix of the folder in the root files where to get the histograms')
    parser.add_argument('-i', '--input', 
                        type=str,
                        default="",
                        help='Filename containing list of ROOT files to open, default uses predefined names')
    parser.add_argument('-o', '--outfolder', 
                        type=str, 
                        default="./",
                        help='Output folder for comparison plots')
    parser.add_argument('-n', '--histNames',
                        type=str,
                        nargs='*',
                        default=[],
                        help='Hist names to be compared, if not provided all histograms in the specific file are compared')
    parser.add_argument('--cmpData',
                        action='store_true',
                        help='Whether to compare data vs data instead of data vs MC')
    parser.add_argument('--doProfiles', 
                        action='store_true',
                        help='Whether to do profile comparisons')
    parser.add_argument('--doScatter',
                        action='store_true',
                        help='Whether to do scatter plot comparisons')
    parser.add_argument('--rrange',
                        type=float,
                        nargs=2,
                        default=[0.4, 1.6],
                        help='Y-axis range for ratio plots')

    args = parser.parse_args()

    data_mc_comparison_run3(analysis=args.analysis,
                            year=args.year,
                            folder_postfix=args.folder_postfix,
                            histNames=args.histNames,
                            cmpData=args.cmpData,
                            rrange=args.rrange,
                            doProfiles=args.doProfiles,
                            inputFilename=args.input,
                            outfolder=args.outfolder)




