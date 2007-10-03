#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripPerformance.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"

using namespace std;
using namespace sistrip;
using namespace edm;

SiStripPerformance::SiStripPerformance( const ParameterSet& pset ) :

  sistripDigis_(pset.getParameter<edm::InputTag>("SiStripDigis")),
  sistripClusters_(pset.getParameter<edm::InputTag>("SiStripClusters")),
  particles_(pset.getParameter<edm::InputTag>("Particles")),
  electronFilter_(pset.getParameter<edm::InputTag>("ElectronFilter")),
  muonFilter_(pset.getParameter<edm::InputTag>("MuonFilter")),
  taujetFilter_(pset.getParameter<edm::InputTag>("TauJetFilter")),
  bjetFilter_(pset.getParameter<edm::InputTag>("BJetFilter")),
  timingmodules_(pset.getParameter< vector< string> >("TimingModules")),
  timingpaths_(pset.getParameter< vector< string> >("TimingPaths")),
  cabling_(),
  pdt_(),
  file_(0),
  tree_(0),
  data_(0),
  time_(0.),
  nchans_(0),
  nunpackedchans_(0)
{
  file_ = new TFile(pset.getUntrackedParameter<string>("FileName" ,"Output.root").c_str(),"UPDATE");
  tree_ = new TTree(pset.getUntrackedParameter<string>("TreeName" ,"Tree").c_str(),"");
  data_ = new SimpleEventData();
  tree_->Branch("SimpleEventData","SimpleEventData",&data_);
  tree_->Branch("time",&time_,"time/D");
  tree_->Branch("nchans",&nchans_,"nchans/i");
  tree_->Branch("nunpackedchans",&nunpackedchans_,"nunpackedchans/i"); 
}

SiStripPerformance::~SiStripPerformance() {
  if (data_) delete data_;
  if (tree_) delete tree_;
  file_->Close();
}

void SiStripPerformance::beginJob( const EventSetup& iSetup ) {
  iSetup.get<SiStripRegionCablingRcd>().get(cabling_);
  iSetup.getData(pdt_);
  nchans_ = channels(*cabling_);
}

void SiStripPerformance::endJob() {
  file_->cd();
  tree_->Write();
}

void SiStripPerformance::analyze( const Event& iEvent,const EventSetup& iSetup ) {
 
  //Refresh performance record
  data_->clear();
  data_->event()++;
  nunpackedchans_ = constants::invalid32;
  time_ = constants::invalid;
  
  //Timing
  try {
    auto_ptr<HLTPerformanceInfo> hltinfo = Service<service::PathTimerService>().operator->()->getInfo();
    time_ = moduletimer(*hltinfo, timingmodules_)+pathtimer(*hltinfo, timingpaths_);
  } catch(...) {;}
 
  //SiStripDigis and SiStripClusters
  try {
    Handle< edm::DetSetVector<SiStripDigi> > sistripDigis;
    Handle< DetSetVector<SiStripCluster> > sistripClusters;
    iEvent.getByLabel(sistripDigis_,sistripDigis);
    iEvent.getByLabel(sistripClusters_,sistripClusters);
    nunpackedchans_ = channels(*cabling_);
    sistripdigis(sistripDigis);
    sistripclusters(sistripClusters);
  } catch(...) {;}
  
  try {
    Handle< edm::DetSetVector<SiStripDigi> > sistripDigis;
    Handle< edm::SiStripRefGetter<SiStripCluster> > sistripClusters;
    iEvent.getByLabel(sistripDigis_,sistripDigis);
    iEvent.getByLabel(sistripClusters_,sistripClusters);
    nunpackedchans_ = channels(*cabling_,*sistripClusters);
    sistripdigis(sistripDigis, sistripClusters);
    sistripclusters(sistripClusters);
 } catch(...) {;}
 
  //Monte Carlo
  try {
    Handle<HepMCProduct> mcp;
    iEvent.getByLabel(particles_,mcp);
    particles(mcp);
  } catch(...) {;}
  
  //Electrons
  try {
    Handle<reco::HLTFilterObjectWithRefs> Electrons;
    iEvent.getByLabel(electronFilter_,Electrons);
    electrons(Electrons);
  } catch(...) {;}
  
  //Muons
  try {
    Handle<reco::HLTFilterObjectWithRefs> Muons;
    iEvent.getByLabel(muonFilter_,Muons);
    muons(Muons);
  } catch(...) {;}

  //Taus
  try {
    Handle<reco::HLTFilterObjectWithRefs> Taus;
    iEvent.getByLabel(taujetFilter_,Taus);
    jets(Taus);
  } catch(...) {;}

  //Bjets
  try {
    Handle<reco::HLTFilterObjectWithRefs> Bjets;
    iEvent.getByLabel(bjetFilter_,Bjets);
    jets(Bjets);
  } catch(...) {;}

  //Trigger
  try {
    Handle<TriggerResults> Trigger;
    iEvent.getByType(Trigger);
    trigger(Trigger);
  } catch(...) {;}

  //Update performance record
  tree_->Fill();
}

