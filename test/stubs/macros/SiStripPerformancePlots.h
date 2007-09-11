#ifndef EventFilter_SiStripRawToDigi_SiStripPerformancePlots_H
#define EventFilter_SiStripRawToDigi_SiStripPerformancePlots_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

class SiStripPerformancePlots {
  
 public:

  enum TH1FType{TIME=0,FRAC=1,CLUSTERSIZE=2,OCCUPANCY=3};

  enum TProfileType{TIMEVSOCCUPANCY=0,TIMEVSDIGIS=1,TIMEVSCLUSTERS=2,TIMEVSCLUSTERSIZE=3,TIMEVSFRAC=4};

  enum EfficiencyType{HLT=0,PT1=1,PT2=2,ETA1=3,ETA2=4,ETA3=5};
  
  SiStripPerformancePlots();
  ~SiStripPerformancePlots();
  
  void book();
  void unbook();
  void format();
  void normalise();
  void save(TDirectory*);
  void read(TDirectory*);
  TH1F* get(TH1FType);
  TProfile* get(TProfileType);
  SimpleEfficiency* get(EfficiencyType);
  
 private:
  
  std::vector<TH1F*> th1fs_;
  std::vector<TProfile*> tprofiles_;
  std::vector<SimpleEfficiency*> efficiencies_;
  
 };

#endif
