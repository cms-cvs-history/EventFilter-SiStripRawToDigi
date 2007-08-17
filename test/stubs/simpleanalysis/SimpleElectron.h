#ifndef simpleanalysis_SimpleElectron_H
#define simpleanalysis_SimpleElectron_H

class SimpleElectron : public TObject {

 public:

  SimpleElectron() : track_(), scluster_(), classification_(constants::invalid32), tag_(false) {}

  SimpleElectron(SimpleTrack& track, SimpleSCluster& scluster, int classification) : track_(track), scluster_(scluster), classification_(classification), tag_(false) {}

  virtual ~SimpleElectron() {}

  const SimpleTrack track() const {return track_;}
  const SimpleSCluster scluster() const {return scluster_;}
  const int classification() const {return classification_;}
  const bool tag() const {return tag_;}
  const double edivp() const {return scluster().energy()/track().innerP();}
  const double isolation(const std::vector<SimpleTrack>& tracks) const {return constants::invalid;}
  void tag(bool tag) {tag_ = tag;}

  static const double parentMass(const SimpleElectron& a, const SimpleElectron& b) {

  double xa = a.scluster().et() * sin(a.track().innerPhi());
  double ya = a.scluster().et() * cos(a.track().innerPhi());
  double za = a.scluster().et() * 1./(tan(2*atan(exp(-a.track().innerEta()))));
  double xb = b.scluster().et() * sin(b.track().innerPhi());
  double yb = b.scluster().et() * cos(b.track().innerPhi());
  double zb = b.scluster().et() * 1./(tan(2*atan(exp(-b.track().innerEta()))));
  
  return sqrt(2*sqrt(xa*xa + ya*ya + za*za)*sqrt(xb*xb + yb*yb + zb*zb)*(1-((xa*xb + ya*yb + za*zb)/(sqrt(xa*xa + ya*ya + za*za) * sqrt(xb*xb + yb*yb + zb*zb)))));
}

  bool operator < (const SimpleElectron& compare) const {

    if (tag_ && !compare.tag()) return true;
    else if (compare.tag() && !tag_) return false;
    else if (compare.scluster().et() < scluster().et()) return true;
    else return false;
  }

 private:

  SimpleTrack track_;
  SimpleSCluster scluster_;
  int classification_;
  bool tag_;

  ClassDef(SimpleElectron,1)

};

#endif
