
#ifndef EventFilter_SiStripRawToDigi_SimpleParticle_h
#define EventFilter_SiStripRawToDigi_SimpleParticle_h

#include "EventFilter/SiStripRawToDigi/test/stubs/Constants.h"
#include "TObject.h"
#include <string>
#include <cmath>

class SimpleParticle : public TObject {

 public: 

  SimpleParticle() : pt(constants::invalid), eta(constants::invalid), phi(constants::invalid), vx(constants::invalid), vy(constants::invalid), vz(constants::invalid), flavour("null"), charge(constants::invalid32) {}

  SimpleParticle(double Pt,double Eta,double Phi,double Vx,double Vy,double Vz, std::string Flavour="", int Charge=constants::invalid32) :  pt(Pt), eta(Eta), phi(Phi), vx(Vx), vy(Vy), vz(Vz), flavour(Flavour), charge(Charge) {}

  virtual ~SimpleParticle() {}

  static const double dR(const SimpleParticle& a, const SimpleParticle& b) {
    return sqrt((a.eta - b.eta)*(a.eta - b.eta) 
		+ (a.phi - b.phi)*(a.phi - b.phi));
  }

  static const double dVertex(const SimpleParticle& a, const SimpleParticle& b) {
    return sqrt((a.vx - b.vx)*(a.vx - b.vx) + 
		(a.vy - b.vy)*(a.vy - b.vy) + 
		(a.vz - b.vz)*(a.vz - b.vz)); 
  }
 
  const double p() const {
    return pt/sin(2*atan(exp(-eta)));
  }

  const double px() const {
    return pt * sin(phi);
  }

  const double py() const {
    return pt * cos(phi);
  }

  const double pz() const {
    return pt * 1/(tan(2*atan(exp(-eta))));
  }

  const double etaECAL() const {
    float zhit =  constants::rEcal*sinh(eta) + vz;
    
    if (fabs(zhit) < constants::zEcal) {
      float thetahit = 0.0;
      if (zhit!=0.0) thetahit = atan(constants::rEcal/zhit);
      if (thetahit<0.) thetahit = thetahit + constants::pi;
      float etahit = -log(tan(0.5*thetahit));
      return etahit;
    } 
    
    else {
      float rhit = (constants::zEcal-vz) / sinh(eta);
      float thetahit = 0.0;
      if (rhit!=0.0) thetahit = atan(rhit/constants::zEcal);
      if (thetahit<0.) thetahit = thetahit + constants::pi;
      float etahit = -log(tan(0.5*thetahit));
      return etahit;
    }
  }

  const double phiECAL() const {
    double phicorr;
    if( fabs(eta) <=  constants::ecEcal) {
      phicorr = phi - asin(constants::bend/pt)*charge;
      if (phicorr >  constants::pi) {phicorr -= 2.*constants::pi;}
      else if (phicorr < -1.*constants::pi) {phicorr += 2.*constants::pi;}}
    
    else {
      double rhit = constants::zendm/sinh (fabs(eta));
      phicorr = phi - asin((rhit/constants::rbarm)*constants::bend/pt)*charge;
      if (phicorr >  constants::pi) {phicorr -= 2.*constants::pi;}
      else if (phicorr < -1.*constants::pi) {phicorr += 2.*constants::pi;}}
    
    return phicorr;
  }
  
  bool operator < (const SimpleParticle& compare) const {
    if (compare.pt < pt) return true;
    return false; 
  }

  double pt;
  double eta;
  double phi;
  double vx;
  double vy;
  double vz;
  std::string flavour;
  int charge;

 private:

  ClassDef(SimpleParticle,1)
};

#endif
