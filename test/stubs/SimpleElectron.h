
#ifndef EventFilter_SiStripRawToDigi_SimpleElectron_h
#define EventFilter_SiStripRawToDigi_SimpleElectron_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleTrack.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSCluster.h"

class SimpleElectron : public TObject {

 public:

  SimpleElectron() : track(), scluster(), tag(false) {}

  SimpleElectron(SimpleTrack& Track,SimpleSCluster& Scluster) : track(Track), scluster(Scluster), tag(false) {}

  virtual ~SimpleElectron() {}

  bool operator < (const SimpleElectron& compare) const {

    if (tag && !compare.tag) return true;
    else if (compare.tag && !tag) return false;
    else if (compare.scluster.et() < scluster.et()) return true;
    else return false;
    
  }

  SimpleTrack track;
  SimpleSCluster scluster;
  bool tag;

  ClassDef(SimpleElectron,1)

};

#endif
