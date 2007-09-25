#ifndef EventFilter_SiStripRawToDigi_ObjectFilter_H
#define EventFilter_SiStripRawToDigi_ObjectFilter_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

namespace objectfilter {
  
  const bool trigger(std::vector<SimpleGenParticle>&, unsigned int);
  
  const unsigned int electron(std::vector<SimpleElectron>&, const SimpleGenParticle&);

  const unsigned int muon(std::vector<SimpleMuon>&, const SimpleGenParticle&);
  
  const unsigned int jet(std::vector<SimpleJet>&, const SimpleGenParticle&);
}

#endif
