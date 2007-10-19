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
  void muon(unsigned int);
  void save();
  void unbook();

private:

  /** Utility */
  const double occupancy();
  const unsigned int clusterizedstrips();
  const bool matched(const std::vector<SimpleElectron>&,const SimpleGenParticle&);
  const bool matched(const std::vector<SimpleMuon>&,const SimpleGenParticle&);

  /** Fill */
  void reconstruction(const SimpleGenParticle&,const bool);

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
