#ifndef EventFilter_SiStripRawToDigi_EventData_h
#define EventFilter_SiStripRawToDigi_EventData_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleParticle.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleTrack.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleElectron.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSiStripCluster.h"

#include "TObject.h"
#include <vector>

class EventData : public TObject {

 public:

  EventData() : mc(), sclusters(), electrons(), sistripclusters() {}
  virtual ~EventData() {}
   
  void clear() {
    mc.clear();
    sclusters.clear();
    electrons.clear();
    sistripclusters.clear();
  }

  void order() {
    sort(mc.begin(),mc.end());
    sort(sclusters.begin(),sclusters.end());
    sort(electrons.begin(),electrons.end());
    sort (sistripclusters.begin(),sistripclusters.end());
  }

  unsigned int electronNum(const double cut=0) const {
    unsigned int count=0;
    for (unsigned int i = 0; i < electrons.size(); i++) {
      if ((electrons[i].scluster.et() > cut) &&
	  (electrons[i].tag)) count++;
      else {break;}
    }
    return count;
  }

  std::vector<SimpleParticle> mc;
  std::vector<SimpleSCluster> sclusters;
  std::vector<SimpleElectron> electrons;
  std::vector<SimpleSiStripCluster> sistripclusters;

  ClassDef(EventData,1)

 };

#endif
