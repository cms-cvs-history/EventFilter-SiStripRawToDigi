#include "EventFilter/SiStripRawToDigi/test/stubs/macros/Plots.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

Plots::Plots() : th1fs_(50,static_cast<TH1F*>(0)), tprofiles_(50,static_cast<TProfile*>(0)), efficiencies_(50,static_cast<SimpleEfficiency*>(0)) {}

Plots::~Plots() {}
  
void Plots::book() {
  
  th1fs_[TIME] = new TH1F("Plots::time", "", 30,0.,0.15);
  tprofiles_[TIMEVSOCCUPANCY] = new TProfile("Plots::timevsoccupancy", "", 50,0.,1.);
  tprofiles_[TIMEVSDIGIS] = new TProfile("Plots::timevsdigis", "", 1000,50000.,150000.);
  tprofiles_[TIMEVSCLUSTERS] = new TProfile("Plots::timevsclusters", "", 100,0.,100000.);
  tprofiles_[TIMEVSCLUSTERSIZE] = new TProfile("Plots::timevsclustersize", "", 7,0.,7.);
  tprofiles_[TIMEVSFRAC] = new TProfile("Plots::timevsfrac", "", 11,0.,1.1);
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

  th1fs_[TIME]->SetTitle("Event unpacking time.");
  th1fs_[TIME]->GetXaxis()->SetTitle("Time [ s ]");
  th1fs_[TIME]->GetYaxis()->SetTitle("Number of events");
  tprofiles_[TIMEVSOCCUPANCY]->SetTitle("Event unpacking time vs Occupancy.");
  tprofiles_[TIMEVSOCCUPANCY]->GetXaxis()->SetTitle("Occupancy [ % ]");
  tprofiles_[TIMEVSOCCUPANCY]->GetYaxis()->SetTitle("Event unpacking time [ s ]");
  tprofiles_[TIMEVSDIGIS]->SetTitle("Event unpacking time vs Number of digis.");
  tprofiles_[TIMEVSDIGIS]->GetXaxis()->SetTitle("digis");
  tprofiles_[TIMEVSDIGIS]->GetYaxis()->SetTitle("Event unpacking time [ s ]");
  tprofiles_[TIMEVSCLUSTERS]->SetTitle("Event unpacking time vs Number of clusters.");
  tprofiles_[TIMEVSCLUSTERS]->GetXaxis()->SetTitle("clusters");
  tprofiles_[TIMEVSCLUSTERS]->GetYaxis()->SetTitle("Event unpacking time [ s ]");
  tprofiles_[TIMEVSCLUSTERSIZE]->SetTitle("Event unpacking time vs Mean cluster size.");
  tprofiles_[TIMEVSCLUSTERSIZE]->GetXaxis()->SetTitle("mean clusters size [ strips ]");
  tprofiles_[TIMEVSCLUSTERSIZE]->GetYaxis()->SetTitle("Event unpacking time [ s ]");
  tprofiles_[TIMEVSFRAC]->SetTitle("Event unpacking time vs Fraction of SST unpacked.");
  tprofiles_[TIMEVSFRAC]->GetXaxis()->SetTitle("Fraction of SST");
  tprofiles_[TIMEVSFRAC]->GetYaxis()->SetTitle("Event unpacking time [ s ]");
  efficiencies_[HLT]->get()->SetTitle("HLT efficiency");
  efficiencies_[HLT]->get()->GetXaxis()->SetTitle("");
  efficiencies_[HLT]->get()->GetYaxis()->SetTitle("HLT Efficiency");
  efficiencies_[PT1]->get()->SetTitle("Reconstruction efficiency");
  efficiencies_[PT1]->get()->GetXaxis()->SetTitle("Electron P_{T} [GeV/c]");
  efficiencies_[PT1]->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");
  efficiencies_[PT2]->get()->SetTitle("Reconstruction efficiency");
  efficiencies_[PT2]->get()->GetXaxis()->SetTitle("Electron P_{T} [GeV/c]");
  efficiencies_[PT2]->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");
  efficiencies_[ETA1]->get()->SetTitle("Reconstruction efficiency");
  efficiencies_[ETA1]->get()->GetXaxis()->SetTitle("Electron #eta");
  efficiencies_[ETA1]->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");
  efficiencies_[ETA2]->get()->SetTitle("Reconstruction efficiency");
  efficiencies_[ETA2]->get()->GetXaxis()->SetTitle("Electron #eta");
  efficiencies_[ETA2]->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");
  efficiencies_[ETA3]->get()->SetTitle("Reconstruction efficiency");
  efficiencies_[ETA3]->get()->GetXaxis()->SetTitle("Electron #eta");
  efficiencies_[ETA3]->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");
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
  efficiencies_[HLT]->get()->Write();
  efficiencies_[PT1]->get()->Write();
  efficiencies_[PT2]->get()->Write();
  efficiencies_[ETA1]->get()->Write();
  efficiencies_[ETA2]->get()->Write();
  efficiencies_[ETA3]->get()->Write();
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
