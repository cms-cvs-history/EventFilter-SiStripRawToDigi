#ifndef EventFilter_SiStripRawToDigi_PerformanceOverlay_H
#define EventFilter_SiStripRawToDigi_PerformanceOverlay_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

class PerformanceOverlay {

public:
  
  PerformanceOverlay(TFile*);
  ~PerformanceOverlay();

  void book();
  void overlay(std::string);
  void save(std::string);
  void unbook();

private:

  TFile* file_;
  SimpleOverlay* overlay_;

};

#endif
