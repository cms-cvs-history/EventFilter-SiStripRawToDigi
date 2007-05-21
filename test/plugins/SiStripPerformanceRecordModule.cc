#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripPerformanceRecordModule.h"

//FWCore
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DataFormats
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h" 

//CalibTracker
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"

//DQM
#include "DQM/HLTEvF/interface/PathTimerService.h"

//std
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------

SiStripPerformanceRecordModule::SiStripPerformanceRecordModule( const edm::ParameterSet& pset ) :

  sistripDemand_(pset.getUntrackedParameter<bool>("SiStripDemand", true)),
  sistripClustersModuleLabel_(pset.getUntrackedParameter<string>("SiStripClustersModuleLabel","")),
  sistripClustersProductLabel_(pset.getUntrackedParameter<string>("SiStripClustersProductLabel","")),
  recordMc_(pset.getUntrackedParameter<bool>("RecordMc", true)),
  mcModuleLabel_(pset.getUntrackedParameter<string>("McModuleLabel","")),
  mcProductLabel_(pset.getUntrackedParameter<string>("McProductLabel","")),
  recordElectrons_(pset.getUntrackedParameter<bool>("RecordElectrons", true)),
  electronsModuleLabel_(pset.getUntrackedParameter<string>("ElectronsModuleLabel","")),
  electronsProductLabel_(pset.getUntrackedParameter<string>("ElectronsProductLabel","")),
  unpackingModuleLabels_(pset.getUntrackedParameter< vector< string> >("UnpackingModuleLabels")),
  filename_(pset.getUntrackedParameter<string>("FileName" ,"Performance.root")),
  treename_(pset.getUntrackedParameter<string>("TreeName" ,"Data")),
  cabling_(),
  sumtime_(0.),
  sumtime2_(0.),
  file_(0),
  tree_(0),
  data_(0),
  event_(0),
  time_(0.),
  nchans_(0),
  nunpackedchans_(0)

{
  data_ = new EventData();
  file_ = new TFile(filename_.c_str(),"UPDATE");
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

  std::ofstream output("output.txt",ios::app);
  output << "Timing Run : "
	 << filename_
	 << ":"
	 << treename_
	 << ". Unpacking time : "
	 << sumtime_/event_
	 << " +/- "
	 << sqrt(fabs((sumtime2_/event_)-(sumtime_/event_)*(sumtime_/event_))) 
	 << std::endl;
  output.close();
}

// -----------------------------------------------------------------------------

void SiStripPerformanceRecordModule::analyze( const edm::Event& iEvent, 
					      const edm::EventSetup& iSetup ) {

  //Event number
  event_++;

  //Timer
  timer();

  //SiStripClusters
  if (sistripDemand_) {
    edm::Handle< RefGetter > Sistripclusters;
    iEvent.getByLabel(sistripClustersModuleLabel_, sistripClustersProductLabel_, Sistripclusters);
    sistripclusters(Sistripclusters);
    sistripchannels(Sistripclusters);}
  
  else {
    edm::Handle< DSV > Sistripclusters;
    iEvent.getByLabel( sistripClustersModuleLabel_, sistripClustersProductLabel_, Sistripclusters );
    sistripclusters(Sistripclusters);
    sistripchannels();}
 
  //Monte Carlo
  if (recordMc_) {
    edm::Handle<edm::HepMCProduct> mcp;
    iEvent.getByLabel(mcModuleLabel_,mcProductLabel_, mcp);
    mc(mcp);}
  
  //Electrons
  if (recordElectrons_) {
  edm::Handle<reco::HLTFilterObjectWithRefs> Electrons;
  iEvent.getByLabel( electronsModuleLabel_, electronsProductLabel_, Electrons );
  electrons(Electrons);}

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

void SiStripPerformanceRecordModule::timer() {
  
  std::auto_ptr<HLTPerformanceInfo> hltinfo = edm::Service<edm::service::PathTimerService>().operator->()->getInfo();
  HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo->beginModules();
  double time = 0.;
  for (;imodule != hltinfo->endModules(); imodule++) {
    vector<string>::const_iterator iunpacking = unpackingModuleLabels_.begin();
    for (;iunpacking != unpackingModuleLabels_.end(); iunpacking++) {
      if (imodule->name() == *iunpacking) time+=imodule->time();}}
  sumtime_+=time;
  sumtime2_+=time*time; 
}

void SiStripPerformanceRecordModule::mc(const edm::Handle<edm::HepMCProduct>& mcp) {

  const HepMC::GenEvent& gen = mcp->getHepMCData();
  for(int ibar=0; ibar < gen.particles_size(); ibar++) {
    HepMC::GenParticle* prt = gen.barcode_to_particle(ibar);
    if (!prt) continue; 
    SimpleParticle particle(prt->momentum().perp(),
			    -1.0*log(tan(0.5*(prt->momentum().theta()))),
			    prt->momentum().phi(),
			    0.,//prt->production_vertex()->position().x(),
			    0.,//prt->production_vertex()->position().y(),
			    0.,//prt->production_vertex()->position().z(),
			    prt->pdg_id(),
			    0); //@@ fix this
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

void SiStripPerformanceRecordModule::electrons(const edm::Handle<reco::HLTFilterObjectWithRefs>& Electrons) {
 
  reco::HLTFilterObjectWithRefs::const_iterator ielectron = Electrons->begin();
  for (; ielectron != Electrons->end(); ielectron++) {
    reco::Candidate& eleccand = const_cast<reco::Candidate&>(*ielectron);
    reco::Electron& elec = dynamic_cast<reco::Electron&>(eleccand);

    SimpleTrack track(elec.track().get()->momentum().Rho(), 
		      elec.track().get()->momentum().Eta(), 
		      elec.track().get()->momentum().Phi(), 
		      elec.track().get()->vertex().X(),
		      elec.track().get()->vertex().Y(),
		      elec.track().get()->vertex().Z(),
		      elec.track().get()->outerMomentum().Rho(), 
		      elec.track().get()->outerMomentum().Eta(), 
		      elec.track().get()->outerMomentum().Phi(), 
		      elec.track().get()->outerPosition().X(),
		      elec.track().get()->outerPosition().Y(),
		      elec.track().get()->outerPosition().Z(),
		      elec.track().get()->charge());
    
    SimpleSCluster scluster(elec.superCluster().get()->energy(),
			    elec.superCluster().get()->eta(),
			    elec.superCluster().get()->phi());
    
    SimpleElectron electron(track,
			    scluster);

    data_->electrons().push_back(electron);
  }
}


void SiStripPerformanceRecordModule::reset() {
  data_->clear();
  nunpackedchans_ = 0;
  time_ = 0.;
}
