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

  void electron(const std::vector<SimpleParticle>&, 
		const std::vector<SimpleElectron>&);

  void tau(const std::vector<SimpleParticle>&, 
	   const std::vector<SimpleJet>&);

  void timer(const Double_t, 
	     const Int_t, 
	     const Int_t, 
	     const Double_t, 
	     const Double_t);

  static const bool electron1(std::vector<SimpleParticle>&);

  static const bool electron2(std::vector<SimpleParticle>&);
  
  static const bool electron_match(const SimpleParticle&, 
			     const std::vector<SimpleElectron>&);
  
  static const bool tau_match(const SimpleParticle&, 
			      const std::vector<SimpleJet>&);

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

  //Efficiency
  Efficiency* eff_hlt_;
  Efficiency* eff_vspt_1_;
  Efficiency* eff_vspt_2_;
  Efficiency* eff_vseta_1_;
  Efficiency* eff_vseta_2_;
  Efficiency* eff_vseta_3_;

};

#endif
