#ifndef simpleanalysis_SimpleTrack_H
#define simpleanalysis_SimpleTrack_H

class SimpleTrack : public TObject {

 public:

  SimpleTrack() :  innerpt_(constants::invalid), innereta_(constants::invalid), innerphi_(constants::invalid), innervx_(constants::invalid), innervy_(constants::invalid), innervz_(constants::invalid), outerpt_(constants::invalid), outereta_(constants::invalid), outerphi_(constants::invalid), outervx_(constants::invalid), outervy_(constants::invalid), outervz_(constants::invalid), charge_(constants::invalid32), validHits_(constants::invalid16) {}

  SimpleTrack(double InnerPt, double InnerEta, double InnerPhi, double InnerVx, double InnerVy, double InnerVz, double OuterPt, double OuterEta, double OuterPhi, double OuterVx, double OuterVy, double OuterVz, int Charge, unsigned short ValidHits) :   innerpt_(InnerPt), innereta_(InnerEta), innerphi_(InnerPhi), innervx_(InnerVx), innervy_(InnerVy), innervz_(InnerVz), outerpt_(OuterPt), outereta_(OuterEta), outerphi_(OuterPhi), outervx_(OuterVx), outervy_(OuterVy), outervz_(OuterVz), charge_(Charge), validHits_(ValidHits) {}

  virtual ~SimpleTrack() {}

  const double innerPt() const {return innerpt_;}
  const double innerEta() const {return innereta_;}
  const double innerPhi() const { return innerphi_;}
  const double innerVx() const {return innervx_;}
  const double innerVy() const {return innervy_;}
  const double innerVz() const {return innervz_;}
  const double innerP() const {return innerpt_/sin(2*atan(exp(-innereta_)));}
  const double outerPt() const {return outerpt_;}
  const double outerEta() const {return outereta_;}
  const double outerPhi() const {return outerphi_;}
  const double outerVx() const {return outervx_;}
  const double outerVy() const {return outervy_;}
  const double outerVz() const {return outervz_;}
  const double outerP() const {return outerpt_/sin(2*atan(exp(-outereta_)));}
  const double brem() const {return (innerP() - outerP())/innerP();}
  const int charge() const {return charge_;}
  const unsigned short hits() const {return validHits_;}

 private:

  double innerpt_;
  double innereta_;
  double innerphi_;
  double innervx_;
  double innervy_;
  double innervz_;
  double outerpt_;
  double outereta_;
  double outerphi_;
  double outervx_;
  double outervy_;
  double outervz_;
  int charge_;
  unsigned short validHits_;

  ClassDef(SimpleTrack,1)
};

inline bool operator < (const SimpleTrack& one,const SimpleTrack& two) {
  return (one.innerPt() < two.innerPt()) ? true : false;
}

#endif