const double SiStripPerformance::moduletimer(HLTPerformanceInfo& hltinfo, const std::vector<std::string>& labels) {
  
  double time=0.;
  vector<string>::const_iterator iname = labels.begin();
  for (;iname != labels.end(); iname++) {
    HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo.findModule(iname->c_str());
    if (hltinfo.findModule(iname->c_str()) != hltinfo.endModules()) {time+=imodule->time();}
    else {time = constants::invalid;}
  }
  return time;
}

const double SiStripPerformance::pathtimer(HLTPerformanceInfo& hltinfo, const std::vector<std::string>& labels) {
  
  double time=0.;
  vector<string>::const_iterator iname = labels.begin();
  for (;iname != labels.end(); iname++) {
    HLTPerformanceInfo::PathList::const_iterator ipath = hltinfo.findPath(iname->c_str());
    if (ipath != hltinfo.endPaths()) {time+=ipath->time();}
    else {time = constants::invalid;}
  }
  return time;
}


const uint32_t SiStripPerformance::channels(const SiStripRegionCabling& cabling) {
  
  uint32_t channels=0;
  const Cabling& fullcabling = cabling.getRegionCabling();
  for (uint32_t iregion = 0;iregion < fullcabling.size();iregion++) {
    for (uint32_t isubdet = 0;isubdet < fullcabling[iregion].size();isubdet++) {
      for (uint32_t ilayer = 0;ilayer < fullcabling[iregion][isubdet].size();ilayer++) {
	const ElementCabling& elementcabling = fullcabling[iregion][isubdet][ilayer];
	ElementCabling::const_iterator idet = elementcabling.begin();
	for (;idet != elementcabling.end();idet++) {
	  channels+=idet->second.size();
	}
      }
    }
  }
  return channels;
}

const uint32_t SiStripPerformance::channels(const SiStripRegionCabling& cabling, const edm::SiStripRefGetter<SiStripCluster>& demandclusters) {

  uint32_t regionalchannels=0;
  const Cabling& fullcabling = cabling.getRegionCabling();
  edm::SiStripRefGetter<SiStripCluster>::const_iterator ielement = demandclusters.begin();
    for (;ielement != demandclusters.end();ielement++) { 
      uint32_t region = SiStripRegionCabling::region(ielement->region());
      uint32_t subdet = SiStripRegionCabling::subdet(ielement->region());
      uint32_t layer = SiStripRegionCabling::layer(ielement->region());
      const ElementCabling& elementcabling = fullcabling[region][subdet][layer];
      ElementCabling::const_iterator idet = elementcabling.begin();
      for (;idet!=elementcabling.end();idet++) {
	regionalchannels+=idet->second.size();
      }
    }
  return regionalchannels;
}

void SiStripPerformance::sistripdigis(const edm::Handle< DetSetVector<SiStripDigi> >& digis) {
 
  data_->sistripdigis().reserve(100000);
  DetSetVector<SiStripDigi>::const_iterator idetset = digis->begin();
  for (;idetset!=digis->end();++idetset) {
    DetSet<SiStripDigi>::const_iterator idigi = idetset->begin();
    for (;idigi!=idetset->end();++idigi) {
      data_->sistripdigis().push_back(objectconverter::sistripdigi(*idigi,idetset->id));
    }
  }
}

