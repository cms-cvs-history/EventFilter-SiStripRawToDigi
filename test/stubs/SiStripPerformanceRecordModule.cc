#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripPerformanceRecordModule.h"

//edm
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

// data formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"

// std
#include <cstdlib>

//Timing
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQM/HLTEvF/interface/PathTimerService.h"

using namespace std;


// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::SiStripPerformanceRecordModule( const edm::ParameterSet& pset ) :

  inputModuleLabel_(pset.getParameter<string>( "InputModuleLabel" )),
  inputProductLabel_(pset.getParameter<string>( "InputProductLabel" )),
  unpackingModuleLabels_(pset.getParameter< vector< string> >( "UnpackingModuleLabels" )),
  verbose_(pset.getUntrackedParameter<bool>("Verbose" ,false)),
  filename_(pset.getUntrackedParameter<string>("FileName" ,"Performance.root")),
  treename_(pset.getUntrackedParameter<string>("TreeName" ,"Data")),
  fedcabling_(),
  meantime_(0),
  file_(0),
  tree_(0),
  clusts_(0),
  event_(0),
  time_(0.),
  nchannels_(0),
  nunpacked_(0)
 

{
  clusts_ = new SimpleSiStripCollection(inputModuleLabel_);
  file_ = new TFile(filename_.c_str(),"UPDATE");
  tree_ = new TTree(treename_.c_str(),treename_.c_str());
  tree_->Branch("event",&event_,"event/i");
  tree_->Branch("SimpleSiStripCollection","SimpleSiStripCollection",&clusts_,16000,99);
  tree_->Branch("time",&time_,"time/D");
  tree_->Branch("nchannels",&nchannels_,"nchannels/i");
  tree_->Branch("nunpacked",&nunpacked_,"nunpacked/i");
}

// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::~SiStripPerformanceRecordModule() {
  if (tree_) delete tree_;
  file_->Close();
  if (clusts_) delete clusts_;
}

// -----------------------------------------------------------------------------
void SiStripPerformanceRecordModule::beginJob( const edm::EventSetup& iSetup ) {

  iSetup.get<SiStripFedCablingRcd>().get( fedcabling_ );

  vector<uint16_t>::const_iterator ifed = fedcabling_->feds().begin();
  for (; ifed != fedcabling_->feds().end(); ifed++) {
    nchannels_+=fedcabling_->connections(*ifed).size();}
}

// -----------------------------------------------------------------------------
void SiStripPerformanceRecordModule::endJob() {

  file_->cd();
  tree_->Write(treename_.c_str(),TObject::kOverwrite);
}

// -----------------------------------------------------------------------------
void SiStripPerformanceRecordModule::analyze( const edm::Event& iEvent, 
					      const edm::EventSetup& iSetup ) {
  
 
 

  edm::Handle< edm::DetSetVector<SiStripCluster> > clusters;
  iEvent.getByLabel( inputModuleLabel_, inputProductLabel_, clusters );
  
  //Loop clusters - store clusters

  edm::DetSetVector<SiStripCluster>::const_iterator idetset = clusters->begin();
  for (; idetset != clusters->end(); idetset++) {

    edm::DetSet<SiStripCluster>::const_iterator icluster = idetset->begin();
    for (;icluster != idetset->end() ; icluster++) {

    SimpleSiStripCluster simplecluster;
    simplecluster.detId_ = idetset->id;
    simplecluster.firstStrip_ = icluster->firstStrip();
    vector<unsigned short>::const_iterator iadc = icluster->amplitudes().begin();
    for (;iadc != icluster->amplitudes().end(); iadc++) {
      simplecluster.amplitudes_.push_back(*iadc);}
    clusts_->clusters_.push_back(simplecluster);
    }
  }

  nunpacked_ = nchannels_;

  std::auto_ptr<HLTPerformanceInfo> hltinfo = edm::Service<edm::service::PathTimerService>().operator->()->getInfo();
  HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo->beginModules();
  for (;imodule != hltinfo->endModules(); imodule++) {
    vector<string>::const_iterator iunpacking = unpackingModuleLabels_.begin();
    for (;iunpacking != unpackingModuleLabels_.end(); iunpacking++) {
      if (imodule->name() == *iunpacking) time_+=imodule->time(); 
    }
  } 
  
  meantime_ = (meantime_*(iEvent.id().event()-1) + time_)/(double)iEvent.id().event();
 
 //Debug
 if (verbose_) { print(); }
 
 //Update Performance record
 tree_->Fill();
 reset();
}

void SiStripPerformanceRecordModule::reset() {

  clusts_->clusters_.clear();
  time_ = 0.;
  nunpacked_ = 0;
}

void SiStripPerformanceRecordModule::print() {

  cout << endl;
  
  cout << "[SiStripPerformanceRecordModule::"
       << __func__
       << "]: "
       << endl;
  
  cout << "  Fraction of SST unpacked = "
       << (double)nunpacked_/(double)nchannels_
       << endl;
  
  cout << "    Event unpacking time = "
       << time_
       << endl;  

  cout << "        Mean unpacking time over run = "
       << meantime_
       << endl;  
}
