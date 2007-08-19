#include "EventFilter/SiStripRawToDigi/test/stubs/macros/ObjectFilter.h"

const bool objectfilter::singleelectron(std::vector<SimpleGenParticle>& particles) {
  
  for (std::vector<SimpleGenParticle>::const_iterator ipart = particles.begin(); ipart != particles.end(); ipart++) {
    if ((abs(ipart->pid()) == 11) && (ipart->pt() > 26.) && (fabs(ipart->eta()) < constants::etaCut)) return true;
  }
  return false;
}
  
const bool objectfilter::doubleelectron(std::vector<SimpleGenParticle>& particles) {
  
  unsigned short count = 0;
  for (std::vector<SimpleGenParticle>::const_iterator ipart = particles.begin(); ipart != particles.end(); ipart++) {
    if ((abs(ipart->pid()) == 11) && (ipart->pt() > 12.) && (fabs(ipart->eta()) < constants::etaCut)) count++;
  }
  return (count >= 2) ? true : false;
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
