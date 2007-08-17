#include "EventFilter/SiStripRawToDigi/test/stubs/macros/PerformanceAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

using namespace std;

PerformanceAnalysis::PerformanceAnalysis(TFile* file,string treename) :

  file_(file),
  tree_(0),
  time_(0),
  timeVsocc_(0),
  timeVsdigis_(0),
  timeVsclusters_(0),
  timeVsclustersize_(0),
  timeVsfrac_(0),
  eff_hlt_(0),
  eff_vspt_1_(0),
  eff_vspt_2_(0),
  eff_vseta_1_(0),
  eff_vseta_2_(0),
  eff_vseta_3_(0)

{tree_ = dynamic_cast<TTree*>(file_->Get(treename.c_str()));}

PerformanceAnalysis::~PerformanceAnalysis() {;}

void PerformanceAnalysis::book() {

  if (!tree_) {return;}
  time_ = new TH1F(tree_->GetName(), tree_->GetName(), 30,0.,0.15);
  timeVsocc_ = new TProfile(tree_->GetName(), tree_->GetName(), 50,0.,1.);
  timeVsdigis_ = new TProfile(tree_->GetName(), tree_->GetName(), 1000,50000.,150000.);
  timeVsclusters_ = new TProfile(tree_->GetName(), tree_->GetName(), 100,0.,100000.);
  timeVsclustersize_ = new TProfile(tree_->GetName(), tree_->GetName(), 7,0.,7.);
  timeVsfrac_ = new TProfile(tree_->GetName(), tree_->GetName(), 11,0.,1.1);
  eff_hlt_ = new SimpleEfficiency(tree_->GetName(), tree_->GetName(), 1,0.,1.);
  eff_vspt_1_ = new SimpleEfficiency(tree_->GetName(), tree_->GetName(), 20,0.,100.);
  eff_vspt_2_ = new SimpleEfficiency(tree_->GetName(), tree_->GetName(), 20,0.,100.);
  eff_vseta_1_ = new SimpleEfficiency(tree_->GetName(), tree_->GetName(), 30,-3.,3.);
  eff_vseta_2_ = new SimpleEfficiency(tree_->GetName(), tree_->GetName(), 30,-3.,3.);
  eff_vseta_3_ = new SimpleEfficiency(tree_->GetName(), tree_->GetName(), 30,-3.,3.);
}

void PerformanceAnalysis::unbook() {

  if (time_) delete time_;
  if (timeVsocc_) delete timeVsocc_;
  if (timeVsdigis_) delete timeVsdigis_;
  if (timeVsclusters_) delete timeVsclusters_;
  if (timeVsclustersize_) delete timeVsclustersize_;
  if (timeVsfrac_) delete timeVsfrac_;
  if (eff_hlt_) delete eff_hlt_;
  if (eff_vspt_1_) delete eff_vspt_1_;
  if (eff_vspt_2_) delete eff_vspt_2_;
  if (eff_vseta_1_) delete eff_vseta_1_;
  if (eff_vseta_2_) delete eff_vseta_2_;
  if (eff_vseta_3_) delete eff_vseta_3_;
}

void PerformanceAnalysis::analyze(Trigger trigger) {

 if (!tree_) {return;}
 
 SimpleEventData* data = 0;
 double time;
 unsigned int nchans;
 unsigned int nunpackedchans;
 
 tree_->SetBranchAddress("SimpleEventData", &data);
 tree_->SetBranchAddress("time", &time);
 tree_->SetBranchAddress("nchans", &nchans);
 tree_->SetBranchAddress("nunpackedchans", &nunpackedchans);
 
 unsigned int nevents = tree_->GetEntries();
  for (unsigned int ievent=0; ievent<nevents; ievent++) {
  
    //Collect event data
    tree_->GetEntry(ievent); 
  
    //Record HLT and recon efficiencies
    if (trigger==ELECTRON_SINGLE && electron1(data->mc())) {
      electron(data->mc(),data->electrons());
      if (data->trigger().get(18)) eff_hlt_->select(1);
      else eff_hlt_->select(1,false);
    }
   
    if (trigger==ELECTRON_DOUBLE && electron2(data->mc())) {
      electron(data->mc(),data->electrons());
      if (data->trigger().get(20)) eff_hlt_->select(1);
      else eff_hlt_->select(1,false);
    }
 
    //Record timing
    double occ = occupancy(*data,nunpackedchans);
    unsigned int ndigis = data->sistripdiginum();
    unsigned int nclusters = data->sistripclusternum();
    
    time_->Fill(time);
    timeVsocc_->Fill(occ,time);
    timeVsdigis_->Fill(ndigis,time);
    timeVsclusters_->Fill(nclusters,time);
    timeVsclustersize_->Fill((double)ndigis/(double)nclusters,time);
    timeVsfrac_->Fill((double)nunpackedchans/(double)nchans,time); 
 }
}

