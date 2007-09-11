#include "EventFilter/SiStripRawToDigi/test/stubs/macros/ObjectFilter.h"
  
const bool objectfilter::trigger(std::vector<SimpleGenParticle>& particles, unsigned int bit) {

  unsigned short count = 0;
  for (unsigned int i=0;i<particles.size();i++) {
    if (bit == 4 && abs(particles[i].pid()) == 11 && particles[i].pt() > 12. && fabs(particles[i].eta()) < constants::etaCut) {
      count++;
    }
  }
  if (bit == 4 && count >=2) return true;
  return false;
}
  
const unsigned int objectfilter::electron(std::vector<SimpleElectron>& electrons, const SimpleGenParticle& particle) {
  
  unsigned int index = 0;
 double dR = constants::large;
 for (unsigned int i = 0; i < electrons.size(); i++) {
   if (SimpleSCluster::dR(electrons[i].scluster(),particle) < dR) {
     dR = SimpleSCluster::dR(electrons[i].scluster(),particle);
     index = i;
   }
 }  
 return index;
}
  
const unsigned int objectfilter::jet(std::vector<SimpleJet>& jets, const SimpleGenParticle& particle) {

unsigned int index = 0;
 double dR = constants::large;
 for (unsigned int i = 0; i < jets.size(); i++) {
   if (SimpleHCluster::dR(jets[i].hcluster(),particle) < dR) {
     dR = SimpleHCluster::dR(jets[i].hcluster(),particle);
     index = i;
   }
 }
 return index;
}
