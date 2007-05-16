
#ifndef EventFilter_SiStripRawToDigi_SimpleElectron_h
#define EventFilter_SiStripRawToDigi_SimpleElectron_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleTrack.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSCluster.h"

class SimpleElectron : public TObject {

 public:

  SimpleElectron() : track_(), scluster_(), tag_(false) {}

  SimpleElectron(SimpleTrack& Track,SimpleSCluster& Scluster) : track_(Track), scluster_(Scluster), tag_(false) {}

  virtual ~SimpleElectron() {}

  const SimpleTrack& track() const {return track_;}
  const SimpleSCluster& scluster() const {return scluster_;}
  const bool tag() const {return tag_;}
  void tag(bool Tag) {tag_=Tag;}

  bool operator < (const SimpleElectron& compare) const {

    if (tag_ && !compare.tag()) return true;
    else if (compare.tag() && !tag_) return false;
    else if (compare.scluster().et() < scluster().et()) return true;
    else return false;
    
  }

 private:

  SimpleTrack track_;
  SimpleSCluster scluster_;
  bool tag_;

  ClassDef(SimpleElectron,1)

};

#endif
