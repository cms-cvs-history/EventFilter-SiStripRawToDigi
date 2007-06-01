
#include "PerformanceAnalysis.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/EventData.h"
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
  electronVspt_barrel_(0),
  electronVspt_endcap_(0),
  electronVseta_lt100_(0),
  electronVseta_gt100_(0)

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
  electronVspt_barrel_ = new Efficiency(treename_.c_str(),treename_.c_str(),20,0.,100.);
  electronVspt_endcap_ = new Efficiency(treename_.c_str(),treename_.c_str(),20,0.,100.);
  electronVseta_lt100_ = new Efficiency(treename_.c_str(),treename_.c_str(),10,-3.,3.);
  electronVseta_gt100_ = new Efficiency(treename_.c_str(),treename_.c_str(),10,-3.,3.);
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
  if (electronVspt_barrel_) delete electronVspt_barrel_;
  if (electronVspt_endcap_) delete electronVspt_endcap_;
  if (electronVseta_lt100_) delete electronVseta_lt100_;
  if (electronVseta_gt100_) delete electronVseta_gt100_;
}

void PerformanceAnalysis::analyze() {

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

    //Loop Monte-Carlo
    vector<SimpleParticle>::const_iterator ipart = data->mc().begin();
    for (; ipart != data->mc().end(); ipart++) {
      
      //Electron efficiency
      if ((abs(ipart->pid()) == 11) &&
	  (fabs(ipart->eta()) < constants::etaCut) &&
	  (ipart->pt() > 10.)) {
      
	bool matched = false;
	vector<SimpleElectron>::const_iterator ielectron = data->electrons().begin();
	for (;ielectron!=data->electrons().end();ielectron++) {
	  if (SimpleSCluster::dR(ielectron->scluster(),*ipart) < constants::dRelec) {
	    matched = true; break;}
	}

	if (ipart->eta() < constants::ecEcal) {
	  if (matched) electronVspt_barrel_->select((Double_t)ipart->pt());
	  else electronVspt_barrel_->select((Double_t)ipart->pt(),false);
	}

	if (ipart->eta() > constants::ecEcal) {
	  if (matched) electronVspt_endcap_->select((Double_t)ipart->pt());
	  else electronVspt_endcap_->select((Double_t)ipart->pt(),false);
	}

	if (ipart->pt() < 100.) {
	  if (matched) electronVseta_lt100_->select((Double_t)ipart->eta());
	  else electronVseta_lt100_->select((Double_t)ipart->eta(),false);
	}

	if (ipart->pt() > 100.) {
	  if (matched) electronVseta_gt100_->select((Double_t)ipart->eta());
	  else electronVseta_gt100_->select((Double_t)ipart->eta(),false);
	}
      }
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

  name = "ELECTRONVSPT_BARREL";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  electronVspt_barrel_->calculate();
  electronVspt_barrel_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "ELECTRONVSPT_ENDCAP";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  electronVspt_endcap_->calculate();
  electronVspt_endcap_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "ELECTRONVSETA_LT100";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  electronVseta_lt100_->calculate();
  electronVseta_lt100_->get()->Write(treename_.c_str(),TObject::kOverwrite);

  name = "ELECTRONVSETA_GT100";
  file_->mkdir(name.c_str());
  file_->GetDirectory(name.c_str())->cd();
  electronVseta_gt100_->calculate();
  electronVseta_gt100_->get()->Write(treename_.c_str(),TObject::kOverwrite);
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

  electronVspt_barrel_->get()->SetTitle("Barrel electron reconstruction efficiency");
  electronVspt_barrel_->get()->GetXaxis()->SetTitle("Electron P_{T} [GeV/c]");
  electronVspt_barrel_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  electronVspt_endcap_->get()->SetTitle("Endcap electron reconstruction efficiency");
  electronVspt_endcap_->get()->GetXaxis()->SetTitle("Electron P_{T} [GeV/c]");
  electronVspt_endcap_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  electronVseta_lt100_->get()->SetTitle("Electron reconstruction efficiency (<100 GeV)");
  electronVseta_lt100_->get()->GetXaxis()->SetTitle("Electron #eta");
  electronVseta_lt100_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

  electronVseta_gt100_->get()->SetTitle("Electron reconstruction efficiency (>100 GeV)");
  electronVseta_gt100_->get()->GetXaxis()->SetTitle("Electron #eta");
  electronVseta_gt100_->get()->GetYaxis()->SetTitle("Reconstruction Efficiency");

}
