#ifndef EventFilter_SiStripRawToDigi_PerformanceAnalysis_H
#define EventFilter_SiStripRawToDigi_PerformanceAnalysis_H

//stl
#include <string>
#include <vector>

//root
#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH1F.h"

//EventFilter/SiStripRawToDigi
#include "EventFilter/SiStripRawToDigi/test/stubs/Efficiency.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/Overlay.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/EventData.h"

class PerformanceAnalysis {

public:
  
  PerformanceAnalysis(TFile*,std::string);
  ~PerformanceAnalysis();

  enum Trigger {ELECTRON_SINGLE,ELECTRON_DOUBLE,TAU_SINGLE,TAU_DOUBLE,NONE};

  void book();
  void format();
  void analyze(Trigger=NONE);
  void save();
  void unbook();

private:

  bool electron_single(std::vector<SimpleParticle>&);
  bool electron_double(std::vector<SimpleParticle>&);
  bool tau_single(std::vector<SimpleParticle>&);
  bool tau_double(std::vector<SimpleParticle>&);

  void electron(std::vector<SimpleParticle>&, 
		std::vector<SimpleElectron>&);
  void tau(std::vector<SimpleParticle>&, 
	   std::vector<SimpleJet>&);

  TFile* file_;
  std::string treename_;
  TTree* tree_;

  //Timing
  TH1F* time_;
  TProfile* timeVsocc_;
  TProfile* timeVsdigis_;
  TProfile* timeVsclusters_;
  TProfile* timeVsclustersize_;
  TProfile* timeVsfrac_;
  TProfile* timeVsevent_;
  TProfile2D* timeVsoccVsfrac_;

  //Clusters
  TH1F* clusterwidth_;
  TH1F* firststrip_;
  TProfile* firststripVsclusterwidth_;

  //Efficiency
  Efficiency* eff_hlt_;
  Efficiency* eff_total_;
  Efficiency* eff_vspt_1_;
  Efficiency* eff_vspt_2_;
  Efficiency* eff_vseta_1_;
  Efficiency* eff_vseta_2_;
  Efficiency* eff_vseta_3_;

};

#endif
