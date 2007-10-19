#include "EventFilter/SiStripRawToDigi/test/stubs/macros/ObjectFilter.h"
  
const bool objectfilter::trigger(std::vector<SimpleGenParticle>& particles, std::vector<SimpleGenJet>& genjets, unsigned int bit) {

  unsigned short particlecount = 0;
  for (unsigned int i=0;i<particles.size();i++) {
    //Double isolated electron
    if ((bit == 34 && abs(particles[i].pid()) == 11 && particles[i].pt() > 10. && fabs(particles[i].eta()) < constants::tracker) ||
	//Double relaxed muon
	(bit == 49 && abs(particles[i].pid()) == 13 && particles[i].pt() > 3. && fabs(particles[i].eta()) < constants::tracker) ||
	//B-jet path 4
	(bit == 63 && abs(particles[i].pid()) == 5 && particles[i].pt() > 35. && fabs(particles[i].eta()) < constants::tracker) ||
	//Single tau
	(bit == 71 && abs(particles[i].pid()) == 15 && particles[i].pt() > 15. && fabs(particles[i].eta()) < constants::tracker) ||
	//Double tau 
	(bit == 73 && abs(particles[i].pid()) == 15 && particles[i].pt() > 15. && fabs(particles[i].eta()) < constants::tracker)) {
      particlecount++;
    }
  }

  unsigned short genjetcount = 0;
  for (unsigned int i=0;i<genjets.size();i++) {
    //B-jet path 4
    if ((bit == 63 && genjets[i].hcluster().et() > 120.)) {
      genjetcount++;
    }
  }

  return ((bit == 34 && particlecount >= 2) || (bit == 49 && particlecount >= 2) || (bit == 63 && particlecount >= 1 && genjetcount >= 2) || (bit == 71 && particlecount >= 1) || (bit == 73 && particlecount >= 2)) ? true : false;
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

const unsigned int objectfilter::muon(std::vector<SimpleMuon>& muons, const SimpleGenParticle& particle) {
  
  unsigned int index = 0;
  double dR = constants::large;
  for (unsigned int i = 0; i < muons.size(); i++) {
    if (SimpleTrack::dR(muons[i].track(),particle) < dR) {
      dR = SimpleTrack::dR(muons[i].track(),particle);
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
