
#ifndef EventFilter_SiStripRawToDigi_SimpleParticle_h
#define EventFilter_SiStripRawToDigi_SimpleParticle_h

#include "EventFilter/SiStripRawToDigi/test/stubs/Constants.h"
#include "TObject.h"
#include <string>
#include <cmath>

class SimpleParticle : public TObject {

 public: 

  SimpleParticle() : pt_(constants::invalid), eta_(constants::invalid), phi_(constants::invalid), vx_(constants::invalid), vy_(constants::invalid), vz_(constants::invalid), pid_(constants::invalid32), charge_(constants::invalid32) {}

  SimpleParticle(double Pt,double Eta,double Phi,double Vx,double Vy,double Vz, int Pid, int Charge) :  pt_(Pt), eta_(Eta), phi_(Phi), vx_(Vx), vy_(Vy), vz_(Vz), pid_(Pid), charge_(Charge) {}

  virtual ~SimpleParticle() {}

  const double pt() const {return pt_;}
  const double eta() const {return eta_;}
  const double phi() const {return phi_;}
  const double vx() const {return vx_;}
  const double vy() const {return vy_;}
  const double vz() const {return vz_;}
  const double pid() const {return pid_;}
  const double charge() const {return charge_;}
  const double p() const {return pt_/sin(2*atan(exp(-eta_)));}
  const double px() const {return pt_ * sin(phi_);}
  const double py() const {return pt_ * cos(phi_);}
  const double pz() const {return pt_ * 1/(tan(2*atan(exp(-eta_))));}

  static const double dR(const SimpleParticle& a, const SimpleParticle& b) {
    return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		+ (a.phi() - b.phi())*(a.phi() - b.phi()));
  }
  
  static const double dVertex(const SimpleParticle& a, const SimpleParticle& b) {
    return sqrt((a.vx() - b.vx())*(a.vx() - b.vx()) + 
		(a.vy() - b.vy())*(a.vy() - b.vy()) + 
		(a.vz() - b.vz())*(a.vz() - b.vz())); 
  }
  
  const double etaECAL() const {
    float zhit =  constants::rEcal*sinh(eta_) + vz_;
    
    if (fabs(zhit) < constants::zEcal) {
      float thetahit = 0.0;
      if (zhit!=0.0) thetahit = atan(constants::rEcal/zhit);
      if (thetahit<0.) thetahit = thetahit + constants::pi;
      float etahit = -log(tan(0.5*thetahit));
      return etahit;
    } 
    
    else {
      float rhit = (constants::zEcal-vz_) / sinh(eta_);
      float thetahit = 0.0;
      if (rhit!=0.0) thetahit = atan(rhit/constants::zEcal);
      if (thetahit<0.) thetahit = thetahit + constants::pi;
      float etahit = -log(tan(0.5*thetahit));
      return etahit;
    }
  }

  const double phiECAL() const {
    double phicorr;
    if( fabs(eta_) <=  constants::ecEcal) {
      phicorr = phi_ - asin(constants::bend/pt_)*charge_;
      if (phicorr >  constants::pi) {phicorr -= 2.*constants::pi;}
      else if (phicorr < -1.*constants::pi) {phicorr += 2.*constants::pi;}}
    
    else {
      double rhit = constants::zendm/sinh (fabs(eta_));
      phicorr = phi_ - asin((rhit/constants::rbarm)*constants::bend/pt_)*charge_;
      if (phicorr >  constants::pi) {phicorr -= 2.*constants::pi;}
      else if (phicorr < -1.*constants::pi) {phicorr += 2.*constants::pi;}}
    
    return phicorr;
  }
  
  bool operator < (const SimpleParticle& compare) const {
    if (compare.pt() < pt_) return true;
    return false; 
  }

 private:

  double pt_;
  double eta_;
  double phi_;
  double vx_;
  double vy_;
  double vz_;
  int pid_;
  int charge_;

  ClassDef(SimpleParticle,1)
};

#endif
