#ifndef EventFilter_SiStripRawToDigi_ObjectFilter_H
#define EventFilter_SiStripRawToDigi_ObjectFilter_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

namespace objectfilter {
  
  const bool singleelectron(std::vector<SimpleGenParticle>&);
  
  const bool doubleelectron(std::vector<SimpleGenParticle>&);
  
  const unsigned int electron(std::vector<SimpleElectron>&, const SimpleGenParticle&);
  
  const unsigned int jet(std::vector<SimpleJet>&, const SimpleGenParticle&);
}

#endif
