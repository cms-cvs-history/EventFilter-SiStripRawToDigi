import FWCore.ParameterSet.Config as cms

process = cms.Process("ValidateDigiToRawToClusters")

process.load("DQM.SiStripCommon.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("CalibTracker.Configuration.SiStrip_FakeConditions_cff")

process.load("CalibTracker.SiStripConnectivity.SiStripRegionConnectivity_cfi")

process.load("IORawData.SiStripInputSources.EmptySource_cff")

process.load("EventFilter.SiStripRawToDigi.test.SiStripTrivialDigiSource_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigis_cfi")

process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripRawToClusters_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripRawToClustersRoI_cfi")

process.load("EventFilter.SiStripRawToDigi.test.SiStripClustersDSVBuilder_cfi")

process.load("EventFilter.SiStripRawToDigi.test.SiStripClusterValidator_cfi")

process.p = cms.Path(process.DigiSource*process.SiStripDigiToRaw*process.siStripDigis*process.siStripClusters*process.SiStripRawToClustersFacility*process.SiStripRoI*process.siStripClustersDSV*process.ValidateSiStripClusters)
process.maxEvents.input = 1
process.SiStripDigiToRaw.FedReadoutMode = 'ZERO_SUPPRESSED'
process.SiStripDigiToRaw.InputModuleLabel = 'DigiSource'
process.SiStripDigiToRaw.InputDigiLabel = ''
process.siStripDigis.ProductLabel = 'SiStripDigiToRaw'
process.siStripClusters.DigiProducersList = cms.VPSet(cms.PSet(
    DigiLabel = cms.string('ZeroSuppressed'),
    DigiProducer = cms.string('siStripDigis')
))
process.SiStripRawToClustersFacility.FedRawData = 'SiStripDigiToRaw'
process.siStripClustersDSV.SiStripRefGetter = 'SiStripRoI'

