#ifndef simpleanalysis_SimpleSiStripCluster_H
#define simpleanalysis_SimpleSiStripCluster_H

class SimpleSiStripCluster : public TObject {

 public:

  SimpleSiStripCluster() :   detId_(constants::invalid32), firstStrip_(constants::invalid16), amplitudes_(constants::invalid16), baryCenter_(constants::invalid) {}

  SimpleSiStripCluster(unsigned int DetId, unsigned short FirstStrip, unsigned short Amplitudes, double BaryCenter) :   detId_(DetId), firstStrip_(FirstStrip), amplitudes_(Amplitudes), baryCenter_(BaryCenter) {}


  virtual ~SimpleSiStripCluster() {}

  const unsigned int detId() const {return detId_;}
  const unsigned short firstStrip() const {return firstStrip_;}
  const unsigned short amplitudes() const {return amplitudes_;}
  const double baryCenter() const {return baryCenter_;}

  bool operator < (const SimpleSiStripCluster& compare) const {
    if (compare.detId_ < detId_) return true;
    else if ((compare.detId_ == detId_ &&
	      compare.firstStrip_ < firstStrip_)) return true;
    else return false;
  }

  unsigned int detId_;
  unsigned short firstStrip_;
  unsigned short amplitudes_;
  double baryCenter_;

  ClassDef(SimpleSiStripCluster,1)
};

#endif
