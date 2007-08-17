#ifndef EventFilter_SiStripRawToDigi_PerformanceAnalysis_H
#define EventFilter_SiStripRawToDigi_PerformanceAnalysis_H

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

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

  void electron(const std::vector<SimpleGenParticle>&, const std::vector<SimpleElectron>&);

  void tau(const std::vector<SimpleGenParticle>&, const std::vector<SimpleJet>&);

  static const bool electron1(std::vector<SimpleGenParticle>&);

  static const bool electron2(std::vector<SimpleGenParticle>&);
  
  static const bool electron_match(const SimpleGenParticle&, const std::vector<SimpleElectron>&);
  
  static const bool tau_match(const SimpleGenParticle&, const std::vector<SimpleJet>&);

  static const double occupancy(SimpleEventData&, unsigned int);

  TFile* file_;
  TTree* tree_;

  //Timing
  TH1F* time_;
  TProfile* timeVsocc_;
  TProfile* timeVsdigis_;
  TProfile* timeVsclusters_;
  TProfile* timeVsclustersize_;
  TProfile* timeVsfrac_;

  //Efficiency
  SimpleEfficiency* eff_hlt_;
  SimpleEfficiency* eff_vspt_1_;
  SimpleEfficiency* eff_vspt_2_;
  SimpleEfficiency* eff_vseta_1_;
  SimpleEfficiency* eff_vseta_2_;
  SimpleEfficiency* eff_vseta_3_;
};

#endif
