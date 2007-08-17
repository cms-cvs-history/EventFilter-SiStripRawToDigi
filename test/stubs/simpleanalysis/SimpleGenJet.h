#ifndef simpleanalysis_SimpleGenJet_H
#define simpleanalysis_SSimpleGenJet_H

class SimpleGenJet : public TObject {

 public:

  SimpleGenJet() : vx_(constants::invalid), vy_(constants::invalid), vz_(constants::invalid), hcluster_(), tag_(false) {;}

  SimpleGenJet(double vx, double vy, double vz, SimpleHCluster& hcluster) : vx_(vx), vy_(vy), vz_(vz), hcluster_(hcluster), tag_(false) {;}

  virtual ~SimpleGenJet() {;}

  const double vx() const {return vx_;}
  const double vy() const {return vy_;}
  const double vz() const {return vz_;}
  const SimpleHCluster hcluster() const {return hcluster_;}
  const bool tag() const {return tag_;}
  void tag(bool tag) {tag_ = tag;}

  bool operator < (const SimpleGenJet& compare) const {

    if (tag_ && !compare.tag()) return true;
    else if (compare.tag() && !tag_) return false;
    else if (compare.hcluster().et() < hcluster().et()) return true;
    else return false;
  }

 private:

  double vx_;
  double vy_;
  double vz_;
  SimpleHCluster hcluster_;
  bool tag_;

  ClassDef(SimpleGenJet,1)

};

#endif
