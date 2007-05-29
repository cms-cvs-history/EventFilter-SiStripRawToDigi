#ifndef EventFilter_SiStripRawToDigi_PerformanceAnalysis_H
#define EventFilter_SiStripRawToDigi_PerformanceAnalysis_H

#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH1F.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/Efficiency.h"
#include <vector>
#include "EventFilter/SiStripRawToDigi/test/stubs/Overlay.h"

class PerformanceAnalysis {

public:
  
  PerformanceAnalysis(TFile*,std::string);
  ~PerformanceAnalysis();

  void book();
  void unbook();
  void analyze();

private:

  void save();
  void format();

  TFile* file_;
  std::string treename_;
  TTree* tree_;

  //Plots
  TH1F* time_;
  TProfile* timeVsocc_;
  TProfile* timeVsdigis_;
  TProfile* timeVsclusters_;
  TProfile* timeVsclustersize_;
  TProfile* timeVsfrac_;
  TProfile* timeVsevent_;
  TProfile2D* timeVsoccVsfrac_;
  TH1F* clusterwidth_;
  TH1F* firststrip_;
  TProfile* firststripVsclusterwidth_;
  Efficiency* effelectronVspt_;
  Efficiency* effelectronVseta_;

};

#endif
