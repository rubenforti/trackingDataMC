# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 -s DQM -n 1 --eventcontent DQM --conditions auto:com10 --filein /store/relval/CMSSW_7_1_2/MinimumBias/RECO/GR_R_71_V7_dvmc_RelVal_mb2012Cdvmc-v1/00000/00209DF4-3708-E411-9FA7-0025905A6126.root --data --no_exec --python_filename=test_step1_cfg.py
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

process = cms.Process('DQM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
  secondaryFileNames = cms.untracked.vstring(),
  fileNames = cms.untracked.vstring([
  ])
)
process.options = cms.untracked.PSet(
)
# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step1 nevts:1'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.DQMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.DQMEventContent.outputCommands,
    fileName = cms.untracked.string('step1_DQM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_Prompt_v4', '') #TO BE MODIFIED SO THAT IT MATCHES THE GT OF THE DATASET YOU ARE PROCESSING

## HLT Filter
#import HLTrigger.HLTfilters.hltHighLevel_cfi

#process.HLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
#     HLTPaths = ['HLT_*PixelCluster*'],
#     throw = False #dont throw except on unknown path names
#)
#process.hltFilter_step = cms.Path(process.HLTFilter)

# Tracker Data MC validation suite
process.load('DQM.TrackingMonitorSource.TrackingDataMCValidation_Standalone_cff')
process.analysis_step = cms.Path(process.standaloneValidationMinbias)

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.analysis_step, process.endjob_step, process.DQMoutput_step)
