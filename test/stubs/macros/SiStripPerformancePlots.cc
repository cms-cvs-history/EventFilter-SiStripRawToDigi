#include "EventFilter/SiStripRawToDigi/test/stubs/macros/SiStripPerformancePlots.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

SiStripPerformancePlots::SiStripPerformancePlots() : th1fs_(50,static_cast<TH1F*>(0)), tprofiles_(50,static_cast<TProfile*>(0)), efficiencies_(50,static_cast<SimpleEfficiency*>(0)) {}

SiStripPerformancePlots::~SiStripPerformancePlots() {}
  
void SiStripPerformancePlots::book() {
  
  th1fs_[TIME] = new TH1F("time", "", 20,0.,.4);
  th1fs_[FRAC] = new TH1F("frac", "", 50,0.,100.);
  th1fs_[CLUSTERSIZE] = new TH1F("clustersize", "", 50,0.,50.);
  th1fs_[OCCUPANCY] = new TH1F("occupancy", "", 50,0.,5.);
  tprofiles_[TIMEVSOCCUPANCY] = new TProfile("timevsoccupancy", "", 20,0.,2.);
  tprofiles_[TIMEVSDIGIS] = new TProfile("timevsdigis", "", 100,50000.,150000.);
  tprofiles_[TIMEVSCLUSTERS] = new TProfile("timevsclusters", "", 100,0.,100000.);
  tprofiles_[TIMEVSCLUSTERSIZE] = new TProfile("timevsclustersize", "", 20,0.,20.);
  tprofiles_[TIMEVSFRAC] = new TProfile("timevsfrac", "", 10,0.,100.);
  efficiencies_[HLT] = new SimpleEfficiency("hlt", "", 1,0.,1.);
  efficiencies_[PT1] = new SimpleEfficiency("pt1", "", 20,0.,100.);
  efficiencies_[PT2] = new SimpleEfficiency("pt2", "", 20,0.,100.);
  efficiencies_[ETA1] = new SimpleEfficiency("eta1", "", 30,-3.,3.);
  efficiencies_[ETA2] = new SimpleEfficiency("eta2", "", 30,-3.,3.);
  efficiencies_[ETA3] = new SimpleEfficiency("eta3", "", 30,-3.,3.);
}

void SiStripPerformancePlots::unbook() {

  for (unsigned int i=0;i<th1fs_.size();i++) {if (th1fs_[i]) delete th1fs_[i];}
  for (unsigned int i=0;i<tprofiles_.size();i++) {if (tprofiles_[i]) delete tprofiles_[i];}
  for (unsigned int i=0;i<efficiencies_.size();i++) {if (efficiencies_[i]) delete efficiencies_[i];}
}

void SiStripPerformancePlots::format() {

  th1fs_[TIME]->SetTitle("");
  th1fs_[TIME]->GetXaxis()->SetTitle("time [ s ]");
  th1fs_[TIME]->GetYaxis()->SetTitle("");
  th1fs_[FRAC]->SetTitle("");
  th1fs_[FRAC]->GetXaxis()->SetTitle("fraction of SST [ % ]");
  th1fs_[FRAC]->GetYaxis()->SetTitle("");
  th1fs_[CLUSTERSIZE]->SetTitle("");
  th1fs_[CLUSTERSIZE]->GetXaxis()->SetTitle("mean cluster size [ strips ]");
  th1fs_[CLUSTERSIZE]->GetYaxis()->SetTitle("");
  th1fs_[OCCUPANCY]->SetTitle("");
  th1fs_[OCCUPANCY]->GetXaxis()->SetTitle("occupancy [ % ]");
  th1fs_[OCCUPANCY]->GetYaxis()->SetTitle("");
  tprofiles_[TIMEVSOCCUPANCY]->SetTitle("");
  tprofiles_[TIMEVSOCCUPANCY]->GetXaxis()->SetTitle("occupancy [ % ]");
  tprofiles_[TIMEVSOCCUPANCY]->GetYaxis()->SetTitle("time [ s ]");
  tprofiles_[TIMEVSDIGIS]->SetTitle("");
  tprofiles_[TIMEVSDIGIS]->GetXaxis()->SetTitle("digis");
  tprofiles_[TIMEVSDIGIS]->GetYaxis()->SetTitle("time [ s ]");
  tprofiles_[TIMEVSCLUSTERS]->SetTitle("");
  tprofiles_[TIMEVSCLUSTERS]->GetXaxis()->SetTitle("clusters");
  tprofiles_[TIMEVSCLUSTERS]->GetYaxis()->SetTitle("time [ s ]");
  tprofiles_[TIMEVSCLUSTERSIZE]->SetTitle("");
  tprofiles_[TIMEVSCLUSTERSIZE]->GetXaxis()->SetTitle("mean clusters size [ strips ]");
  tprofiles_[TIMEVSCLUSTERSIZE]->GetYaxis()->SetTitle("time [ s ]");
  tprofiles_[TIMEVSFRAC]->SetTitle("");
  tprofiles_[TIMEVSFRAC]->GetXaxis()->SetTitle("SST unpacked [ % ]");
  tprofiles_[TIMEVSFRAC]->GetYaxis()->SetTitle("time [ s ]");
  efficiencies_[HLT]->get()->SetTitle("");
  efficiencies_[HLT]->get()->GetXaxis()->SetTitle("");
  efficiencies_[HLT]->get()->GetYaxis()->SetTitle("efficiency");
  efficiencies_[PT1]->get()->SetTitle("");
  efficiencies_[PT1]->get()->GetXaxis()->SetTitle("electron P_{T} [ GeV/c ]");
  efficiencies_[PT1]->get()->GetYaxis()->SetTitle("efficiency");
  efficiencies_[PT2]->get()->SetTitle("");
  efficiencies_[PT2]->get()->GetXaxis()->SetTitle("electron P_{T} [ GeV/c ]");
  efficiencies_[PT2]->get()->GetYaxis()->SetTitle("efficiency");
  efficiencies_[ETA1]->get()->SetTitle("");
  efficiencies_[ETA1]->get()->GetXaxis()->SetTitle("electron #eta");
  efficiencies_[ETA1]->get()->GetYaxis()->SetTitle("efficiency");
  efficiencies_[ETA2]->get()->SetTitle("");
  efficiencies_[ETA2]->get()->GetXaxis()->SetTitle("electron #eta");
  efficiencies_[ETA2]->get()->GetYaxis()->SetTitle("efficiency");
  efficiencies_[ETA3]->get()->SetTitle("");
  efficiencies_[ETA3]->get()->GetXaxis()->SetTitle("electron #eta");
  efficiencies_[ETA3]->get()->GetYaxis()->SetTitle("efficiency");
}

