#ifndef simpleanalysis_SimpleSiStripCluster_H
#define simpleanalysis_SimpleSiStripCluster_H

class SimpleSiStripCluster : public TObject {

 public:

  SimpleSiStripCluster() :   detid_(constants::invalid32), firststrip_(constants::invalid16), amplitudes_(constants::invalid16), barycenter_(constants::invalid) {}

  SimpleSiStripCluster(unsigned int detid, unsigned short firststrip, unsigned short amplitudes, double barycenter) :   detid_(detid), firststrip_(firststrip), amplitudes_(amplitudes), barycenter_(barycenter) {}


  virtual ~SimpleSiStripCluster() {}

  const unsigned int detid() const {return detid_;}
  const unsigned short firststrip() const {return firststrip_;}
  const unsigned short amplitudes() const {return amplitudes_;}
  const double barycenter() const {return barycenter_;}

  unsigned int detid_;
  unsigned short firststrip_;
  unsigned short amplitudes_;
  double barycenter_;

  ClassDef(SimpleSiStripCluster,1)
};

inline bool operator < (const SimpleSiStripCluster& one,const SimpleSiStripCluster& two) {
  return ((one.detid() < two.detid()) || (one.detid() == two.detid() && one.firststrip() < two.firststrip())) ? true : false;
}

#endif
