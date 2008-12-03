import FWCore.ParameterSet.Config as cms

process = cms.Process("RegionalDigi2Raw2RecHits")

process.load("DQM.SiStripCommon.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("CalibTracker.Configuration.SiStrip_FakeConditions_cff")

process.load("Alignment.CommonAlignmentProducer.FakeAlignmentSource_cfi")

process.load("CalibTracker.SiStripConnectivity.SiStripRegionConnectivity_cfi")

process.load("IORawData.SiStripInputSources.EmptySource_cff")

process.load("EventFilter.SiStripRawToDigi.test.SiStripTrivialClusterSource_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi")

process.load("EventFilter.SiStripRawToDigi.SiStripRawToClusters_cff")

process.load("CalibTracker.SiStripLorentzAngle.SiStripLAFakeESSource_cfi")

process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")

process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")

process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")

process.p = cms.Path(process.ClusterSource*process.SiStripDigiToRaw*process.SiStripRawToClusters*process.siStripMatchedRecHits)

process.maxEvents.input = 20
process.SiStripDigiToRaw.InputModuleLabel = 'ClusterSource'
process.SiStripDigiToRaw.InputDigiLabel = ''
process.SiStripRawToClustersFacility.FedRawData = cms.InputTag("SiStripDigiToRaw","")
process.siStripMatchedRecHits.ClusterProducer = 'SiStripRoI'
process.siStripMatchedRecHits.Regional = True