void SiStripPerformance::sistripdigis(const edm::Handle< edm::DetSetVector<SiStripDigi> >& digis, const edm::Handle< edm::SiStripRefGetter<SiStripCluster> >& clusters) {
  
  const Cabling& fullcabling = cabling_->getRegionCabling();
  edm::SiStripRefGetter<SiStripCluster>::const_iterator ielement = clusters->begin();
  for (;ielement != clusters->end();ielement++) { 
    uint32_t region = SiStripRegionCabling::region(ielement->region());
    uint32_t subdet = SiStripRegionCabling::subdet(ielement->region());
    uint32_t layer = SiStripRegionCabling::layer(ielement->region());
    const ElementCabling& elementcabling = fullcabling[region][subdet][layer];
    ElementCabling::const_iterator idet = elementcabling.begin();
    for (;idet!=elementcabling.end();idet++) {
      DetSetVector<SiStripDigi>::const_iterator idetset = digis->find(idet->first);
      if (idetset==digis->end()) continue;
      DetSet<SiStripDigi>::const_iterator idigi = idetset->begin();
      for (;idigi!=idetset->end();++idigi) {
	data_->sistripdigis().push_back(objectconverter::sistripdigi(*idigi,idetset->id));
      }     
    }
  }
}

void SiStripPerformance::sistripclusters(const Handle< edm::SiStripRefGetter<SiStripCluster> >& clusters) {
  
  data_->sistripclusters().reserve(100000);
  edm::SiStripRefGetter<SiStripCluster>::const_iterator iregion = clusters->begin();
  for(;iregion!=clusters->end();++iregion) {
    vector<SiStripCluster>::const_iterator icluster = iregion->begin();
    for (;icluster!=iregion->end();icluster++) {
      data_->sistripclusters().push_back(objectconverter::sistripcluster(*icluster));
    }
  }
}

void SiStripPerformance::sistripclusters(const Handle< DetSetVector<SiStripCluster> >& clusters) {
  
  data_->sistripclusters().reserve(100000);
  DetSetVector<SiStripCluster>::const_iterator idetset = clusters->begin();
  for (;idetset!=clusters->end();++idetset) {
    DetSet<SiStripCluster>::const_iterator icluster = idetset->begin();
    for (;icluster!=idetset->end();++icluster) {
      data_->sistripclusters().push_back(objectconverter::sistripcluster(*icluster));
    }
  }
}

void SiStripPerformance::particles(const Handle<HepMCProduct>& mcp) {

  const HepMC::GenEvent& gen = mcp->getHepMCData();
  for(int ibar=0; ibar < gen.particles_size(); ibar++) {
    HepMC::GenParticle* prt = gen.barcode_to_particle(ibar);
    if (prt && prt->status()==3) data_->mc().push_back(objectconverter::particle(*prt,pdt_));
  }
}

void SiStripPerformance::electrons(const Handle<reco::HLTFilterObjectWithRefs>& collection) {

  reco::HLTFilterObjectWithRefs::const_iterator ielectron = collection->begin();
  for (; ielectron != collection->end(); ielectron++) {
    data_->electrons().push_back(objectconverter::electron(*ielectron));
  }
}

void SiStripPerformance::muons(const Handle<reco::HLTFilterObjectWithRefs>& collection) {
 
  reco::HLTFilterObjectWithRefs::const_iterator imuon = collection->begin();
  for (; imuon != collection->end(); imuon++) {
    data_->muons().push_back(objectconverter::muon(*imuon));
  }
}

void SiStripPerformance::jets(const Handle<reco::HLTFilterObjectWithRefs>& collection) {

  reco::HLTFilterObjectWithRefs::const_iterator ijet = collection->begin();
  for (; ijet != collection->end(); ijet++) {
    data_->jets().push_back(objectconverter::jet(*ijet));
  }
}

void SiStripPerformance::trigger(const Handle<TriggerResults>& Trigger) {
  data_->trigger() = objectconverter::trigger(*Trigger);
}

