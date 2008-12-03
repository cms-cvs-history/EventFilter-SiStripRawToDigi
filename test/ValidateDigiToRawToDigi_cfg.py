import FWCore.ParameterSet.Config as cms

process = cms.Process("ValidateDigiToRawToDigi")

process.load("DQM.SiStripCommon.MessageLogger_cfi")

process.load("CalibTracker.Configuration.SiStrip_FakeConditions_cff")

process.load("IORawData.SiStripInputSources.EmptySource_cff")

process.load("EventFilter.SiStripRawToDigi.test.SiStripTrivialDigiSource_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigis_cfi")

process.load("EventFilter.SiStripRawToDigi.test.SiStripDigiValidator_cfi")

process.p = cms.Path(process.DigiSource*process.SiStripDigiToRaw*process.siStripDigis*process.DigiValidator)
process.maxEvents.input = 20
process.DigiSource.FedRawDataMode = False
process.DigiSource.UseFedKey = False
process.SiStripDigiToRaw.InputModuleLabel = 'SiStripDigiSource'
process.SiStripDigiToRaw.InputDigiLabel = ''
process.SiStripDigiToRaw.FedReadoutMode = 'ZERO_SUPPRESSED'
process.SiStripDigiToRaw.UseFedKey = False
process.siStripDigis.ProductLabel = 'SiStripDigiToRaw'
process.siStripDigis.UseFedKey = False
process.DigiValidator.FedRawDataMode = False

