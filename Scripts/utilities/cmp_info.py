yr_info_base = {
    "2022" : "13.6 TeV",
    "2023" : "13.6 TeV",
    "2024" : "13.6 TeV",
    "2025" : "13.6 TeV",
}

yr_info_zerobias = yr_info_base | {
    "2025C": "141.8 nb^{-1} (2025, 13.6 TeV)",
    "2025E":  "87.3 nb^{-1} (2025, 13.6 TeV)",
    "2025G": "138.9 nb^{-1} (2025, 13.6 TeV)",
}

yr_info_lepton = yr_info_base | {
    "2024C": "6.86 fb^{-1} (2024, 13.6 TeV)",
    "2024F": "27.8 fb^{-1} (2024, 13.6 TeV)",
    "2024F-before": "9.17 fb^{-1} (2024, 13.6 TeV)",
    "2024F-after" : "18.7 fb^{-1} (2024, 13.6 TeV)",
    "2024I": "11.5 fb^{-1} (2024, 13.6 TeV)",

    "2025C": "21.9 fb^{-1} (2025, 13.6 TeV)",
    "2025E": "14.0 fb^{-1} (2025, 13.6 TeV)",
    "2025G": "22.2 fb^{-1} (2025, 13.6 TeV)",
}


an_info = {

    "ZeroBias" : {
        "folder" : "highPurityTracks",
        "printAnInfo" : "ZeroBias",
        "filenames" : [
            "highpurityHistolist_run3.txt",
            "highpurityProfile_run3.txt",
            "highpurityScatter_run3.txt"
        ],
    },

    "ZEE" : {
        "folder" : "ElectronTracks",
        "printAnInfo" : "Z#rightarrowee",
        "filenames" : [
            "ElectronHistolist_run3.txt",
            "ElectronProfile_run3.txt"
        ],
    },
    "ZMM" : {
        "folder" : "MuonTracks",
        "printAnInfo" : "Z#rightarrow#mu#mu",
        "filenames" : [
            "MuonHistolist_run3.txt",
            "MuonProfile_run3.txt"
        ],
    },
    "ZeroBias_K0" : {
        "folder" : "highPurityTracks",
        "printAnInfo" : "ZeroBias_K0",
        "filenames" : [
            "highpurity_K0_Histolist_run3.txt",
            "highpurity_K0_Profile_run3.txt"
        ],
    },
    "ZeroBias_Lambda" : {
        "folder" : "highPurityTracks",
        "printAnInfo" : "ZeroBias_Lambda",
        "filenames" : [
            "highpurity_Lambda_Histolist_run3.txt",
            "highpurity_Lambda_Profile_run3.txt"
        ],
    },
}
