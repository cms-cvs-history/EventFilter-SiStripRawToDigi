#ifndef simpleanalysis_SimpleSCluster_H
#define simpleanalysis_SimpleSCluster_H

class SimpleSCluster : public TObject {

 public:

  SimpleSCluster() :  rawenergy_(constants::invalid), energy_(constants::invalid), eta_(constants::invalid), phi_(constants::invalid), hdivem_(constants::invalid), energy2x2_(constants::invalid), energy3x3_(constants::invalid), energy5x5_(constants::invalid), energySeed_(constants::invalid), covEtaEta_(constants::invalid) {}

  SimpleSCluster(double rawenergy, double energy, double eta, double phi, double energy2x2=constants::invalid, double energy3x3=constants::invalid, double energy5x5=constants::invalid, double energySeed=constants::invalid, double covEtaEta=constants::invalid, double hdivem=constants::invalid) :  rawenergy_(rawenergy), energy_(energy), eta_(eta), phi_(phi), hdivem_(hdivem), energy2x2_(energy2x2), energy3x3_(energy3x3), energy5x5_(energy5x5), energySeed_(energySeed), covEtaEta_(covEtaEta)  {}

  virtual ~SimpleSCluster() {}

  const double rawenergy() const {return rawenergy_;}
  const double energy() const {return energy_;}
  const double eta() const {return eta_;}
  const double phi() const {return phi_;}
  const double hdivem() const {return hdivem_;}
  const double energy2x2() const {return energy2x2_;}
  const double energy3x3() const {return energy3x3_;}
  const double energy5x5() const {return energy5x5_;}
  const double energySeed() const {return energySeed_;}
  const double covEtaEta() const {return covEtaEta_;}
  const double et() const {return energy_ * sin(2*atan(exp(-eta_)));}
  const double ex() const {return et() * sin(phi_);}
  const double ey() const {return et() * cos(phi_);}
  const double ez() const {return et() * 1/(tan(2*atan(exp(-eta_))));}

  static const double dR(const SimpleSCluster& a,const SimpleSCluster& b) {
    return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		+ (a.phi() - b.phi())*(a.phi() - b.phi()));
  }

  static const double dR(const SimpleSCluster& a,const SimpleGenParticle& b) {
    return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		+ (a.phi() - b.phi())*(a.phi() - b.phi()));
  }

 private:

  double rawenergy_;
  double energy_;
  double eta_;
  double phi_;
  double hdivem_;
  double energy2x2_;
  double energy3x3_;
  double energy5x5_;
  double energySeed_;
  double covEtaEta_; 

  ClassDef(SimpleSCluster,1)
};

inline bool operator < (const SimpleSCluster& one,const SimpleSCluster& two) {
  return (one.et() < two.et()) ? true : false;
}

#endif
