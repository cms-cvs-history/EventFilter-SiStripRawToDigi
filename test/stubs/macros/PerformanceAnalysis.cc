#include "EventFilter/SiStripRawToDigi/test/stubs/macros/PerformanceAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/macros/ObjectFilter.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

using namespace std;

PerformanceAnalysis::PerformanceAnalysis(TFile* file,string treename) :

  file_(file),
  tree_(0),
  data_(0),
  time_(0),
  nchans_(0),
  nunpackedchans_(0),
  plots_()

{
  tree_ = dynamic_cast<TTree*>(file_->Get(treename.c_str()));
  tree_->SetBranchAddress("SimpleEventData", &data_);
  tree_->SetBranchAddress("time", &time_);
  tree_->SetBranchAddress("nchans", &nchans_);
  tree_->SetBranchAddress("nunpackedchans", &nunpackedchans_);
}

PerformanceAnalysis::~PerformanceAnalysis() {;}

void PerformanceAnalysis::book() {
  plots_.book();
}

void PerformanceAnalysis::unbook() {
  plots_.unbook();
}

void PerformanceAnalysis::analyze() {

  if (!tree_) {return;}
  
  unsigned int nevents = tree_->GetEntries();
  for (unsigned int ievent=0; ievent<nevents; ievent++) {
    
    //Collect event data
    tree_->GetEntry(ievent); 
    
    //Record HLT and recon efficiencies    
    if (objectfilter::doubleelectron(data_->mc())) {
      electron();
      if (data_->trigger().get(4)) plots_.get(Plots::HLT)->select(1);
      else plots_.get(Plots::HLT)->select(1,false);
    }

    //Record timing
    plots_.get(Plots::TIME)->Fill(time_);
    plots_.get(Plots::FRAC)->Fill(100.*(double)nunpackedchans_/(double)nchans_);
    plots_.get(Plots::TIMEVSOCCUPANCY)->Fill(occupancy(),time_);
    plots_.get(Plots::TIMEVSDIGIS)->Fill(data_->sistripdiginum(),time_);
    plots_.get(Plots::TIMEVSCLUSTERS)->Fill(data_->sistripclusternum(),time_);
    plots_.get(Plots::TIMEVSCLUSTERSIZE)->Fill((double)data_->sistripdiginum()/(double)data_->sistripclusternum(),time_);
    plots_.get(Plots::TIMEVSFRAC)->Fill(100.*(double)nunpackedchans_/(double)nchans_,time_); 
 }
}

void PerformanceAnalysis::electron() {
  
  vector<SimpleGenParticle>::const_iterator ipart = data_->mc().begin();
  for (; ipart != data_->mc().end(); ipart++) {
   
    if ((abs(ipart->pid()) == 11) && (fabs(ipart->eta()) < constants::etaCut)) {

      bool matched = (data_->electrons().size() && SimpleSCluster::dR(data_->electrons()[objectfilter::electron(data_->electrons(),*ipart)].scluster(), *ipart) < constants::dRelec) ? true : false;

      if (fabs(ipart->eta()) < constants::ecEcal) {
	if (matched) plots_.get(Plots::PT1)->select((Double_t)ipart->pt());
	else plots_.get(Plots::PT1)->select((Double_t)ipart->pt(),false);
      }
      
      if (fabs(ipart->eta()) > constants::ecEcal) {
	if (matched) plots_.get(Plots::PT2)->select((Double_t)ipart->pt());
	else plots_.get(Plots::PT2)->select((Double_t)ipart->pt(),false);
      }
      
      if (ipart->pt() < 40.) {
	if (matched) plots_.get(Plots::ETA1)->select((Double_t)ipart->eta());
	else plots_.get(Plots::ETA1)->select((Double_t)ipart->eta(),false);
      }
      
      if ((ipart->pt() > 40.) && (ipart->pt() < 100.)) {
	if (matched) plots_.get(Plots::ETA2)->select((Double_t)ipart->eta());
	else plots_.get(Plots::ETA2)->select((Double_t)ipart->eta(),false);
      }
      
      if (ipart->pt() > 100.) {
	if (matched) plots_.get(Plots::ETA3)->select((Double_t)ipart->eta());
	else plots_.get(Plots::ETA3)->select((Double_t)ipart->eta(),false);
      }
    }
  }
}

void PerformanceAnalysis::tau() {
 
 vector<SimpleGenParticle>::const_iterator ipart = data_->mc().begin();
  for (; ipart != data_->mc().end(); ipart++) {

  if ((abs(ipart->pid()) == 15) && (fabs(ipart->eta()) < constants::etaCut)) {
    
    bool matched = (data_->jets().size() && SimpleHCluster::dR(data_->jets()[objectfilter::jet(data_->jets(),*ipart)].hcluster(), *ipart) < constants::dRtau) ? true : false;
    
    if (ipart->eta() < constants::ecEcal) {
      if (matched) plots_.get(Plots::PT1)->select((Double_t)ipart->pt());
      else plots_.get(Plots::PT1)->select((Double_t)ipart->pt(),false);
    }
    
    if (ipart->eta() > constants::ecEcal) {
      if (matched) plots_.get(Plots::PT2)->select((Double_t)ipart->pt());
      else plots_.get(Plots::PT2)->select((Double_t)ipart->pt(),false);
    }
    
    if (ipart->pt() < 40.) {
      if (matched) plots_.get(Plots::ETA1)->select((Double_t)ipart->eta());
      else plots_.get(Plots::ETA1)->select((Double_t)ipart->eta(),false);
    }
    
    if ((ipart->pt() > 40.) && (ipart->pt() < 100.)) {
      if (matched) plots_.get(Plots::ETA2)->select((Double_t)ipart->eta());
      else plots_.get(Plots::ETA2)->select((Double_t)ipart->eta(),false);
    }
    
    if (ipart->pt() > 100.) {
      if (matched) plots_.get(Plots::ETA3)->select((Double_t)ipart->eta());
      else plots_.get(Plots::ETA3)->select((Double_t)ipart->eta(),false);
    }
  }
  }
}

const double PerformanceAnalysis::occupancy() {
  return (double)(data_->sistripdiginum())/(double)(nunpackedchans_*2.56);
}

void PerformanceAnalysis::format() {
  plots_.format();
}

void PerformanceAnalysis::save() {
  std::string name = tree_->GetName();
  plots_.normalise();
  plots_.save(utility::addDir(file_,"Plots_"+name));
}

