#ifndef EventFilter_SiStripRawToDigi_ObjectFilter_H
#define EventFilter_SiStripRawToDigi_ObjectFilter_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

namespace objectfilter {
  
  const bool trigger(std::vector<SimpleGenParticle>&, std::vector<SimpleGenJet>&, unsigned int);
  
  const unsigned int electron(const std::vector<SimpleElectron>&, const SimpleGenParticle&);

  const unsigned int muon(const std::vector<SimpleMuon>&, const SimpleGenParticle&);
  
  const unsigned int jet(const std::vector<SimpleJet>&, const SimpleGenParticle&);
}

#endif
