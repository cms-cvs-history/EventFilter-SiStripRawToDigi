#ifndef EventFilter_SiStripRawToDigi_SiStripPerformanceFigures_H
#define EventFilter_SiStripRawToDigi_SiStripPerformanceFigures_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/macros/SiStripPerformancePlots.h"

class SiStripPerformanceFigures {

public:
  
  SiStripPerformanceFigures(TFile*);
  ~SiStripPerformanceFigures();

  void book();
  void read(std::string);
  void overlay();
  void unbook();

private:

  void overlay(SiStripPerformancePlots::TH1FType);
  void overlay(SiStripPerformancePlots::TProfileType); 
  void overlay(SiStripPerformancePlots::EfficiencyType);

  TFile* file_;
  std::vector<SiStripPerformancePlots> vplots_;
  std::vector<std::string> vlabels_;
  SimpleOverlay* overlay_;

};

#endif
