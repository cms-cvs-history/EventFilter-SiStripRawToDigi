#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_SEAL_MODULE();

#include "EventFilter/SiStripRawToDigi/test/stubs/AnalyzeSiStripDigis.h"
DEFINE_ANOTHER_FWK_MODULE(AnalyzeSiStripDigis);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripPerformanceRecordModule.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripPerformanceRecordModule);


