#ifndef EventFilter_SiStripRawToDigi_SiStripPerformanceAnalysis_H
#define EventFilter_SiStripRawToDigi_SiStripPerformanceAnalysis_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/macros/SiStripPerformancePlots.h"

class SiStripPerformanceAnalysis {

public:
  
  SiStripPerformanceAnalysis(TFile*,std::string);
  ~SiStripPerformanceAnalysis();

  void book();
  void format();
  void timing();
  void trigger(unsigned int);
  void electron(unsigned int);
  void save();
  void unbook();

private:

  /** Utility */
  const double occupancy();
  const unsigned int clusterizedstrips();

  /** Storage */
  TFile* file_;
  TTree* tree_;
  
  /** Data containers */
  SimpleEventData* data_;
  double time_;
  unsigned int nchans_;
  unsigned int nunpackedchans_;
  
  /** Plots */
  SiStripPerformancePlots plots_;
};

#endif
