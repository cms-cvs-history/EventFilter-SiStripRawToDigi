#ifndef EventFilter_SiStripRawToDigi_SimpleSCluster_h
#define EventFilter_SiStripRawToDigi_SimpleSCluster_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleParticle.h"

class SimpleSCluster : public TObject {

 public:

  SimpleSCluster() :  energy(constants::invalid), eta(constants::invalid), phi(constants::invalid) {}

  SimpleSCluster(double Energy,double Eta,double Phi) :  energy(Energy), eta(Eta), phi(Phi) {}

  virtual ~SimpleSCluster() {}

  static const double dR(const SimpleSCluster& a,const SimpleSCluster& b) {
    return sqrt((a.eta - b.eta)*(a.eta - b.eta) 
		+ (a.phi - b.phi)*(a.phi - b.phi));
  }

  static const double dR(const SimpleSCluster& a,const SimpleParticle& b) {
    return sqrt((a.eta - b.eta)*(a.eta - b.eta) 
		+ (a.phi - b.phi)*(a.phi - b.phi));
  }
  
  const double et() const {
    return energy * sin(2*atan(exp(-eta)));
  }

  bool operator < (const SimpleSCluster& compare) const {
    if (compare.et() < et()) return true;
    return false;
  }

  double energy;
  double eta;
  double phi;

  ClassDef(SimpleSCluster,1)
};

#endif
