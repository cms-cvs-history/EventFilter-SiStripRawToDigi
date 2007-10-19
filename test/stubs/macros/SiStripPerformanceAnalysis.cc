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
    const double csize = (clusternum) ? clusterized/clusternum : 0.;
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

    if (!objectfilter::trigger(data_->mc(),data_->mcjets(),bit)) continue;
    if (data_->trigger().get(bit)) plots_.get(SiStripPerformancePlots::HLT)->select(1);
    else plots_.get(SiStripPerformancePlots::HLT)->select(1,false);
  }
}
    
void SiStripPerformanceAnalysis::electron(unsigned int bit) {
  
  for (unsigned int ievent=0;ievent<tree_->GetEntries();ievent++) {
    
    tree_->GetEntry(ievent); 
    data_->order();
    
    std::vector<SimpleGenParticle>::const_iterator ipart=data_->mc().begin();
    for (;ipart!=data_->mc().end();ipart++) {
      if (!data_->trigger().get(bit)) continue;
      if (abs(ipart->pid()) != 11 || fabs(ipart->eta()) > constants::tracker) continue;
      reconstruction(*ipart,matched(data_->electrons(),*ipart)); 
    }
  }  
}

void SiStripPerformanceAnalysis::muon(unsigned int bit) {
  
  for (unsigned int ievent=0;ievent<tree_->GetEntries();ievent++) {
    
    tree_->GetEntry(ievent); 
    data_->order();
    
    std::vector<SimpleGenParticle>::const_iterator ipart=data_->mc().begin();
    for (;ipart!=data_->mc().end();ipart++) {    
      if (!data_->trigger().get(bit)) continue; 
      if (abs(ipart->pid()) != 13 || fabs(ipart->eta()) > constants::tracker) continue;
      reconstruction(*ipart,matched(data_->muons(),*ipart));
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

const bool SiStripPerformanceAnalysis::matched(const std::vector<SimpleElectron>& electrons ,const SimpleGenParticle& particle) {

  if (!electrons.empty()) {
    unsigned int index = objectfilter::electron(electrons,particle);
    double delta = SimpleSCluster::dR(electrons[index].scluster(),particle);
    return (delta < constants::dRelectron) ? true : false;
  }
  return false;
}

const bool SiStripPerformanceAnalysis::matched(const std::vector<SimpleMuon>& muons,const SimpleGenParticle& particle) {

  if (!muons.empty()) {
    unsigned int index = objectfilter::muon(muons,particle);
    double delta = SimpleTrack::dR(muons[index].track(),particle);
    return (delta < constants::dRmuon) ? true : false;
  }
  return false;
}

void SiStripPerformanceAnalysis::reconstruction(const SimpleGenParticle& particle,const bool matched) {
  
  if (fabs(particle.eta()) < constants::endcap) {
    if (matched) plots_.get(SiStripPerformancePlots::PT1)->select(particle.pt());
    else plots_.get(SiStripPerformancePlots::PT1)->select(particle.pt(),false);
  }
  
  if (fabs(particle.eta()) > constants::endcap) {
    if (matched) plots_.get(SiStripPerformancePlots::PT2)->select(particle.pt());
    else plots_.get(SiStripPerformancePlots::PT2)->select(particle.pt(),false);
  }
  
  if (particle.pt() < 40.) {
    if (matched) plots_.get(SiStripPerformancePlots::ETA1)->select(particle.eta());
    else plots_.get(SiStripPerformancePlots::ETA1)->select(particle.eta(),false);
  }
  
  if ((particle.pt() > 40.) && (particle.pt() < 100.)) {
    if (matched) plots_.get(SiStripPerformancePlots::ETA2)->select(particle.eta());
    else plots_.get(SiStripPerformancePlots::ETA2)->select(particle.eta(),false);
  }
  
  if (particle.pt() > 100.) {
    if (matched) plots_.get(SiStripPerformancePlots::ETA3)->select(particle.eta());
    else plots_.get(SiStripPerformancePlots::ETA3)->select(particle.eta(),false);
  }
}
