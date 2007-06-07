#ifndef EventFilter_SiStripRawToDigi_EventData_h
#define EventFilter_SiStripRawToDigi_EventData_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleParticle.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleTrack.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleElectron.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSiStripCluster.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleJet.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleTrigger.h"

#include "TObject.h"
#include <vector>

class EventData : public TObject {

 public:

  EventData() : mc_(), sclusters_(), electrons_(), taus_(), sistripclusters_(), trigger_() {}
  virtual ~EventData() {}
   
  std::vector<SimpleParticle>& mc() {return mc_;}
  std::vector<SimpleSCluster>& sClusters() {return sclusters_;}
  std::vector<SimpleElectron>& electrons() {return electrons_;}
  std::vector<SimpleJet>& taus() {return taus_;}
  std::vector<SimpleSiStripCluster>& sistripClusters() {return sistripclusters_;}
  SimpleTrigger& trigger() {return trigger_;}

  void clear() {
    mc_.clear();
    sclusters_.clear();
    electrons_.clear();
    taus_.clear();
    sistripclusters_.clear();
    trigger_.reset();
  }

  void order() {
    std::sort(mc_.begin(),mc_.end());
    std::sort(sclusters_.begin(),sclusters_.end());
    std::sort(electrons_.begin(),electrons_.end());
    std::sort(taus_.begin(),taus_.end());
    std::sort (sistripclusters_.begin(),sistripclusters_.end());
  }

  unsigned int electronNum(const double cut=0) const {
    unsigned int count=0;
    for (unsigned int i = 0; i < electrons_.size(); i++) {
      if ((electrons_[i].scluster().et() > cut) &&
	  (electrons_[i].tag())) count++;
      else {break;}
    }
    return count;
  }

 unsigned int tauNum(const double cut=0) const {
    unsigned int count=0;
    for (unsigned int i = 0; i < taus_.size(); i++) {
      if ((taus_[i].hcluster().et() > cut) &&
	  (taus_[i].tag())) count++;
      else {break;}
    }
    return count;
  }

  private :

  std::vector<SimpleParticle> mc_;
  std::vector<SimpleSCluster> sclusters_;
  std::vector<SimpleElectron> electrons_;
  std::vector<SimpleJet> taus_;
  std::vector<SimpleSiStripCluster> sistripclusters_;
  SimpleTrigger trigger_;

  ClassDef(EventData,1)

 };

#endif
