#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_SEAL_MODULE();

#include "EventFilter/SiStripRawToDigi/test/stubs/AnalyzeSiStripDigis.h"
DEFINE_ANOTHER_FWK_MODULE(AnalyzeSiStripDigis);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripTrivialClusterSource.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripTrivialClusterSource);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripTrivialDigiSource.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripTrivialDigiSource);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripPerformanceRecordModule.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripPerformanceRecordModule);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripRawToClustersDummyRoI.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripRawToClustersDummyRoI);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripRawToClustersDummyUnpacker.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripRawToClustersDummyUnpacker);

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripRawToClusters.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripRawToClusters);


