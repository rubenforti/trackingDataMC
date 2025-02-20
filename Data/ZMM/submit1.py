from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'ZMM_2024G_golden_1' #PUT A REQUEST NAME
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.psetName = 'step1_cfg.py'
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = '/Muon1/Run2024G-PromptReco-v1/AOD' #TO BE MODIFIED TO ACTUALLY SELECT YOU REQUIRED DATASET (CAN ALSO BE RECO)
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.publication = False
config.Data.outLFNDirBase = 'trackingPOG/samples'
config.Data.outputDatasetTag = 'Run2024G_golden' #CAN BE MODIFIED ACCORDINGLY
config.Data.lumiMask = '../json_data/Cert_Collisions2024_378981_384380_Golden.json' #EXAMPLE LUMISECTION JSON FILE

config.Site.storageSite = 'T3_CH_CERNBOX' #NAME OF A STORAGE SITE WHERE YOU HAVE WRITING PERMISSIONS
