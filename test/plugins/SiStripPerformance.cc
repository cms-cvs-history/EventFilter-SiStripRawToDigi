#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripPerformance.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"

using namespace std;
using namespace sistrip;
using namespace edm;

SiStripPerformance::SiStripPerformance( const ParameterSet& pset ) :

  sistripDigisModuleLabel_(pset.getUntrackedParameter<string>("SiStripDigisModuleLabel","")),
  sistripDigisProductLabel_(pset.getUntrackedParameter<string>("SiStripDigisProductLabel","")),
  sistripClustersModuleLabel_(pset.getUntrackedParameter<string>("SiStripClustersModuleLabel","")),
  sistripClustersProductLabel_(pset.getUntrackedParameter<string>("SiStripClustersProductLabel","")),
  mcModuleLabel_(pset.getUntrackedParameter<string>("McModuleLabel","")),
  mcProductLabel_(pset.getUntrackedParameter<string>("McProductLabel","")),
  electronsModuleLabel_(pset.getUntrackedParameter<string>("ElectronModuleLabel","")),
  electronsProductLabel_(pset.getUntrackedParameter<string>("ElectronProductLabel","")),
  tausModuleLabel_(pset.getUntrackedParameter<string>("TauModuleLabel","")),
  tausProductLabel_(pset.getUntrackedParameter<string>("TauProductLabel","")),
  timingmodules_(pset.getUntrackedParameter< vector< string> >("TimingModules")),
  timingpaths_(pset.getUntrackedParameter< vector< string> >("TimingPaths")),
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
  nchans_ = allchannels(*cabling_);
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
 
  //SiStripDigis
  try {
    Handle< edm::DetSetVector<SiStripDigi> > sistripDigis;
    iEvent.getByLabel(sistripDigisModuleLabel_, sistripDigisProductLabel_, sistripDigis);
    nunpackedchans_ = allchannels(*cabling_);
    sistripdigis(sistripDigis);
  } catch(...) {;}

  //SiStripClusters
  try {
    Handle< edm::SiStripRefGetter<SiStripCluster> > sistripClusters;
    iEvent.getByLabel(sistripClustersModuleLabel_, sistripClustersProductLabel_, sistripClusters);
    nunpackedchans_ = regionalchannels(*cabling_,*sistripClusters);
    sistripclusters(sistripClusters);
  } catch(...) {;}
 
  try {
    Handle< DetSetVector<SiStripCluster> > sistripClusters;
    iEvent.getByLabel( sistripClustersModuleLabel_, sistripClustersProductLabel_, sistripClusters );
    sistripclusters(sistripClusters);
  } catch(...) {;}
  
  //Monte Carlo
  try {
    Handle<HepMCProduct> mcp;
    iEvent.getByLabel(mcModuleLabel_,mcProductLabel_, mcp);
    particles(mcp);
  } catch(...) {;}
  
  //Electrons
  try {
    Handle<reco::HLTFilterObjectWithRefs> Electrons;
    iEvent.getByLabel( electronsModuleLabel_, electronsProductLabel_, Electrons );
    electrons(Electrons);
  } catch(...) {;}
  
  //Taus
  try {
    Handle<IsolatedTauTagInfoCollection> Taus;
    iEvent.getByLabel( tausModuleLabel_, tausProductLabel_, Taus );
    taus(Taus);
  } catch(...) {;}
  
  //Trigger
  try {
    Handle<TriggerResults> Trigger;
    iEvent.getByType( Trigger );
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


const uint32_t SiStripPerformance::allchannels(const SiStripRegionCabling& cabling) {
  
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

const uint32_t SiStripPerformance::regionalchannels(const SiStripRegionCabling& cabling, const edm::SiStripRefGetter<SiStripCluster>& demandclusters) {

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

void SiStripPerformance::electrons(const Handle<reco::HLTFilterObjectWithRefs>& Electrons) {

  reco::HLTFilterObjectWithRefs::const_iterator ielectron = Electrons->begin();
  for (; ielectron != Electrons->end(); ielectron++) {
    const reco::Electron& electron = dynamic_cast<const reco::Electron&>(*ielectron);
    data_->electrons().push_back(objectconverter::electron(electron));
  }
}

void SiStripPerformance::taus(const Handle<reco::IsolatedTauTagInfoCollection>& Taus) {
 
  reco::IsolatedTauTagInfoCollection::const_iterator itau = Taus->begin();
  for (; itau != Taus->end(); itau++) {
    const CaloJet& jet = dynamic_cast<const CaloJet&>(*(itau->jet()));
    if(itau->discriminator()) {
      data_->jets().push_back(objectconverter::jet(jet,itau->jtaRef()->second));
    }
  }
}

void SiStripPerformance::trigger(const Handle<TriggerResults>& Trigger) {
  data_->trigger() = objectconverter::trigger(*Trigger);
}

