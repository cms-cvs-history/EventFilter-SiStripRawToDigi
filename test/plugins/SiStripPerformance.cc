#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripPerformance.h"

//FWCore
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DataFormats
#include "DataFormats/Common/interface/HLTenums.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h" 
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

//CalibTracker
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"

//DQM
#include "DQM/HLTEvF/interface/PathTimerService.h"

//std
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace sistrip;
using namespace edm;

// -----------------------------------------------------------------------------

SiStripPerformance::SiStripPerformance( const ParameterSet& pset ) :

  sistripDemand_(pset.getUntrackedParameter<bool>("SiStripDemand", true)),
  sistripClustersModuleLabel_(pset.getUntrackedParameter<string>("SiStripClustersModuleLabel","")),
  sistripClustersProductLabel_(pset.getUntrackedParameter<string>("SiStripClustersProductLabel","")),
  mcModuleLabel_(pset.getUntrackedParameter<string>("McModuleLabel","")),
  mcProductLabel_(pset.getUntrackedParameter<string>("McProductLabel","")),
  electronsModuleLabel_(pset.getUntrackedParameter<string>("ElectronModuleLabel","")),
  electronsProductLabel_(pset.getUntrackedParameter<string>("ElectronProductLabel","")),
  tausModuleLabel_(pset.getUntrackedParameter<string>("TauModuleLabel","")),
  tausProductLabel_(pset.getUntrackedParameter<string>("TauProductLabel","")),
  hlTriggerResults_(pset.getParameter<InputTag>("HLTriggerResults")),
  unpackingModuleLabels_(pset.getUntrackedParameter< vector< string> >("UnpackingModuleLabels")),
  filename_(pset.getUntrackedParameter<string>("FileName" ,"Performance.root")),
  treename_(pset.getUntrackedParameter<string>("TreeName" ,"Data")),
  cabling_(),
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

SiStripPerformance::~SiStripPerformance() {
  if (tree_) delete tree_;
  file_->Close();
  if (data_) delete data_;
}

// -----------------------------------------------------------------------------

void SiStripPerformance::beginJob( const EventSetup& iSetup ) {
  iSetup.get<SiStripRegionCablingRcd>().get(cabling_);

  for (uint32_t iregion = 0;
       iregion < cabling_->getRegionCabling().size();
       iregion++) {
    
    for (uint32_t isubdet = 3; 
	 isubdet < cabling_->getRegionCabling()[iregion].size(); 
	 isubdet++) {
      
      for (uint32_t ilayer = 0; 
	   ilayer < cabling_->getRegionCabling()[iregion][isubdet].size(); 
	   ilayer++) {

	SiStripRegionCabling::ElementCabling::const_iterator idet = cabling_->getRegionCabling()[iregion][isubdet][ilayer].begin();
	for (;idet!=cabling_->getRegionCabling()[iregion][isubdet][ilayer].end();idet++) {
	  nchans_+=idet->second.size();
	}
      }
    }
  }
}


// -----------------------------------------------------------------------------

void SiStripPerformance::endJob() {
  file_->cd();
  tree_->Write(treename_.c_str(),TObject::kOverwrite);
}

// -----------------------------------------------------------------------------

void SiStripPerformance::analyze( const Event& iEvent, 
				  const EventSetup& iSetup ) {

  //Event number
  event_++;

  //Timer
  timer();

  //SiStripClusters
  if (sistripDemand_) {
    try {
    Handle< RefGetter > sistripClusters;
    iEvent.getByLabel(sistripClustersModuleLabel_, sistripClustersProductLabel_, sistripClusters);
    sistripchannels(sistripClusters);
    sistripclusters(sistripClusters);
    } catch(...) {;}
  } 
  
  else {
    try {
      Handle< DSV > sistripClusters;
      iEvent.getByLabel( sistripClustersModuleLabel_, sistripClustersProductLabel_, sistripClusters );
      sistripclusters(sistripClusters);
    } catch(...) {;}
  }
 
  //Monte Carlo
    try {
      Handle<HepMCProduct> mcp;
      iEvent.getByLabel(mcModuleLabel_,mcProductLabel_, mcp);
      mc(mcp);
    } catch(...) {;}
  
    //Electrons
    try {
      Handle<reco::HLTFilterObjectWithRefs> Electrons;
      iEvent.getByLabel( electronsModuleLabel_, electronsProductLabel_, Electrons );
      electrons(Electrons);
    } catch(...) {;}
  
    //Taus
    try {
      Handle<reco::HLTFilterObjectWithRefs> Taus;
      Handle<reco::JetTracksAssociationCollection> JTAs;
      iEvent.getByLabel( tausModuleLabel_, tausProductLabel_, Taus );
      iEvent.getByLabel( "associatorL3SingleTau", "", JTAs );
      taus(Taus,JTAs);
    } catch(...) {;}
  
    //Trigger
    try {
      Handle<TriggerResults> Trigger;
      iEvent.getByLabel( hlTriggerResults_, Trigger );
      trigger(Trigger);
    } catch(...) {;}
    
  //Update performance record
  tree_->Fill();

  //Reset recorded values
  reset();
}

void SiStripPerformance::timer() {
  
  std::auto_ptr<HLTPerformanceInfo> hltinfo = Service<service::PathTimerService>().operator->()->getInfo();
  HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo->beginModules();
  for (;imodule != hltinfo->endModules(); imodule++) {
    vector<string>::const_iterator iunpacking = unpackingModuleLabels_.begin();
    for (;iunpacking != unpackingModuleLabels_.end(); iunpacking++) {
      if (imodule->name() == *iunpacking) time_+=imodule->time();
    }
  }
}

