#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripPerformanceRecordModule.h"

//edm
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//Calib Tracker
#include "CalibTracker/SiStripConnectivity/test/SiStripRegionCablingRcd.h"

//std
#include <cstdlib>

//Timing
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQM/HLTEvF/interface/PathTimerService.h"

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::SiStripPerformanceRecordModule( const edm::ParameterSet& pset ) :

  demand_(pset.getUntrackedParameter<bool>( "Demand", false )),
  inputModuleLabel_(pset.getUntrackedParameter<string>( "InputModuleLabel" ,"")),
  inputProductLabel_(pset.getUntrackedParameter<string>( "InputProductLabel" ,"")),
  unpackingModuleLabels_(pset.getUntrackedParameter< vector< string> >( "UnpackingModuleLabels" )),
  filename_(pset.getUntrackedParameter<string>("FileName" ,"Performance.root")),
  treename_(pset.getUntrackedParameter<string>("TreeName" ,"Data")),
  cabling_(),
  meantime_(0),
  file_(0),
  tree_(0),
  clusts_(0),
  event_(0),
  time_(0.),
  nchans_(0),
  nunpackedchans_(0)

{
  clusts_ = new SimpleSiStripCollection(inputModuleLabel_);
  file_ = new TFile(filename_.c_str(),"UPDATE");
  tree_ = new TTree(treename_.c_str(),treename_.c_str());
  tree_->Branch("event",&event_,"event/i");
  tree_->Branch("SimpleSiStripCollection","SimpleSiStripCollection",&clusts_);
  tree_->Branch("time",&time_,"time/D");
  tree_->Branch("nchans",&nchans_,"nchans/i");
  tree_->Branch("nunpackedchans",&nunpackedchans_,"nunpackedchans/i");
}

// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::~SiStripPerformanceRecordModule() {
  if (tree_) delete tree_;
  file_->Close();
  if (clusts_) delete clusts_;
}

// -----------------------------------------------------------------------------
void SiStripPerformanceRecordModule::beginJob( const edm::EventSetup& iSetup ) {

  iSetup.get<SiStripRegionCablingRcd>().get(cabling_);

  SiStripRegionCabling::RegionCabling rcabling = cabling_->getRegionCabling();
  for (uint32_t iregion=0;iregion<rcabling.size();iregion++) {
    SiStripRegionCabling::RegionMap::const_iterator idet = rcabling[iregion].begin();
    for (;idet!=rcabling[iregion].end();idet++) {
      nchans_+=idet->second.size();
    }
  }
}

// -----------------------------------------------------------------------------
void SiStripPerformanceRecordModule::endJob() {
  file_->cd();
  tree_->Write(treename_.c_str(),TObject::kOverwrite);

  cout << "[SiStripPerformanceRecordModule::"
       << __func__
       << "]:"
       << " Mean unpacking time over run = "
       << meantime_ 
       << endl;
}

// -----------------------------------------------------------------------------
void SiStripPerformanceRecordModule::analyze( const edm::Event& iEvent, 
					      const edm::EventSetup& iSetup ) {

  //Get Clusters from event
  if (demand_) {
    edm::Handle< RefGetter > demandclusters;
    iEvent.getByLabel(inputModuleLabel_,inputProductLabel_,demandclusters);

    //Count unpacked channels
    SiStripRegionCabling::RegionCabling rcabling = cabling_->getRegionCabling();
    for (uint32_t iregion=0;iregion<rcabling.size();iregion++) {
      if (demandclusters->find(iregion)!=demandclusters->end()) {
	SiStripRegionCabling::RegionMap::const_iterator idet = rcabling[iregion].begin();
	for (;idet!=rcabling[iregion].end();idet++) {
	  nunpackedchans_+=idet->second.size();
	}
      }
    }
  
    //Record unpacked clusters
    clusters(*demandclusters);
  }

  else {
    edm::Handle< DSV > allclusters;
    iEvent.getByLabel( inputModuleLabel_, inputProductLabel_, allclusters );
    clusters(*allclusters);
    nunpackedchans_ = nchans_;
  }
    
    //Time relevent modules
    std::auto_ptr<HLTPerformanceInfo> hltinfo = edm::Service<edm::service::PathTimerService>().operator->()->getInfo();
    HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo->beginModules();
    for (;imodule != hltinfo->endModules(); imodule++) {
      vector<string>::const_iterator iunpacking = unpackingModuleLabels_.begin();
      for (;iunpacking != unpackingModuleLabels_.end(); iunpacking++) {
	if (imodule->name() == *iunpacking) time_+=imodule->time(); 
      }
    } 
    
  //Caculate "running mean" unpacking time
  meantime_ = (meantime_*(iEvent.id().event()-1) + time_)/(double)iEvent.id().event();
  
  //Print some performance values
  cout << "[SiStripPerformanceRecordModule::"
       << __func__
       << "]:"
       << " Event unpacking time = "
       << time_
       << endl;
  
  //Update performance record
  tree_->Fill();

  //Reset recorded values
  reset();
}

void SiStripPerformanceRecordModule::clusters(const RefGetter& clusts) {
  
  clusts_->clusters_.reserve(1000000);
  RefGetter::const_iterator iregion = clusts.begin();
  for(;iregion!=clusts.end();iregion++) {
    std::vector< DetSet >::const_iterator idetset = iregion->second.begin();
    for(;idetset!=iregion->second.end();idetset++) {
      DetSet::const_iterator icluster = idetset->begin();
      for(;icluster!=idetset->end();icluster++) {
	SimpleSiStripCluster simplecluster;
	convert(*icluster,simplecluster);
	clusts_->clusters_.push_back(simplecluster);
      }
    }
  }
}

void SiStripPerformanceRecordModule::clusters(const DSV& clusts) {
  
  clusts_->clusters_.reserve(1000000);
  DSV::const_iterator idetset = clusts.begin();
  for (;idetset!=clusts.end();++idetset) {
    DetSet::const_iterator icluster = idetset->begin();
      for (;icluster!=idetset->end();++icluster) {
        SimpleSiStripCluster simplecluster;
        convert(*icluster,simplecluster);
        clusts_->clusters_.push_back(simplecluster);
      }
  }
}

void SiStripPerformanceRecordModule::convert(const SiStripCluster& cluster,
					     SimpleSiStripCluster& simplecluster) {
  simplecluster.detId_ = cluster.geographicalId();
  simplecluster.firstStrip_ = cluster.firstStrip();
  simplecluster.baryCenter_ = cluster.barycenter();
  simplecluster.amplitudes_ = cluster.amplitudes().size();
}

void SiStripPerformanceRecordModule::reset() {
  
  clusts_->clusters_.clear();
  nunpackedchans_ = 0;
  time_ = 0.;
}
