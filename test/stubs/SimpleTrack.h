#ifndef EventFilter_SiStripRawToDigi_SimpleTrack_h
#define EventFilter_SiStripRawToDigi_SimpleTrack_h


class SimpleTrack : public TObject {

 public:

  SimpleTrack() :  innerpt_(constants::invalid), innereta_(constants::invalid), innerphi_(constants::invalid), innervx_(constants::invalid), innervy_(constants::invalid), innervz_(constants::invalid), outerpt_(constants::invalid), outereta_(constants::invalid), outerphi_(constants::invalid), outervx_(constants::invalid), outervy_(constants::invalid), outervz_(constants::invalid), charge_(constants::invalid32) {}

  SimpleTrack(double InnerPt, double InnerEta, double InnerPhi, double InnerVx, double InnerVy, double InnerVz, double OuterPt, double OuterEta, double OuterPhi, double OuterVx, double OuterVy, double OuterVz, int Charge) :   innerpt_(InnerPt), innereta_(InnerEta), innerphi_(InnerPhi), innervx_(InnerVx), innervy_(InnerVy), innervz_(InnerVz), outerpt_(OuterPt), outereta_(OuterEta), outerphi_(OuterPhi), outervx_(OuterVx), outervy_(OuterVy), outervz_(OuterVz), charge_(Charge) {}

  virtual ~SimpleTrack() {}

  bool operator < (const SimpleTrack& compare) const {
    if (compare.innerpt_ < innerpt_) return true;
    return false;
  }

  const double innerPt() const {return innerpt_;}
  const double innerEta() const {return innereta_;}
  const double innerPhi() const { return innerphi_;}
  const double innerVx() const {return innervx_;}
  const double innerVy() const {return innervy_;}
  const double innerVz() const {return innervz_;}
  const double outerPt() const {return outerpt_;}
  const double outerEta() const {return outereta_;}
  const double outerPhi() const {return outerphi_;}
  const double outerVx() const {return outervx_;}
  const double outerVy() const {return outervy_;}
  const double outerVz() const {return outervz_;}
  const int charge() const {return charge_;}

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

  ClassDef(SimpleTrack,1)
};

#endif
