import FWCore.ParameterSet.Config as cms

SiStripDigiToRaw = cms.EDProducer(
    "SiStripDigiToRawModule",
    InputModuleLabel = cms.string('mix'),
    InputDigiLabel = cms.string('ZeroSuppressed'),
    FedReadoutMode = cms.string('ZERO_SUPPRESSED'),
    UseFedKey = cms.bool(False),
    UseWrongDigiType = cms.bool(False)
    )

