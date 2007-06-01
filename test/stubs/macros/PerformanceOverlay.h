#ifndef EventFilter_SiStripRawToDigi_PerformanceOverlay_H
#define EventFilter_SiStripRawToDigi_PerformanceOverlay_H

#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include <vector>
#include "EventFilter/SiStripRawToDigi/test/stubs/Overlay.h"

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
  Overlay* overlay_;

};

#endif
