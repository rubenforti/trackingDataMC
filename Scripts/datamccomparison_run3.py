
import os
import argparse
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

current_path = os.path.dirname(__file__)
header_path = os.path.join(current_path, 'utilities', 'comparisons_utils.h')

ROOT.gInterpreter.Declare('#include "'+header_path+'"')
ROOT.gSystem.CompileMacro('utilities/comparisons_utils.cpp', opt="ks")

from utilities.cmp_info import yr_info_base, yr_info_zerobias, yr_info_lepton, an_info

def data_mc_comparison_run3(analysis, 
                            year="", 
                            folder_postfix="", 
                            cmpData=False,
                            doProfiles=False,
                            inputFilename="",
                            outfolder="./"):
    
    print(f"Doing comparisons for {analysis} {year} analysis")
    
    #filelist = ROOT.std.vector('TFile*')()
    #lglist = ROOT.std.vector('string')()
    
    #ROOT.openFiles(filelist, lglist, ROOT.std.string(inputFilename), ROOT.std.string(analysis))

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
        #ROOT.std.string(luminfo)
    ])

    an_folder = ROOT.std.string(an_info[analysis]["folder"] + folder_postfix)

    ROOT.readHistograms(hlist_compare, ROOT.std.string(an_info[analysis]["filenames"][0]))
    print(f">>> # of histograms compare: {len(hlist_compare)}")

    
    canvas = ROOT.TCanvas("canvas", "canvas")
    canvas.SetCanvasSize(800, 800)
    canvas.SetLeftMargin(0.11)

    for hcmp_line in [hlist_compare[0]]:
        tokens = str(hcmp_line).split(":")
        assert len(tokens) > 1
        ROOT.compareHisto(canvas, 
                          data_list, an_folder,
                          cmpData, False, 
                          tokens, print_info, 
                          ROOT.std.string(outfolder))

    
    if doProfiles:
        ROOT.readHistograms(hlist_profile, ROOT.std.string(an_info[analysis]["filenames"][1]))
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
                        choices=['ZeroBias', 'ZEE', 'ZMM'],
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
    parser.add_argument('--cmpData',
                        action='store_true',
                        help='Whether to compare data vs data instead of data vs MC')
    parser.add_argument('--doProfiles', 
                        action='store_true',
                        help='Whether to do profile comparisons')
    parser.add_argument('--doScatter',
                        action='store_true',
                        help='Whether to do scatter plot comparisons')
    parser.add_argument('--inputFilename', 
                        type=str,
                        default="",
                        help='Filename containing list of ROOT files to open, default uses predefined names')
    parser.add_argument('--outfolder', 
                        type=str, 
                        default="./",
                        help='Output folder for comparison plots')

    args = parser.parse_args()

    data_mc_comparison_run3(analysis=args.analysis,
                            year=args.year,
                            folder_postfix=args.folder_postfix,
                            cmpData=args.cmpData,
                            doProfiles=args.doProfiles,
                            inputFilename=args.inputFilename,
                            outfolder=args.outfolder)




