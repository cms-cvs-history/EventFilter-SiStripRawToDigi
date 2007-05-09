#include "EventFilter/SiStripRawToDigi/test/plugins/SiStripRawToClustersDummyRoI.h"

//FWCore
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//Data Formats
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/SiStripCommon/test/stubs/SiStripLazyGetter.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

//CalibFormats
#include "CalibTracker/SiStripConnectivity/test/stubs/SiStripRegionCablingRcd.h"

//CLHEP
#include "CLHEP/Random/RandFlat.h"

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------
//
SiStripRawToClustersDummyRoI::SiStripRawToClustersDummyRoI( const edm::ParameterSet& conf ) :

  inputModuleLabel_(conf.getUntrackedParameter<string>("InputModuleLabel","")),
  cabling_(),
  period_(conf.getUntrackedParameter<unsigned int>("Period",10))
  
{
  LogTrace(mlRawToCluster_)
    << "[SiStripRawToClustersDummyRoI::" 
    << __func__ 
    << "]"
    << " Constructing object...";
  
  produces< RefGetter >();
}

// -----------------------------------------------------------------------------
/** */
SiStripRawToClustersDummyRoI::~SiStripRawToClustersDummyRoI() {

  LogTrace(mlRawToCluster_)
    << "[SiStripRawToClustersDummyRoI::" 
    << __func__ 
    << "]"
    << " Destructing object...";
}

// -----------------------------------------------------------------------------
void SiStripRawToClustersDummyRoI::beginJob( const edm::EventSetup& setup) {

  LogTrace(mlRawToCluster_) 
    << "[SiStripRawToClustersDummyRoI::"
    << __func__ 
    << "]";

 //Fill cabling
  setup.get<SiStripRegionCablingRcd>().get(cabling_);
}

// -----------------------------------------------------------------------------
void SiStripRawToClustersDummyRoI::endJob() {;}

// -----------------------------------------------------------------------------
/** */
void SiStripRawToClustersDummyRoI::produce( edm::Event& event, 
					    const edm::EventSetup& setup ) {
  
  // Retrieve unpacking tool from event
  edm::Handle<edm::SiStripLazyGetter<SiStripCluster> > getter;
  event.getByLabel(inputModuleLabel_,"",getter);
  
  // Retrieve superclusters from event
  edm::Handle<reco::SuperClusterCollection> barrelsclusters;
  edm::Handle<reco::SuperClusterCollection> endcapsclusters;
  event.getByLabel("correctedHybridSuperClusters","",barrelsclusters);
  event.getByLabel("correctedIslandEndcapSuperClusters","",endcapsclusters);
  
  // Create regions of interest vector
  SiStripRegionCabling::Regions demand;
  demand.reserve(cabling_->getRegionCabling().size());

  // Define regions of interest for unpacking
  superclusters(*barrelsclusters,demand);
  superclusters(*endcapsclusters,demand);

  // Add regions of interest to RefGetter object
  std::auto_ptr<RefGetter> region(new RefGetter(getter,demand));

  // Add to event
  event.put(region);
}


void SiStripRawToClustersDummyRoI::event(const uint32_t& event, 
					 SiStripRegionCabling::Regions& regions) const {
  
  uint32_t total = cabling_->getRegionCabling().size();
  uint32_t required = (uint32_t)(RandFlat::shoot()*(total+1));
  for (uint32_t iregion = 0; iregion < required; iregion++) {
	regions.push_back(iregion);
  }
}

void SiStripRawToClustersDummyRoI::superclusters(const reco::SuperClusterCollection& coll, 
						 SiStripRegionCabling::Regions& regions) const {

  reco::SuperClusterCollection::const_iterator iclust = coll.begin();
  for (;iclust!=coll.end();iclust++) {
  SiStripRegionCabling::Position position(iclust->seed()->position().eta(),
					  iclust->seed()->position().phi());
  SiStripRegionCabling::Regions newregions = cabling_->regions(position,0.5);
  std::copy(newregions.begin(),newregions.end(),std::back_inserter(regions));
  }
}
