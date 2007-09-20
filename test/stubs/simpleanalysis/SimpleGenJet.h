#ifndef simpleanalysis_SimpleGenJet_H
#define simpleanalysis_SSimpleGenJet_H

class SimpleGenJet : public TObject {

 public:

  /** Constructors */

  SimpleGenJet() : vx_(constants::invalid), vy_(constants::invalid), vz_(constants::invalid), hcluster_(), tag_(false) {}

  SimpleGenJet(double vx, double vy, double vz, SimpleHCluster hcluster) : vx_(vx), vy_(vy), vz_(vz), hcluster_(hcluster), tag_(false) {}

  /** Desctructor */

  virtual ~SimpleGenJet() {}

  /** Getters */

  const double vx() const {return vx_;}

  const double vy() const {return vy_;}

  const double vz() const {return vz_;}

  const SimpleHCluster hcluster() const {return hcluster_;}

  const bool tag() const {return tag_;}

  /** Setter */

  void tag(bool tag) {tag_ = tag;}

 private:

  double vx_;
  double vy_;
  double vz_;
  SimpleHCluster hcluster_;
  bool tag_;

  ClassDef(SimpleGenJet,1)

};

inline bool operator < (const SimpleGenJet& one,const SimpleGenJet& two) 
{
  return ((one.tag() && !two.tag()) || (one.tag() == two.tag() && one.hcluster().et() < two.hcluster().et())) ? true : false;
}

#endif
