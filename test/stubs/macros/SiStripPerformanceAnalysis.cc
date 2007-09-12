#include "EventFilter/SiStripRawToDigi/test/stubs/macros/SiStripPerformanceAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/macros/ObjectFilter.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

using namespace std;

SiStripPerformanceAnalysis::SiStripPerformanceAnalysis(TFile* file,string treename) : file_(file), tree_(0), data_(0), time_(0), nchans_(0), nunpackedchans_(0), plots_()

{
  tree_ = dynamic_cast<TTree*>(file_->Get(treename.c_str()));
  tree_->SetBranchAddress("SimpleEventData", &data_);
  tree_->SetBranchAddress("time", &time_);
  tree_->SetBranchAddress("nchans", &nchans_);
  tree_->SetBranchAddress("nunpackedchans", &nunpackedchans_);
}

SiStripPerformanceAnalysis::~SiStripPerformanceAnalysis() {}

void SiStripPerformanceAnalysis::book() {
  plots_.book();
}

void SiStripPerformanceAnalysis::unbook() {
  plots_.unbook();
}

void SiStripPerformanceAnalysis::timing() {

  for (unsigned int ievent=0;ievent<tree_->GetEntries();ievent++) {
    
    tree_->GetEntry(ievent); 
    data_->order();

    const double diginum = static_cast<double>(data_->sistripdiginum());
    const double clusternum = static_cast<double>(data_->sistripclusternum());
    const double clusterized = static_cast<double>(clusterizedstrips());
    const double fraction = (100.*nunpackedchans_)/nchans_;
    const double csize = (clusternum) ? clusterized/clusternum : constants::invalid;
    const double occ = occupancy();

    plots_.get(SiStripPerformancePlots::TIME)->Fill(time_);
    plots_.get(SiStripPerformancePlots::FRAC)->Fill(fraction);
    plots_.get(SiStripPerformancePlots::OCCUPANCY)->Fill(occ);
    plots_.get(SiStripPerformancePlots::TIMEVSOCCUPANCY)->Fill(occ,time_);
    plots_.get(SiStripPerformancePlots::TIMEVSDIGIS)->Fill(diginum,time_);
    plots_.get(SiStripPerformancePlots::TIMEVSFRAC)->Fill(fraction,time_); 
    plots_.get(SiStripPerformancePlots::TIMEVSCLUSTERS)->Fill(clusternum,time_);
    plots_.get(SiStripPerformancePlots::CLUSTERSIZE)->Fill(csize);
    plots_.get(SiStripPerformancePlots::TIMEVSCLUSTERSIZE)->Fill(csize,time_);

  }
}

void SiStripPerformanceAnalysis::trigger(unsigned int bit) {

  for (unsigned int ievent=0;ievent<tree_->GetEntries();ievent++) {
   
    tree_->GetEntry(ievent); 
    data_->order();

    if (!objectfilter::trigger(data_->mc(),bit)) continue;
    if (data_->trigger().get(bit)) plots_.get(SiStripPerformancePlots::HLT)->select(1);
    else plots_.get(SiStripPerformancePlots::HLT)->select(1,false);
  }
}
    
void SiStripPerformanceAnalysis::electron(unsigned int bit) {

  for (unsigned int ievent=0;ievent<tree_->GetEntries();ievent++) {
    
    tree_->GetEntry(ievent); 
    data_->order();
    
    for (std::vector<SimpleGenParticle>::const_iterator ipart=data_->mc().begin();ipart!=data_->mc().end();ipart++) {
      
      if (objectfilter::trigger(data_->mc(),bit) && data_->electrons().size() && abs(ipart->pid()) == 11 && fabs(ipart->eta()) > constants::etaCut) {
	
	unsigned int index = objectfilter::electron(data_->electrons(),*ipart);
	double delta = SimpleSCluster::dR(data_->electrons()[index].scluster(),*ipart);
	bool matched = (delta < constants::dRelec) ? true : false;
	
	if (fabs(ipart->eta()) < constants::ecEcal) {
	  if (matched) plots_.get(SiStripPerformancePlots::PT1)->select(ipart->pt());
	  else plots_.get(SiStripPerformancePlots::PT1)->select(ipart->pt(),false);
	}
	
	if (fabs(ipart->eta()) > constants::ecEcal) {
	  if (matched) plots_.get(SiStripPerformancePlots::PT2)->select(ipart->pt());
	  else plots_.get(SiStripPerformancePlots::PT2)->select(ipart->pt(),false);
	}
	
	if (ipart->pt() < 40.) {
	  if (matched) plots_.get(SiStripPerformancePlots::ETA1)->select(ipart->eta());
	  else plots_.get(SiStripPerformancePlots::ETA1)->select(ipart->eta(),false);
	}
	
	if ((ipart->pt() > 40.) && (ipart->pt() < 100.)) {
	  if (matched) plots_.get(SiStripPerformancePlots::ETA2)->select(ipart->eta());
	  else plots_.get(SiStripPerformancePlots::ETA2)->select(ipart->eta(),false);
	}
	
	if (ipart->pt() > 100.) {
	  if (matched) plots_.get(SiStripPerformancePlots::ETA3)->select(ipart->eta());
	  else plots_.get(SiStripPerformancePlots::ETA3)->select(ipart->eta(),false);
	}
      }
    }
  }  
}

void SiStripPerformanceAnalysis::format() {
  plots_.format();
}

void SiStripPerformanceAnalysis::save() {
  std::string name = tree_->GetName();
  plots_.normalise();
  plots_.save(utility::addDir(file_,"Plots_"+name));
}

const double SiStripPerformanceAnalysis::occupancy() {
  return data_->sistripdiginum()/(nunpackedchans_*2.56);
}

const unsigned int SiStripPerformanceAnalysis::clusterizedstrips() {
 
  unsigned int clusterizedstrips=0;
  for (unsigned int i=0;i<data_->sistripclusters().size();i++) {
    clusterizedstrips+=data_->sistripclusters()[i].amplitudes();
  }
  return clusterizedstrips;
}


