#ifndef EventFilter_SiStripRawToDigi_PerformanceOverlay_H
#define EventFilter_SiStripRawToDigi_PerformanceOverlay_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/macros/Plots.h"

class PerformanceOverlay {

public:
  
  PerformanceOverlay(TFile*);
  ~PerformanceOverlay();

  void book();
  void read(std::string);
  void overlay();
  void unbook();

private:

  void overlay(Plots::TH1FType);
  void overlay(Plots::TProfileType); 
  void overlay(Plots::EfficiencyType);

  TFile* file_;
  std::vector<Plots> vplots_;
  std::vector<std::string> vlabels_;
  SimpleOverlay* overlay_;

};

#endif
