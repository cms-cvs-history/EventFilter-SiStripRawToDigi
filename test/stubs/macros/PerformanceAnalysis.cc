
#include "PerformanceAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/Constants.h"
#include <iostream>

using namespace std;

PerformanceAnalysis::PerformanceAnalysis(TFile* file,string treename) :

  file_(file),
  treename_(treename),
  tree_(0),
  time_(0),
  timeVsocc_(0),
  timeVsdigis_(0),
  timeVsclusters_(0),
  timeVsclustersize_(0),
  timeVsfrac_(0),
  timeVsevent_(0),
  timeVsoccVsfrac_(0),
  clusterwidth_(0),
  firststrip_(0),
  firststripVsclusterwidth_(0),
  eff_hlt_(0),
  eff_total_(0),
  eff_vspt_1_(0),
  eff_vspt_2_(0),
  eff_vseta_1_(0),
  eff_vseta_2_(0),
  eff_vseta_3_(0)

{;}

PerformanceAnalysis::~PerformanceAnalysis() {;}

void PerformanceAnalysis::book() {

  tree_ = (TTree*)file_->Get(treename_.c_str());
  time_ = new TH1F(treename_.c_str(), treename_.c_str(), 30,0.,0.15);
  timeVsocc_ = new TProfile(treename_.c_str(), treename_.c_str(), 50,0.,1.);
  timeVsdigis_ = new TProfile(treename_.c_str(), treename_.c_str(), 1000,50000.,150000.);
  timeVsclusters_ = new TProfile(treename_.c_str(), treename_.c_str(), 100,0.,100000.);
  timeVsclustersize_ = new TProfile(treename_.c_str(), treename_.c_str(), 7,0.,7.);
  timeVsfrac_ = new TProfile(treename_.c_str(), treename_.c_str(), 11,0.,1.1);
  timeVsevent_ = new TProfile(treename_.c_str(), treename_.c_str(), 20,0.,200.);
  timeVsoccVsfrac_ = new TProfile2D(treename_.c_str(), treename_.c_str(),10,0.,5.,10,0.,1.);
  clusterwidth_ = new TH1F(treename_.c_str(), treename_.c_str(), 20,0.,20.);
  firststrip_ = new TH1F(treename_.c_str(), treename_.c_str(), 800,0.,800.);
  firststripVsclusterwidth_ = new TProfile(treename_.c_str(), treename_.c_str(), 800,0.,800.);
  eff_hlt_ = new Efficiency(treename_.c_str(),treename_.c_str(),1,0.,1.);
  eff_total_ = new Efficiency(treename_.c_str(),treename_.c_str(),1,0.,1.);
  eff_vspt_1_ = new Efficiency(treename_.c_str(),treename_.c_str(),20,0.,100.);
  eff_vspt_2_ = new Efficiency(treename_.c_str(),treename_.c_str(),20,0.,100.);
  eff_vseta_1_ = new Efficiency(treename_.c_str(),treename_.c_str(),30,-3.,3.);
  eff_vseta_2_ = new Efficiency(treename_.c_str(),treename_.c_str(),30,-3.,3.);
  eff_vseta_3_ = new Efficiency(treename_.c_str(),treename_.c_str(),30,-3.,3.);
}

void PerformanceAnalysis::unbook() {
  if (time_) delete time_;
  if (timeVsocc_) delete timeVsocc_;
  if (timeVsdigis_) delete timeVsdigis_;
  if (timeVsclusters_) delete timeVsclusters_;
  if (timeVsclustersize_) delete timeVsclustersize_;
  if (timeVsevent_) delete timeVsevent_;
  if (timeVsfrac_) delete timeVsfrac_;
  if (timeVsoccVsfrac_) delete timeVsoccVsfrac_;
  if (clusterwidth_) delete clusterwidth_;
  if (firststrip_) delete firststrip_;
  if (firststripVsclusterwidth_) delete firststripVsclusterwidth_;
  if (eff_hlt_) delete eff_hlt_;
  if (eff_total_) delete eff_total_;
  if (eff_vspt_1_) delete eff_vspt_1_;
  if (eff_vspt_2_) delete eff_vspt_2_;
  if (eff_vseta_1_) delete eff_vseta_1_;
  if (eff_vseta_2_) delete eff_vseta_2_;
  if (eff_vseta_3_) delete eff_vseta_3_;
}

