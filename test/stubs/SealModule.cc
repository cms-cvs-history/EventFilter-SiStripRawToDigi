#include "PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE();

#include "FWCore/Framework/interface/MakerMacros.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripAnalyzeDigis.h"
DEFINE_ANOTHER_FWK_MODULE(SiStripAnalyzeDigis)

