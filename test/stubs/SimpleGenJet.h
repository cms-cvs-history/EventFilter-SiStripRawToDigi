
#ifndef EventFilter_SiStripRawToDigi_SimpleGenJet_h
#define EventFilter_SiStripRawToDigi_SimpleGenJet_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleHCluster.h"

class SimpleGenJet : public TObject {

 public:

  SimpleGenJet() : vx_(constants::invalid), vy_(constants::invalid), vz_(constants::invalid), hcluster_(), tag_(false) {;}

  SimpleGenJet(double Vx, double Vy, double Vz, SimpleHCluster& Hcluster) : vx_(Vx), vy_(Vy), vz_(Vz), hcluster_(Hcluster), tag_(false) {;}

  virtual ~SimpleGenJet() {;}

  bool operator < (const SimpleGenJet& compare) const {

    if (tag_ && !compare.tag()) return true;
    else if (compare.tag() && !tag_) return false;
    else if (compare.hcluster().et() < hcluster().et()) return true;
    else return false;
  }

  const double vx() const {return vx_;}
  const double vy() const {return vy_;}
  const double vz() const {return vz_;}
  const SimpleHCluster& hcluster() const {return hcluster_;}
  const bool tag() const {return tag_;}

 private:

  double vx_;
  double vy_;
  double vz_;
  SimpleHCluster hcluster_;
  bool tag_;

  ClassDef(SimpleGenJet,1)

};

#endif