void SiStripPerformancePlots::normalise() {
  
  for (unsigned short i=0;i<th1fs_.size();i++) {if (th1fs_[i]) utility::normalise(th1fs_[i]);}
  for (unsigned short i=0;i<efficiencies_.size();i++) {if (efficiencies_[i]) efficiencies_[i]->calculate();}
  
}

void SiStripPerformancePlots::save(TDirectory* dir) {

  dir->cd();
  for (unsigned int i=0;i<th1fs_.size();i++) {if (th1fs_[i]) th1fs_[i]->Write();}
  for (unsigned int i=0;i<tprofiles_.size();i++) {if (tprofiles_[i]) tprofiles_[i]->Write();}
  for (unsigned int i=0;i<efficiencies_.size();i++) {if (efficiencies_[i]) efficiencies_[i]->Write();}
}

void SiStripPerformancePlots::read(TDirectory* dir) {

  th1fs_[TIME] = dynamic_cast<TH1F*>(dir->Get("time"));
  th1fs_[FRAC] = dynamic_cast<TH1F*>(dir->Get("frac"));
  th1fs_[CLUSTERSIZE] = dynamic_cast<TH1F*>(dir->Get("clustersize"));
  th1fs_[OCCUPANCY] = dynamic_cast<TH1F*>(dir->Get("occupancy"));
  tprofiles_[TIMEVSOCCUPANCY] = dynamic_cast<TProfile*>(dir->Get("timevsoccupancy"));
  tprofiles_[TIMEVSDIGIS] = dynamic_cast<TProfile*>(dir->Get("timevsdigis"));
  tprofiles_[TIMEVSCLUSTERS] = dynamic_cast<TProfile*>(dir->Get("timevsclusters"));
  tprofiles_[TIMEVSCLUSTERSIZE] = dynamic_cast<TProfile*>(dir->Get("timevsclustersize"));
  tprofiles_[TIMEVSFRAC] = dynamic_cast<TProfile*>(dir->Get("timevsfrac"));
  efficiencies_[HLT] = dynamic_cast<SimpleEfficiency*>(dir->Get("hlt"));
  efficiencies_[PT1] = dynamic_cast<SimpleEfficiency*>(dir->Get("pt1"));
  efficiencies_[PT2] = dynamic_cast<SimpleEfficiency*>(dir->Get("pt2"));
  efficiencies_[ETA1] = dynamic_cast<SimpleEfficiency*>(dir->Get("eta1"));
  efficiencies_[ETA2] = dynamic_cast<SimpleEfficiency*>(dir->Get("eta2"));
  efficiencies_[ETA3] = dynamic_cast<SimpleEfficiency*>(dir->Get("eta3"));
}

TH1F* SiStripPerformancePlots::get(TH1FType type) {return th1fs_[type];}

TProfile* SiStripPerformancePlots::get(TProfileType type) {return tprofiles_[type];}

SimpleEfficiency* SiStripPerformancePlots::get(EfficiencyType type) {return efficiencies_[type];}
