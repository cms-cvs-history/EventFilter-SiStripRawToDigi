#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripPerformanceRecordModule.h"

//FWCore
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DataFormats
#include "DataFormats/Common/interface/Handle.h"

//CalibTracker
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"

//DQM
#include "DQM/HLTEvF/interface/PathTimerService.h"

//std
#include <cstdlib>

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::SiStripPerformanceRecordModule( const edm::ParameterSet& pset ) :

  siStripDemand_(pset.getUntrackedParameter<bool>( "SiStripDemand", true )),
  recordMc_(pset.getUntrackedParameter<bool>( "RecordMc", true )),
  recordElectrons_(pset.getUntrackedParameter<bool>( "RecordElectrons", true )),
  recordSuperClusters_(pset.getUntrackedParameter<bool>( "RecordSuperClusters", true )),
  unpackingModuleLabels_(pset.getUntrackedParameter< vector< string> >( "UnpackingModuleLabels" )),
  filename_(pset.getUntrackedParameter<string>("FileName" ,"Performance.root")),
  treename_(pset.getUntrackedParameter<string>("TreeName" ,"Data")),
  cabling_(),
  meantime_(0),
  file_(0),
  tree_(0),
  data_(0),
  event_(0),
  time_(0.),
  nchans_(0),
  nunpackedchans_(0)

{
  data_ = new EventData();
  file_ = new TFile(filename_.c_str(),"RECREATE");
  tree_ = new TTree(treename_.c_str(),treename_.c_str());
  tree_->Branch("event",&event_,"event/i");
  tree_->Branch("EventData","EventData",&data_);
  tree_->Branch("time",&time_,"time/D");
  tree_->Branch("nchans",&nchans_,"nchans/i");
  tree_->Branch("nunpackedchans",&nunpackedchans_,"nunpackedchans/i");
}

// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::~SiStripPerformanceRecordModule() {
  if (tree_) delete tree_;
  file_->Close();
  if (data_) delete data_;
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

 
  //SiStripClusters
  if (siStripDemand_) {
    edm::Handle< RefGetter > Sistripclusters;
    iEvent.getByLabel("DummyRoI", "", Sistripclusters);
    sistripclusters(Sistripclusters);
    sistripchannels(Sistripclusters);
  }
  
  else {
    edm::Handle< DSV > Sistripclusters;
    iEvent.getByLabel( "SiStripRawToClustersModule", "", Sistripclusters );
    sistripclusters(Sistripclusters);
    sistripchannels();
  }

  //Monte Carlo
  if (recordMc_) {
    edm::Handle<edm::HepMCProduct> mcp;
    iEvent.getByLabel("VtxSmeared","", mcp );
    mc(mcp);
  }
  
  //SuperClusters
  if (recordSuperClusters_) {
  edm::Handle<reco::SuperClusterCollection> barrelsclusters;
  edm::Handle<reco::SuperClusterCollection> endcapsclusters;
  iEvent.getByLabel( "correctedHybridSuperClusters", "", barrelsclusters );
  iEvent.getByLabel( "correctedIslandEndcapSuperClusters","",endcapsclusters);
  sclusters(barrelsclusters);
  sclusters(endcapsclusters);
  }

  //Electrons
  if (recordElectrons_) {
  edm::Handle<reco::PixelMatchGsfElectronCollection> Electrons;
  iEvent.getByLabel( "pixelMatchGsfElectrons", "", Electrons );
  electrons(Electrons);
  }

  //Time relevent modules
  timer(iEvent.id().event());

  //Update performance record
  tree_->Fill();

  //Reset recorded values
  reset();
}

void SiStripPerformanceRecordModule::sistripchannels(const edm::Handle< RefGetter >& demandclusters) {

  SiStripRegionCabling::RegionCabling rcabling = cabling_->getRegionCabling();
  for (uint32_t iregion=0;iregion<rcabling.size();iregion++) {
    if (demandclusters->find(iregion)!=demandclusters->end()) {
      SiStripRegionCabling::RegionMap::const_iterator idet = rcabling[iregion].begin();
      for (;idet!=rcabling[iregion].end();idet++) {
	nunpackedchans_+=idet->second.size();
      }
    }
  }
}

void SiStripPerformanceRecordModule::sistripchannels() {
  nunpackedchans_ = nchans_;
}

