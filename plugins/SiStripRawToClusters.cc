#include "EventFilter/SiStripRawToDigi/plugins/SiStripRawToClusters.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------

SiStripRawToClusters::SiStripRawToClusters( const edm::ParameterSet& conf ) :

  allregions_(),
  productLabel_(conf.getUntrackedParameter<string>("ProductLabel","source")),
  productInstance_(conf.getUntrackedParameter<string>("ProductInstance","")),
  cabling_(),
  clusterizer_(0)
  
{
  LogTrace(mlRawToDigi_)
    << "[SiStripRawToClusters::" 
    << __func__ 
    << "]"
    << " Constructing object...";
  
  clusterizer_ = new SiStripClusterizerFactory(conf);
  produces< LazyGetter >();
  produces< RefGetter >();
}

// -----------------------------------------------------------------------------

SiStripRawToClusters::~SiStripRawToClusters() {

  if (clusterizer_) delete clusterizer_;

  LogTrace(mlRawToDigi_)
    << "[SiStripRawToClusters::" 
    << __func__ 
    << "]"
    << " Destructing object...";
}

// -----------------------------------------------------------------------------

void SiStripRawToClusters::beginJob( const edm::EventSetup& setup) {

  setup.get<SiStripRegionCablingRcd>().get(cabling_);
  clusterizer_->eventSetup(setup);

  allregions_.reserve(cabling_->getRegionCabling().size());
  for (uint32_t iregion=0;iregion<cabling_->getRegionCabling().size();iregion++) {
    for (uint32_t isubdet=0;isubdet<cabling_->getRegionCabling()[iregion].size();isubdet++) {
      for (uint32_t ilayer=0;ilayer<cabling_->getRegionCabling()[iregion][isubdet].size();ilayer++) {
	uint32_t index = SiStripRegionCabling::elementIndex(iregion,static_cast<SubDet>(isubdet),ilayer);
	allregions_.push_back(index);
      }
    }
  }
}

// -----------------------------------------------------------------------------

void SiStripRawToClusters::endJob() {;}

// -----------------------------------------------------------------------------

void SiStripRawToClusters::produce( edm::Event& event, 
				    const edm::EventSetup& setup ) {


  //Retrieve FED raw data
  edm::Handle<FEDRawDataCollection> buffers;
  event.getByLabel( productLabel_, productInstance_, buffers ); 
  
  //Construct.
  boost::shared_ptr<LazyUnpacker> unpacker(new LazyUnpacker(*cabling_,*clusterizer_,*buffers));

  //Store SiStripLazyGetter in event.
  std::auto_ptr<LazyGetter> collection(new LazyGetter(unpacker));
  edm::OrphanHandle<LazyGetter> pcollection = event.put(collection);

  //Store SiStripRefGetter for global unpacking in event. 
  std::auto_ptr<RefGetter> fullcollection(new RefGetter(pcollection,allregions_));
  event.put(fullcollection);
}

