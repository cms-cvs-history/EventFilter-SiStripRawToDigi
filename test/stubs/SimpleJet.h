
#ifndef EventFilter_SiStripRawToDigi_SimpleJet_h
#define EventFilter_SiStripRawToDigi_SimpleJet_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleGenJet.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleTrack.h"

class SimpleJet : public SimpleGenJet {

 public:

  SimpleJet() : SimpleGenJet(), tracks_(), disc_(constants::invalid) {;}

  SimpleJet(double Vx, double Vy, double Vz, SimpleHCluster& Hcluster, std::vector<SimpleTrack>& Tracks, double Disc) : SimpleGenJet(Vx,Vy,Vz,Hcluster), tracks_(Tracks), disc_(Disc) {;}

    virtual ~SimpleJet() {;}

  const std::vector<SimpleTrack>& tracks() const {return tracks_;}
  const double discriminator() const {return disc_;}

  bool operator < (const SimpleJet& compare) const {

    if (tag() && !compare.tag()) return true;
    else if (compare.tag() && !tag()) return false;
    else if (compare.hcluster().et() < hcluster().et()) return true;
    else return false;
  }

 private:

  std::vector<SimpleTrack> tracks_;
  double disc_;

  ClassDef(SimpleJet,1)

};

#endif
