#ifndef EventFilter_SiStripRawToDigi_PerformanceAnalysis_H
#define EventFilter_SiStripRawToDigi_PerformanceAnalysis_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/macros/Plots.h"

class PerformanceAnalysis {

public:
  
  PerformanceAnalysis(TFile*,std::string);
  ~PerformanceAnalysis();

  enum Trigger {NONE=0,ELECTRON_SINGLE=1,ELECTRON_DOUBLE=2,TAU_SINGLE=3,TAU_DOUBLE=4};

  void book();
  void format();
  void analyze(Trigger=NONE);
  void save();
  void unbook();

private:

  void electron();
  void tau();
  const double occupancy();

  //File
  TFile* file_;
  TTree* tree_;

  //Data
  SimpleEventData* data_;
  double time_;
  unsigned int nchans_;
  unsigned int nunpackedchans_;

  //Timing
  Plots plots_;
};

#endif
