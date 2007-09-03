#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripPerformance.h"

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
  unpackingModuleLabels_(pset.getUntrackedParameter< vector< string> >("UnpackingModuleLabels")),
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
  allchannels();
}

void SiStripPerformance::endJob() {
  file_->cd();
  tree_->Write();
}

void SiStripPerformance::analyze( const Event& iEvent,const EventSetup& iSetup ) {
 
  //Refresh performance record
  data_->clear();
  data_->event()++;
  nunpackedchans_ = nchans_;
  
  //Timing
  try {
    auto_ptr<HLTPerformanceInfo> hltinfo = Service<service::PathTimerService>().operator->()->getInfo();
    timer(*hltinfo);
  } catch(...) {;}

  //SiStripDigis
  try {
    Handle< edm::DetSetVector<SiStripDigi> > sistripDigis;
    iEvent.getByLabel(sistripDigisModuleLabel_, sistripDigisProductLabel_, sistripDigis);
    sistripdigis(sistripDigis);
  } catch(...) {;}

  //SiStripClusters
  try {
    Handle< edm::SiStripRefGetter<SiStripCluster> > sistripClusters;
    iEvent.getByLabel(sistripClustersModuleLabel_, sistripClustersProductLabel_, sistripClusters);
    regionalchannels(sistripClusters);
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

void SiStripPerformance::timer(const HLTPerformanceInfo& hltinfo) {

  time_=0.;  
  HLTPerformanceInfo::Modules::const_iterator imodule = hltinfo.beginModules();
  for (;imodule != hltinfo.endModules(); imodule++) {
    vector<string>::const_iterator iunpacking = unpackingModuleLabels_.begin();
    for (;iunpacking != unpackingModuleLabels_.end(); iunpacking++) {
      if (imodule->name() == *iunpacking) {
	time_+=imodule->time();
      }
    }
  }
}

void SiStripPerformance::allchannels() {

 for (uint32_t iregion = 0;iregion < cabling_->getRegionCabling().size();iregion++) {
    for (uint32_t isubdet = 0;isubdet < cabling_->getRegionCabling()[iregion].size();isubdet++) {
      for (uint32_t ilayer = 0;ilayer < cabling_->getRegionCabling()[iregion][isubdet].size();ilayer++) {
	for (SiStripRegionCabling::ElementCabling::const_iterator idet = cabling_->getRegionCabling()[iregion][isubdet][ilayer].begin();idet!=cabling_->getRegionCabling()[iregion][isubdet][ilayer].end();idet++) {
	  nchans_+=idet->second.size();
	}
      }
    }
  }
}

void SiStripPerformance::regionalchannels(const Handle< edm::SiStripRefGetter<SiStripCluster> >& demandclusters) {

  nunpackedchans_=0;
  for (edm::SiStripRefGetter<SiStripCluster>::const_iterator ielement = demandclusters->begin();ielement != demandclusters->end();ielement++) { 
    for (SiStripRegionCabling::ElementCabling::const_iterator idet=cabling_->getRegionCabling()[SiStripRegionCabling::region(ielement->region())][SiStripRegionCabling::subdet(ielement->region())][SiStripRegionCabling::layer(ielement->region())].begin();idet!=cabling_->getRegionCabling()[SiStripRegionCabling::region(ielement->region())][SiStripRegionCabling::subdet(ielement->region())][SiStripRegionCabling::layer(ielement->region())].end();idet++) {
      nunpackedchans_+=idet->second.size();
    }
  }
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
    if (!prt) continue;
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

