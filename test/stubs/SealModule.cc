#include "FWCore/Framework/interface/MakerMacros.h"
#include "PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE();

#include "EventFilter/SiStripRawToDigi/test/stubs/AnalyzeSiStripDigis.h"
DEFINE_ANOTHER_FWK_MODULE(AnalyzeSiStripDigis);