void PerformanceAnalysis::electron(const std::vector<SimpleGenParticle>& mc, 
				   const std::vector<SimpleElectron>& electrons) {
 
 vector<SimpleGenParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {

  if ((abs(ipart->pid()) == 11) &&
      (fabs(ipart->eta()) < constants::etaCut)) {
    
    bool matched = electron_match(*ipart,electrons);
    
    if (ipart->eta() < constants::ecEcal) {
      if (matched) eff_vspt_1_->select((Double_t)ipart->pt());
      else eff_vspt_1_->select((Double_t)ipart->pt(),false);
    }
    
    if (ipart->eta() > constants::ecEcal) {
      if (matched) eff_vspt_2_->select((Double_t)ipart->pt());
      else eff_vspt_2_->select((Double_t)ipart->pt(),false);
    }
    
    if (ipart->pt() < 40.) {
      if (matched) eff_vseta_1_->select((Double_t)ipart->eta());
      else eff_vseta_1_->select((Double_t)ipart->eta(),false);
    }
    
    if ((ipart->pt() > 40.) && (ipart->pt() < 100.)) {
      if (matched) eff_vseta_2_->select((Double_t)ipart->eta());
      else eff_vseta_2_->select((Double_t)ipart->eta(),false);
    }
    
    if (ipart->pt() > 100.) {
      if (matched) eff_vseta_3_->select((Double_t)ipart->eta());
      else eff_vseta_3_->select((Double_t)ipart->eta(),false);
    }
  }
  }
}


void PerformanceAnalysis::tau(const std::vector<SimpleGenParticle>& mc, 
			      const std::vector<SimpleJet>& taus) {
 
 vector<SimpleGenParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {

  if ((abs(ipart->pid()) == 15) &&
      (fabs(ipart->eta()) < constants::etaCut)) {
    
    bool matched = tau_match(*ipart,taus);
    
    if (ipart->eta() < constants::ecEcal) {
      if (matched) eff_vspt_1_->select((Double_t)ipart->pt());
      else eff_vspt_1_->select((Double_t)ipart->pt(),false);
    }
    
    if (ipart->eta() > constants::ecEcal) {
      if (matched) eff_vspt_2_->select((Double_t)ipart->pt());
      else eff_vspt_2_->select((Double_t)ipart->pt(),false);
    }
    
    if (ipart->pt() < 40.) {
      if (matched) eff_vseta_1_->select((Double_t)ipart->eta());
      else eff_vseta_1_->select((Double_t)ipart->eta(),false);
    }
    
    if ((ipart->pt() > 40.) && (ipart->pt() < 100.)) {
      if (matched) eff_vseta_2_->select((Double_t)ipart->eta());
      else eff_vseta_2_->select((Double_t)ipart->eta(),false);
    }
    
    if (ipart->pt() > 100.) {
      if (matched) eff_vseta_3_->select((Double_t)ipart->eta());
      else eff_vseta_3_->select((Double_t)ipart->eta(),false);
    }
  }
  }
}

 const bool PerformanceAnalysis::electron1(std::vector<SimpleGenParticle>& mc) {
    
    std::vector<SimpleGenParticle>::const_iterator ipart = mc.begin();
    for (; ipart != mc.end(); ipart++) {
      if ((abs(ipart->pid()) == 11) && 
	  (ipart->pt() > 26.) && 
	  (fabs(ipart->eta()) < constants::etaCut)) 
	return true;
    }
    return false;
  }

const bool PerformanceAnalysis::electron2(std::vector<SimpleGenParticle>& mc) {

  unsigned short count = 0;
  std::vector<SimpleGenParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {
    if ((abs(ipart->pid()) == 11) && (ipart->pt() > 12.) && (fabs(ipart->eta()) < constants::etaCut)) count++;
  }
  if (count >= 2) return true;
  return false;
}

const bool PerformanceAnalysis::electron_match(const SimpleGenParticle& mc, const std::vector<SimpleElectron>& electrons) {
  
  vector<SimpleElectron>::const_iterator ielectron = electrons.begin();
  for (;ielectron!=electrons.end();ielectron++) {
    if (SimpleSCluster::dR(ielectron->scluster(),mc) < constants::dRelec)
      return true;
  }
  return false;
}

const bool PerformanceAnalysis::tau_match(const SimpleGenParticle& mc, const std::vector<SimpleJet>& jets) {

 vector<SimpleJet>::const_iterator ijet = jets.begin();
  for (;ijet!=jets.end();ijet++) {
    if (SimpleHCluster::dR(ijet->hcluster(),mc) < constants::dRtau)
      return true;
  }
  return false;
}