void SiStripPerformanceRecordModule::timer(uint32_t event) {
  
  std::auto_ptr<HLTPerformanceInfo> hltinfo = edm::Service<edm::service::PathTimerService>().operator->()->getInfo();
  HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo->beginModules();
  for (;imodule != hltinfo->endModules(); imodule++) {
    vector<string>::const_iterator iunpacking = unpackingModuleLabels_.begin();
    for (;iunpacking != unpackingModuleLabels_.end(); iunpacking++) {
      if (imodule->name() == *iunpacking) time_+=imodule->time(); 
    }
  } 
  
  //Caculate "running mean" unpacking time
  meantime_ = (meantime_*(event-1) + time_)/(double)event;
  
  //Print some performance values
  cout << "[SiStripPerformanceRecordModule::"
       << __func__
       << "]:"
       << " Event unpacking time = "
       << time_
       << endl;
}

void SiStripPerformanceRecordModule::mc(const edm::Handle<edm::HepMCProduct>& mcp) {
  
  const HepMC::GenEvent& gen = mcp->getHepMCData();
 
  for(int ibar=0; ibar < gen.particles_size(); ibar++) {
    HepMC::GenParticle* prt = gen.barcode_to_particle(ibar);
    if (!prt) continue;
    SimpleParticle particle(prt->momentum().perp(),
			    -1.0*log(tan(0.5*(prt->momentum().theta()))),
			    prt->momentum().phi(),
			    prt->production_vertex()->position().x(),
			    prt->production_vertex()->position().y(),
			    prt->production_vertex()->position().z(),
			    prt->pdg_id());
    data_->mc().push_back(particle);
  }
}

void SiStripPerformanceRecordModule::sistripclusters(const edm::Handle< RefGetter >& clusts) {
  
  data_->sistripClusters().reserve(1000000);
  RefGetter::const_iterator iregion = clusts->begin();
  for(;iregion!=clusts->end();++iregion) {
    vector<SiStripCluster>::const_iterator icluster = iregion->first;
    for (;icluster!=iregion->second;icluster++) {
      data_->sistripClusters().push_back(SimpleSiStripCluster(icluster->geographicalId(), icluster->firstStrip(), icluster->amplitudes().size(), icluster->barycenter()));
    }
  }
}

void SiStripPerformanceRecordModule::sistripclusters(const edm::Handle< DSV >& clusts) {
  
  data_->sistripClusters().reserve(1000000);
  DSV::const_iterator idetset = clusts->begin();
  for (;idetset!=clusts->end();++idetset) {
    DetSet::const_iterator icluster = idetset->begin();
    for (;icluster!=idetset->end();++icluster) {
      data_->sistripClusters().push_back(SimpleSiStripCluster(icluster->geographicalId(), icluster->firstStrip(), icluster->amplitudes().size(), icluster->barycenter()));
    }
  }
}

void SiStripPerformanceRecordModule::electrons(const edm::Handle<reco::PixelMatchGsfElectronCollection>& Electrons) {

  reco::PixelMatchGsfElectronCollection::const_iterator ielectron = Electrons->begin();
  for (; ielectron != Electrons->end(); ielectron++) {
    
    SimpleTrack track(ielectron->track().get()->momentum().Rho(), 
		      ielectron->track().get()->momentum().Eta(), 
		      ielectron->track().get()->momentum().Phi(), 
		      ielectron->track().get()->vertex().X(),
		      ielectron->track().get()->vertex().Y(),
		      ielectron->track().get()->vertex().Z(),
		      ielectron->track().get()->outerMomentum().Rho(), 
		      ielectron->track().get()->outerMomentum().Eta(), 
		      ielectron->track().get()->outerMomentum().Phi(), 
		      ielectron->track().get()->outerPosition().X(),
		      ielectron->track().get()->outerPosition().Y(),
		      ielectron->track().get()->outerPosition().Z(),
		      ielectron->track().get()->charge());
    
    SimpleSCluster scluster(ielectron->superCluster().get()->energy(),
			    ielectron->superCluster().get()->eta(),
			    ielectron->superCluster().get()->phi());
    
    SimpleElectron electron(track,
			    scluster);

    data_->electrons().push_back(electron);
  }
}

void SiStripPerformanceRecordModule::sclusters(const edm::Handle<reco::SuperClusterCollection>& sClusters) {
  
  reco::SuperClusterCollection::const_iterator iscluster = sClusters->begin();
  for (;iscluster!=sClusters->end();iscluster++) {
    data_->sClusters().push_back(SimpleSCluster(iscluster->rawEnergy(),iscluster->seed()->position().eta(),iscluster->seed()->position().phi()));
  }
}

void SiStripPerformanceRecordModule::reset() {
  data_->clear();
  nunpackedchans_ = 0;
  time_ = 0.;
}
