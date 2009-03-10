import FWCore.ParameterSet.Config as cms

Source = str("SIM") # Options: "TRIV", "SIM"

process = cms.Process("DigiToRawToClusters")

process.load("EventFilter.SiStripRawToDigi.test.Validate_DigiToRawToClusters_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.MessageLogger.destinations = cms.untracked.vstring(
    "cerr",
    "DigiToRawToClusters",
    "info",
    "warning",
    "error"
    )

process.output.fileName = "DigiToRawToClusters.root"

if Source == str("TRIV") :

    process.source = cms.Source(
        "EmptySource",
        firstRun = cms.untracked.uint32(999999)
        )

    process.SiStripDigiToRaw.FedReadoutMode = 'ZERO_SUPPRESSED'
    process.SiStripDigiToRaw.InputModuleLabel = 'DigiSource'
    process.SiStripDigiToRaw.InputDigiLabel = ''

    process.p = cms.Path( process.DigiSource * process.s )

elif Source == str("SIM") :

    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_1_0_pre2/RelValQCD_FlatPt_15_3000/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_30X_v1/0001/027A5B5E-AF03-DE11-832C-000423D99AAE.root'
        )
        )

    process.SiStripDigiToRaw.FedReadoutMode = 'ZERO_SUPPRESSED'
    process.SiStripDigiToRaw.InputModuleLabel = 'simSiStripDigis'
    process.SiStripDigiToRaw.InputDigiLabel = 'ZeroSuppressed'

    process.p = cms.Path( process.s )

else :

    print "UNKNOWN INPUT SOURCE!"
    
    
