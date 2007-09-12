#ifndef simpleanalysis_SimpleHCluster_H
#define simpleanalysis_SimpleHCluster_H

class SimpleHCluster : public TObject {

 public:

  /** Constructor */

  SimpleHCluster() : rawEnergy_(constants::invalid), energy_(constants::invalid), eta_(constants::invalid), phi_(constants::invalid) {}

  SimpleHCluster(double RawEnergy, double Energy, double Eta, double Phi) : rawEnergy_(RawEnergy), energy_(Energy), eta_(Eta), phi_(Phi) {}

  /** Desctructor */
 
  virtual ~SimpleHCluster() {}

  /** Getters */

  const double eta() const {return eta_;}

  const double phi() const {return phi_;}

  /** Utility */

  const double rawEt() const 
    {
      return rawEnergy_ * sin(2*atan(exp(-eta_)));
    }

  const double et() const 
    {
      return energy_ * sin(2*atan(exp(-eta_)));
    }

  static const double dR(const SimpleHCluster& a,const SimpleHCluster& b) 
    {
      return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		  + (a.phi() - b.phi())*(a.phi() - b.phi()));
    }
  
  static const double dR(const SimpleHCluster& a,const SimpleGenParticle& b) 
    {
      return sqrt((a.eta() - b.eta())*(a.eta() - b.eta()) 
		  + (a.phi() - b.phi())*(a.phi() - b.phi()));
    }
  
 private:

  double rawEnergy_;
  double energy_;
  double eta_;
  double phi_;

  ClassDef(SimpleHCluster,1)
};

inline bool operator < (const SimpleHCluster& one,const SimpleHCluster& two) 
{
  return (one.et() < two.et()) ? true : false;
}

#endif
