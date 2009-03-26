import FWCore.ParameterSet.Config as cms

# Services
from DQM.SiStripCommon.MessageLogger_cfi import *
MessageLogger.debugModules = cms.untracked.vstring()
Timing = cms.Service("Timing")
Tracer = cms.Service(
    "Tracer", 
    sourceSeed = cms.untracked.string("$$")
    )

# Conditions
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
GlobalTag.globaltag = "IDEAL_30X::All" 

# Digi Source (old and new)
from EventFilter.SiStripRawToDigi.test.SiStripTrivialDigiSource_cfi import *
DigiSource.FedRawDataMode = False
DigiSource.UseFedKey = False

# DigiToRaw (old and new) 
from EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi import *
SiStripDigiToRaw.FedReadoutMode = 'ZERO_SUPPRESSED'
SiStripDigiToRaw.InputModuleLabel = 'DigiSource'
SiStripDigiToRaw.InputDigiLabel = ''
SiStripDigiToRaw.UseFedKey = False

# RawToDigi (new)
from EventFilter.SiStripRawToDigi.SiStripDigis_cfi import *
siStripDigis.ProductLabel = 'SiStripDigiToRaw'
siStripDigis.UseFedKey = False

# RawToDigi (old)
oldSiStripDigis = cms.EDProducer(
    "OldSiStripRawToDigiModule",
    ProductLabel =  cms.untracked.string('SiStripDigiToRaw'),
    UseFedKey = cms.untracked.bool(False),
    )

# Digi Validator (new)
from EventFilter.SiStripRawToDigi.test.SiStripDigiValidator_cfi import *
DigiValidator.TagCollection1 = "DigiSource"
DigiValidator.TagCollection2 = "siStripDigis:ZeroSuppressed"
DigiValidator.RawCollection1 = False
DigiValidator.RawCollection2 = False

# Digi Validator (old)
oldDigiValidator = DigiValidator.clone()
oldDigiValidator.TagCollection1 = "DigiSource"
oldDigiValidator.TagCollection2 = "oldSiStripDigis:ZeroSuppressed"
oldDigiValidator.RawCollection1 = False
oldDigiValidator.RawCollection2 = False

# Digi Validator (compare)
testDigiValidator = DigiValidator.clone()
testDigiValidator.TagCollection1 = "oldSiStripDigis:ZeroSuppressed"
testDigiValidator.TagCollection2 = "siStripDigis:ZeroSuppressed"
testDigiValidator.RawCollection1 = False
testDigiValidator.RawCollection2 = False

# PoolOutput
output = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('output.root'),
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep SiStrip*_simSiStripDigis_*_*', # (to drop SimLinks)
    'keep *_*_*_DigiToRawToDigi'
    )
    )

# Sequences and Paths
new = cms.Sequence(
    siStripDigis *
    DigiValidator
    )

old = cms.Sequence(
    oldSiStripDigis *
    oldDigiValidator
    )

test = cms.Sequence(
    testDigiValidator
    )

s = cms.Sequence( SiStripDigiToRaw * old * new * test )
e = cms.EndPath( output )