void PerformanceAnalysis::analyze(Trigger trigger) {

 if (!tree_) {
   cout << __PRETTY_FUNCTION__ 
	<< "Invalid treename." 
	<< endl;
   return;}
 
 UInt_t event;
 EventData* data = 0;
 Double_t time;
 UInt_t nchans;
 UInt_t nunpackedchans;
 
 tree_->SetBranchAddress("event", &event);
 tree_->SetBranchAddress("EventData", &data);
 tree_->SetBranchAddress("time", &time);
 tree_->SetBranchAddress("nchans", &nchans);
 tree_->SetBranchAddress("nunpackedchans", &nunpackedchans);
 
 Int_t nEvents = tree_->GetEntries();
  
  for (Int_t ievent=0; ievent<nEvents; ievent++) {

    //Collect event data
    tree_->GetEntry(ievent); 

    //Record HLT and recon efficiencies
    if (trigger==ELECTRON_SINGLE && electron_single(data->mc())) {
      electron(data->mc(),data->electrons());
      if (data->trigger().get(18)) eff_hlt_->select(1);
      else eff_hlt_->select(1,false);
    }
   
    if (trigger==ELECTRON_DOUBLE && electron_double(data->mc())) {
      electron(data->mc(),data->electrons());
      if (data->trigger().get(20)) eff_hlt_->select(1);
      else eff_hlt_->select(1,false);
    }
    
    //Loop clusters container
    Int_t ndigis = 0;
    vector<SimpleSiStripCluster>::const_iterator icluster = data->sistripClusters().begin();
    for (; icluster != data->sistripClusters().end(); icluster++) {
      ndigis+=icluster->amplitudes_;
      clusterwidth_->Fill(icluster->amplitudes_);
      firststrip_->Fill(icluster->firstStrip_);
      firststripVsclusterwidth_->Fill(icluster->firstStrip_,icluster->amplitudes_);
    }
    
    //Timing plots
    Double_t occupancy = (double)ndigis/((double)nunpackedchans*2.56);
    time_->Fill(time);
    timeVsocc_->Fill(occupancy,time);
    timeVsdigis_->Fill(ndigis,time);
    timeVsclusters_->Fill(data->sistripClusters().size(),time);
    timeVsclustersize_->Fill((Double_t)ndigis/(Double_t)data->sistripClusters().size(),time);
    timeVsfrac_->Fill((Double_t)nunpackedchans/(Double_t)nchans,time);
    timeVsevent_->Fill((Double_t)ievent,time);
    timeVsoccVsfrac_->Fill(occupancy,(Double_t)nunpackedchans/(Double_t)nchans,time);
    
  }
}

bool PerformanceAnalysis::electron_single(std::vector<SimpleParticle>& mc) {

  unsigned short count = 0;
  vector<SimpleParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {
    if ((abs(ipart->pid()) == 11) && 
	(ipart->pt() > 26.) && 
	(fabs(ipart->eta()) < constants::etaCut)) 
      count++;
  }
  if (count >= 1) return true;
  return false;
}


bool PerformanceAnalysis::electron_double(std::vector<SimpleParticle>& mc) {

  unsigned short count = 0;
  vector<SimpleParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {
    if ((abs(ipart->pid()) == 11) && 
	(ipart->pt() > 12.) && 
	(fabs(ipart->eta()) < constants::etaCut)) 
      count++;
  }
  if (count >= 2) return true;
  return false;
}


void PerformanceAnalysis::electron(std::vector<SimpleParticle>& mc, 
				   std::vector<SimpleElectron>& electrons) {
 
 vector<SimpleParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {

  if ((abs(ipart->pid()) == 11) &&
      (ipart->pt() > 10.) &&
      (fabs(ipart->eta()) < constants::etaCut)) {
    
    bool matched = false;
    vector<SimpleElectron>::const_iterator ielectron = electrons.begin();
    for (;ielectron!=electrons.end();ielectron++) {
      if (SimpleSCluster::dR(ielectron->scluster(),*ipart) < constants::dRelec) {
	matched = true; break;}
    }
    
    if (matched) eff_total_->select(1);
    else eff_total_->select(1,false);
    
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

void PerformanceAnalysis::tau(std::vector<SimpleParticle>& mc, 
			      std::vector<SimpleJet>& taus) {
 
 vector<SimpleParticle>::const_iterator ipart = mc.begin();
  for (; ipart != mc.end(); ipart++) {

  if ((abs(ipart->pid()) == 15) &&
      (ipart->pt() > 20.) &&
      (fabs(ipart->eta()) < constants::etaCut)) {
    
    bool matched = false;
    vector<SimpleJet>::const_iterator itau = taus.begin();
    for (;itau!=taus.end();itau++) {
      if (SimpleHCluster::dR(itau->hcluster(),*ipart) < constants::dRtau) {
	matched = true; break;}
    }
    
    if (matched) eff_total_->select(1);
    else eff_total_->select(1,false);
    
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

  timeVsevent_->SetTitle("Event unpacking time vs Event number.");
  timeVsevent_->GetXaxis()->SetTitle("Event number");
  timeVsevent_->GetYaxis()->SetTitle("Event unpacking time [ s ]");

  eff_hlt_->get()->SetTitle("HLT efficiency");
  eff_hlt_->get()->GetXaxis()->SetTitle("");
  eff_hlt_->get()->GetYaxis()->SetTitle("HLT Efficiency");

  eff_total_->get()->SetTitle("Electron reconstruction efficiency");
  eff_total_->get()->GetXaxis()->SetTitle("");
  eff_total_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");
  
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

  name = "TIMING_VS_EVENT";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  timeVsevent_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "TIMING_VS_OCCUPANCY_VS_FRAC";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  timeVsoccVsfrac_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "CLUSTER_WIDTH";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  clusterwidth_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "CLUSTER_FIRST";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  firststrip_->Write(treename_.c_str(),TObject::kOverwrite);
  
  name = "CLUSTER_WIDTHVSFIRST";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  firststripVsclusterwidth_->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY_HLT";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_hlt_->calculate();
  eff_hlt_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "EFFICIENCY";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  eff_total_->calculate();
  eff_total_->get()->Write(treename_.c_str(),TObject::kOverwrite);

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