void SiStripPerformance::sistripchannels(const Handle< RefGetter >& demandclusters) {

  nunpackedchans_=0;

  for (uint32_t iregion = 0;
       iregion < cabling_->getRegionCabling().size();
       iregion++) {
    
    if (demandclusters->find(iregion)!=demandclusters->end()) {
     
    for (uint32_t isubdet = 0; 
	 isubdet < cabling_->getRegionCabling()[iregion].size(); 
	 isubdet++) {
      
      for (uint32_t ilayer = 0; 
	   ilayer < cabling_->getRegionCabling()[iregion][isubdet].size(); 
	   ilayer++) {

	SiStripRegionCabling::ElementCabling::const_iterator idet = cabling_->getRegionCabling()[iregion][isubdet][ilayer].begin();
	for (;idet!=cabling_->getRegionCabling()[iregion][isubdet][ilayer].end();idet++) {
	  nchans_+=idet->second.size();
	}
      }
    }
    }
  }
}

void SiStripPerformance::sistripclusters(const Handle< RefGetter >& clusts) {
  
  data_->sistripClusters().reserve(1000000);
  RefGetter::const_iterator iregion = clusts->begin();
  for(;iregion!=clusts->end();++iregion) {
    vector<SiStripCluster>::const_iterator icluster = iregion->begin();
    for (;icluster!=iregion->end();icluster++) {

      SimpleSiStripCluster cluster(icluster->geographicalId(), 
				   icluster->firstStrip(), 
				   icluster->amplitudes().size(), 
				   icluster->barycenter());

      data_->sistripClusters().push_back(cluster);
    }
  }
}

void SiStripPerformance::sistripclusters(const Handle< DSV >& clusts) {
  
  data_->sistripClusters().reserve(1000000);
  DSV::const_iterator idetset = clusts->begin();
  for (;idetset!=clusts->end();++idetset) {
    DetSet::const_iterator icluster = idetset->begin();
    for (;icluster!=idetset->end();++icluster) {
      
      SimpleSiStripCluster cluster(icluster->geographicalId(),
				   icluster->firstStrip(), 
				   icluster->amplitudes().size(), 
				   icluster->barycenter());

      data_->sistripClusters().push_back(cluster);
      
    }
  }
}

void SiStripPerformance::mc(const Handle<HepMCProduct>& mcp) {

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

void SiStripPerformance::electrons(const Handle<reco::HLTFilterObjectWithRefs>& Electrons) {
 
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
		      elec.track().get()->charge(),
		      elec.track().get()->found());
    
    SimpleSCluster scluster(elec.superCluster().get()->energy(),
			    elec.superCluster().get()->eta(),
			    elec.superCluster().get()->phi());
    
    SimpleElectron electron(track,
			    scluster);

    data_->electrons().push_back(electron);
  }
}

void SiStripPerformance::taus(const Handle<reco::HLTFilterObjectWithRefs>& Taus, const Handle<reco::JetTracksAssociationCollection>& JTAs) {
  
  for (size_t i=0; i<Taus->size(); i++) {

    const edm::ProductID& id = Taus->getPID(i);
    reco::JetTracksAssociationCollection::const_iterator taujta=JTAs->end();
    reco::JetTracksAssociationCollection::const_iterator ijta=JTAs->begin();
    for (;ijta!=JTAs->end();ijta++) {
      if (ijta->first.id() == id) taujta = ijta;
      break;
    }
   
    SimpleHCluster hcluster(taujta->first.get()->energy(),
			    taujta->first.get()->energy(),
			    taujta->first.get()->eta(),
			    taujta->first.get()->phi());
    
    std::vector<SimpleTrack> tracks;
    RefVector<reco::TrackCollection>::const_iterator itrack=taujta->second.begin();
    for (;itrack!=taujta->second.end();++itrack) {
      tracks.push_back(SimpleTrack(itrack->get()->momentum().Rho(), 
				   itrack->get()->momentum().Eta(), 
				   itrack->get()->momentum().Phi(), 
				   itrack->get()->vertex().X(),
				   itrack->get()->vertex().Y(),
				   itrack->get()->vertex().Z(),
				   itrack->get()->outerMomentum().Rho(), 
				   itrack->get()->outerMomentum().Eta(), 
				   itrack->get()->outerMomentum().Phi(), 
				   itrack->get()->outerPosition().X(),
				   itrack->get()->outerPosition().Y(),
				   itrack->get()->outerPosition().Z(),
				   itrack->get()->charge(),
				   itrack->get()->found()));      
    }
    
    SimpleJet jet(taujta->first.get()->vertex().X(),
		  taujta->first.get()->vertex().Y(),
		  taujta->first.get()->vertex().Z(),
		  hcluster,
		  tracks,
		  0.);

    data_->taus().push_back(jet);
  }
}

void SiStripPerformance::trigger(const Handle<TriggerResults>& Trigger) {
    
  unsigned int num = Trigger->size();
  SimpleTrigger trigger(num);
  for (unsigned int i=0;i<num;i++) {
    trigger.set(i,(Trigger->at(i).state() == hlt::Pass));
  }
  data_->trigger() = trigger;
}

void SiStripPerformance::reset() {
  data_->clear();
  nunpackedchans_ = nchans_;
  time_ = 0.;
}
