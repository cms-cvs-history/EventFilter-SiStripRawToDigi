#include "EventFilter/SiStripRawToDigi/test/stubs/macros/Plots.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

Plots::Plots() : th1fs_(50,static_cast<TH1F*>(0)), tprofiles_(50,static_cast<TProfile*>(0)), efficiencies_(50,static_cast<SimpleEfficiency*>(0)) {}

Plots::~Plots() {}
  
void Plots::book() {
  
  th1fs_[TIME] = new TH1F("Plots::time", "", 30,0.,5.);
  tprofiles_[TIMEVSOCCUPANCY] = new TProfile("Plots::timevsoccupancy", "", 10,0.,2.);
  tprofiles_[TIMEVSDIGIS] = new TProfile("Plots::timevsdigis", "", 100,50000.,150000.);
  tprofiles_[TIMEVSCLUSTERS] = new TProfile("Plots::timevsclusters", "", 100,0.,100000.);
  tprofiles_[TIMEVSCLUSTERSIZE] = new TProfile("Plots::timevsclustersize", "", 7,0.,7.);
  tprofiles_[TIMEVSFRAC] = new TProfile("Plots::timevsfrac", "", 10,0.,100.);
  efficiencies_[HLT] = new SimpleEfficiency("Plots::hlt", "", 1,0.,1.);
  efficiencies_[PT1] = new SimpleEfficiency("Plots::pt1", "", 20,0.,100.);
  efficiencies_[PT2] = new SimpleEfficiency("Plots::pt2", "", 20,0.,100.);
  efficiencies_[ETA1] = new SimpleEfficiency("Plots::eta1", "", 30,-3.,3.);
  efficiencies_[ETA2] = new SimpleEfficiency("Plots::eta2", "", 30,-3.,3.);
  efficiencies_[ETA3] = new SimpleEfficiency("Plots::eta3", "", 30,-3.,3.);
}

void Plots::unbook() {

  for (unsigned int i=0;i<th1fs_.size();i++) {if (th1fs_[i]) delete th1fs_[i];}
  for (unsigned int i=0;i<tprofiles_.size();i++) {if (tprofiles_[i]) delete tprofiles_[i];}
  for (unsigned int i=0;i<efficiencies_.size();i++) {if (efficiencies_[i]) delete efficiencies_[i];}
}

void Plots::format() {

  th1fs_[TIME]->SetTitle("");
  th1fs_[TIME]->GetXaxis()->SetTitle("time [ s ]");
  th1fs_[TIME]->GetYaxis()->SetTitle("");
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

void Plots::normalise() {
  
  for (unsigned short i=0;i<th1fs_.size();i++) {if (th1fs_[i]) utility::normalise(th1fs_[i]);}
  for (unsigned short i=0;i<efficiencies_.size();i++) {if (efficiencies_[i]) efficiencies_[i]->calculate();}
  
}

void Plots::save(TDirectory* dir) {

  dir->cd();
  th1fs_[TIME]->Write();
  tprofiles_[TIMEVSOCCUPANCY]->Write();
  tprofiles_[TIMEVSDIGIS]->Write();
  tprofiles_[TIMEVSCLUSTERS]->Write();
  tprofiles_[TIMEVSCLUSTERSIZE]->Write();
  tprofiles_[TIMEVSFRAC]->Write();
  efficiencies_[HLT]->Write();
  efficiencies_[PT1]->Write();
  efficiencies_[PT2]->Write();
  efficiencies_[ETA1]->Write();
  efficiencies_[ETA2]->Write();
  efficiencies_[ETA3]->Write();
}

void Plots::read(TDirectory* dir) {

  th1fs_[TIME] = dynamic_cast<TH1F*>(dir->Get("Plots::time"));
  tprofiles_[TIMEVSOCCUPANCY] = dynamic_cast<TProfile*>(dir->Get("Plots::timevsoccupancy"));
  tprofiles_[TIMEVSDIGIS] = dynamic_cast<TProfile*>(dir->Get("Plots::timevsdigis"));
  tprofiles_[TIMEVSCLUSTERS] = dynamic_cast<TProfile*>(dir->Get("Plots::timevsclusters"));
  tprofiles_[TIMEVSCLUSTERSIZE] = dynamic_cast<TProfile*>(dir->Get("Plots::timevsclustersize"));
  tprofiles_[TIMEVSFRAC] = dynamic_cast<TProfile*>(dir->Get("Plots::timevsfrac"));
  efficiencies_[HLT] = dynamic_cast<SimpleEfficiency*>(dir->Get("Plots::hlt"));
  efficiencies_[PT1] = dynamic_cast<SimpleEfficiency*>(dir->Get("Plots::pt1"));
  efficiencies_[PT2] = dynamic_cast<SimpleEfficiency*>(dir->Get("Plots::pt2"));
  efficiencies_[ETA1] = dynamic_cast<SimpleEfficiency*>(dir->Get("Plots::eta1"));
  efficiencies_[ETA2] = dynamic_cast<SimpleEfficiency*>(dir->Get("Plots::eta2"));
  efficiencies_[ETA3] = dynamic_cast<SimpleEfficiency*>(dir->Get("Plots::eta3"));
}

TH1F* Plots::get(TH1FType type) {return th1fs_[type];}

TProfile* Plots::get(TProfileType type) {return tprofiles_[type];}

SimpleEfficiency* Plots::get(EfficiencyType type) {return efficiencies_[type];}