const double PerformanceAnalysis::occupancy(SimpleEventData& data, unsigned int nchans) {
  
  unsigned int ndigis = 0;
  std::cout << "here" << std::endl;
  for (unsigned int i=0;i<(data.sistripclusternum());i++) {ndigis+=(data.sistripclusters()[i].amplitudes());}
  std::cout << ndigis << std::endl;
  return (ndigis) ? ndigis/(nchans*2.56) : data.sistripdiginum()/(nchans*2.56);
}

void PerformanceAnalysis::format() {
 
  time_->SetTitle("Event unpacking time.");
  time_->GetXaxis()->SetTitle("Time [ s ]");
  time_->GetYaxis()->SetTitle("Number of events");

  timeVsocc_->SetTitle("Event unpacking time vs Occupancy.");
  timeVsocc_->GetXaxis()->SetTitle("Occupancy [ % ]");
  timeVsocc_->GetYaxis()->SetTitle("Event unpacking time [ s ]");

  timeVsdigis_->SetTitle("Event unpacking time vs Number of digis.");
  timeVsdigis_->GetXaxis()->SetTitle("digis");
  timeVsdigis_->GetYaxis()->SetTitle("Event unpacking time [ s ]");

  timeVsclusters_->SetTitle("Event unpacking time vs Number of clusters.");
  timeVsclusters_->GetXaxis()->SetTitle("clusters");
  timeVsclusters_->GetYaxis()->SetTitle("Event unpacking time [ s ]");

  timeVsclustersize_->SetTitle("Event unpacking time vs Mean cluster size.");
  timeVsclustersize_->GetXaxis()->SetTitle("mean clusters size [ strips ]");
  timeVsclustersize_->GetYaxis()->SetTitle("Event unpacking time [ s ]");

  timeVsfrac_->SetTitle("Event unpacking time vs Fraction of SST unpacked.");
  timeVsfrac_->GetXaxis()->SetTitle("Fraction of SST");
  timeVsfrac_->GetYaxis()->SetTitle("Event unpacking time [ s ]");

  eff_hlt_->get()->SetTitle("HLT efficiency");
  eff_hlt_->get()->GetXaxis()->SetTitle("");
  eff_hlt_->get()->GetYaxis()->SetTitle("HLT Efficiency");
  
  eff_vspt_1_->get()->SetTitle("Reconstruction efficiency");
  eff_vspt_1_->get()->GetXaxis()->SetTitle("Electron P_{T} [GeV/c]");
  eff_vspt_1_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  eff_vspt_2_->get()->SetTitle("Reconstruction efficiency");
  eff_vspt_2_->get()->GetXaxis()->SetTitle("Electron P_{T} [GeV/c]");
  eff_vspt_2_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  eff_vseta_1_->get()->SetTitle("Reconstruction efficiency");
  eff_vseta_1_->get()->GetXaxis()->SetTitle("Electron #eta");
  eff_vseta_1_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  eff_vseta_2_->get()->SetTitle("Reconstruction efficiency");
  eff_vseta_2_->get()->GetXaxis()->SetTitle("Electron #eta");
  eff_vseta_2_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  eff_vseta_3_->get()->SetTitle("Reconstruction efficiency");
  eff_vseta_3_->get()->GetXaxis()->SetTitle("Electron #eta");
  eff_vseta_3_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

}

void PerformanceAnalysis::save() {

  string name = "TIME";
  TDirectory* dir = (TDirectory*)file_->Get(name.c_str());
  if (!dir) dir = file_->mkdir(name.c_str());
  dir->cd();
  time_->Write(treename_.c_str(),TObject::kOverwrite);

  name = "TIMING_VS_OCCUPANCY";
  dir = (TDirectory*)file_->Get(name.c_str());
  if (!dir) dir = file_->mkdir(name.c_str());
  dir->cd();
  timeVsocc_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "TIMING_VS_DIGIS";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  timeVsdigis_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "TIMING_VS_CLUSTERS";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  timeVsclusters_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "TIMING_VS_CLUSTERSIZE";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  timeVsclustersize_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "TIMING_VS_FRAC";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  timeVsfrac_->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_HLT";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_hlt_->calculate();
  eff_hlt_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_VSPT_1";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_vspt_1_->calculate();
  eff_vspt_1_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_VSPT_2";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_vspt_2_->calculate();
  eff_vspt_2_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_VSETA_1";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_vseta_1_->calculate();
  eff_vseta_1_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_VSETA_2";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_vseta_2_->calculate();
  eff_vseta_2_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_VSETA_3";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_vseta_3_->calculate();
  eff_vseta_3_->get()->Write(treename_.c_str(),TObject::kOverwrite);
}

