#ifndef simpleanalysis_SimpleMuon_H
#define simpleanalysis_SimpleMuon_H

class SimpleMuon : public TObject {

 public:

  /** Constructors */

  SimpleMuon() : track_(), tag_(false) {}

  SimpleMuon(SimpleTrack& track) : track_(track), tag_(false) {}
  
  /** Destructor */
  
  virtual ~SimpleMuon() {}

  /** Getters */

  const SimpleTrack track() const {return track_;}

  const bool tag() const {return tag_;}

  /** Setter */

  void tag(bool tag) {tag_ = tag;}

  /** Utility */

  static const double parentMass(const SimpleMuon& a, const SimpleMuon& b) 
    {
      double xa = a.track().innerPt() * sin(a.track().innerPhi());
      double ya = a.track().innerPt() * cos(a.track().innerPhi());
      double za = a.track().innerPt() * 1./(tan(2*atan(exp(-a.track().innerEta()))));
      double xb = b.track().innerPt() * sin(b.track().innerPhi());
      double yb = b.track().innerPt() * cos(b.track().innerPhi());
      double zb = b.track().innerPt() * 1./(tan(2*atan(exp(-b.track().innerEta()))));
      
      return sqrt(2*sqrt(xa*xa + ya*ya + za*za)*sqrt(xb*xb + yb*yb + zb*zb)*(1-((xa*xb + ya*yb + za*zb)/(sqrt(xa*xa + ya*ya + za*za) * sqrt(xb*xb + yb*yb + zb*zb)))));
    }
  
 private:

  SimpleTrack track_;
  bool tag_;

  ClassDef(SimpleMuon,1)
};

inline bool operator < (const SimpleMuon& one, const SimpleMuon& two) 
{
  return ((one.tag() && !two.tag()) || (one.tag() == two.tag() && one.track().innerPt() < two.track().innerPt())) ? true : false;
}

#endif
