#ifndef EventFilter_SiStripRawToDigi_SimpleSCluster_h
#define EventFilter_SiStripRawToDigi_SimpleSCluster_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleParticle.h"

class SimpleSCluster : public TObject {

 public:

  SimpleSCluster() :  energy_(constants::invalid), eta_(constants::invalid), phi_(constants::invalid) {}

  SimpleSCluster(double Energy,double Eta,double Phi) :  energy_(Energy), eta_(Eta), phi_(Phi) {}

  virtual ~SimpleSCluster() {}

  const double energy() const {return energy_;}
  const double eta() const {return eta_;}
  const double phi() const {return phi_;}
  const double et() const {return energy_ * sin(2*atan(exp(-eta_)));}

  static const double dR(const SimpleSCluster& a,const SimpleSCluster& b) {
    return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		+ (a.phi() - b.phi())*(a.phi() - b.phi()));
  }

  static const double dR(const SimpleSCluster& a,const SimpleParticle& b) {
    return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		+ (a.phi() - b.phi())*(a.phi() - b.phi()));
  }

  bool operator < (const SimpleSCluster& compare) const {
    if (compare.et() < et()) return true;
    return false;
  }

 private:

  double energy_;
  double eta_;
  double phi_;

  ClassDef(SimpleSCluster,1)
};

#endif
