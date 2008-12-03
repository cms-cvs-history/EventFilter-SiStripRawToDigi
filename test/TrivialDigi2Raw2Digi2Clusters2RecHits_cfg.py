import FWCore.ParameterSet.Config as cms

process = cms.Process("TrivialDigi2Raw2Digi2Clusters2RecHits")

process.load("DQM.SiStripCommon.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("CalibTracker.Configuration.SiStrip_FakeConditions_cff")

process.load("Alignment.CommonAlignmentProducer.FakeAlignmentSource_cfi")

process.load("CalibTracker.SiStripConnectivity.SiStripRegionConnectivity_cfi")

process.load("IORawData.SiStripInputSources.EmptySource_cff")

process.load("EventFilter.SiStripRawToDigi.test.SiStripTrivialClusterSource_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigis_cfi")

process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")

process.load("CalibTracker.SiStripLorentzAngle.SiStripLAFakeESSource_cfi")

process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")

process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")

process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")

process.p = cms.Path(process.ClusterSource*process.SiStripDigiToRaw*process.siStripDigis*process.siStripClusters*process.siStripMatchedRecHits)
process.maxEvents.input = 20
process.SiStripDigiToRaw.InputModuleLabel = 'ClusterSource'
process.SiStripDigiToRaw.InputDigiLabel = ''
process.siStripDigis.ProductLabel = 'SiStripDigiToRaw'
process.siStripClusters.DigiProducersList = cms.VPSet(cms.PSet(
    DigiLabel = cms.string('ZeroSuppressed'),
    DigiProducer = cms.string('siStripDigis')
))
process.siStripMatchedRecHits.ClusterProducer = 'siStripClusters'

