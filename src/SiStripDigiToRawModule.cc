#include "EventFilter/SiStripRawToDigi/interface/SiStripDigiToRawModule.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripDigiToRaw.h"
// handle to EventSetup
#include "FWCore/Framework/interface/ESHandle.h"
// data collections
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/SiStripDigi/interface/StripDigiCollection.h"
#include "DataFormats/SiStripDigi/interface/StripDigi.h"
// geometry 
//#include "Geometry/TrackerSimAlgo/interface/CmsDigiTracker.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
// utility class
#include "EventFilter/SiStripRawToDigi/interface/SiStripUtility.h"
// connectivity
#include "CalibTracker/SiStripConnectivity/interface/SiStripConnection.h"
// #include "Geometry/Records/interface/TrackerConnectionRecord.h"
//
#include <cstdlib>

// -----------------------------------------------------------------------------
// constructor
SiStripDigiToRawModule::SiStripDigiToRawModule( const edm::ParameterSet& conf ) :
  digiToRaw_(0),
  utility_(0),
  event_(0),
  fedReadoutMode_( conf.getParameter<std::string>("FedReadoutMode") ),
  fedReadoutPath_( conf.getParameter<std::string>("FedReadoutPath") ),
  verbosity_( conf.getParameter<int>("Verbosity") )
{
  if (verbosity_>1) std::cout << "[SiStripDigiToRawModule::SiStripDigiToRawModule] "
			      << "Constructing DigiToRaw module..." << std::endl;
  // specify product type
  produces<FEDRawDataCollection>();

}

// -----------------------------------------------------------------------------
// destructor
SiStripDigiToRawModule::~SiStripDigiToRawModule() {
  if (verbosity_>1) std::cout << "[SiStripDigiToRawModule::~SiStripDigiToRawModule] "
			      << "Destructing DigiToRaw module..." << std::endl;
  if ( digiToRaw_ ) delete digiToRaw_;
  if ( utility_ ) delete utility_; 

}

// -----------------------------------------------------------------------------
//
void SiStripDigiToRawModule::beginJob( const edm::EventSetup& iSetup ) {
  if (verbosity_>2) std::cout << "[SiStripDigiToRawModule::beginJob] "
			      << "creating utility object, connections map, "
			      << "DigiToRaw converter..." << std::endl;

  //@@ cannot presently retrieve connections map from EventSetup !
  
  //   // retrieve cabling map (ESProduct) 
  //   ESHandle<SiStripConnection> connections;
  //   iSetup.get<TrackerConnectionRecord>().get( connections );
  //   cabling_.reset( connections.product() );

  // create utility object and retrieve "dummy" connections map
  utility_ = new SiStripUtility( iSetup );
  if (verbosity_>1) utility_->verbose(true);
  SiStripConnection connections;
  utility_->siStripConnection( connections );
  
  // create instance of DigiToRaw converter
  digiToRaw_ = new SiStripDigiToRaw( connections, verbosity_ );
  digiToRaw_->fedReadoutPath( fedReadoutPath_ );
  digiToRaw_->fedReadoutMode( fedReadoutMode_ );

}

// -----------------------------------------------------------------------------
//
void SiStripDigiToRawModule::endJob() { 
  if (verbosity_>2) std::cout << "[SiStripDigiToRawModule::endJob] "
			      << "currently does nothing..." << std::endl;
}

// -----------------------------------------------------------------------------
// produces a FEDRawDataCollection
void SiStripDigiToRawModule::produce( edm::Event& iEvent, 
				      const edm::EventSetup& iSetup ) {
  if (verbosity_>2) std::cout << "[SiStripDigiToRawModule::produce] "
			      << "input: \"dummy\" FedRawDataCollection, "
			      << "output: StripDigiCollection" << std::endl;
  
  event_++; // increment event counter

  if (verbosity_>0) std::cout << "[SiStripDigiToRawModule::produce] "
			      << "event number: " << event_ << std::endl;
  
  //@@ cannot retrieve digis from Event based on simulated events!
  
  //   // retrieve collection of StripDigi's from Event
  //   edm::Handle<StripDigiCollection> handle;
  //   //iEvent.getByLabel( "write_digis", handle );
  //   iEvent.getByLabel( "RawToDigi", handle );
  //   StripDigiCollection digis = const_cast<StripDigiCollection&>( *handle );

  // retrieve "dummy" collection of StripDigi from utility object
  StripDigiCollection digis;
  numDigis = utility_->stripDigiCollection( digis );
  
  // create product
  std::auto_ptr<FEDRawDataCollection> fed_buffers( new FEDRawDataCollection );

  // use DigiToRaw converter to fill FEDRawDataCollection
  digiToRaw_->createFedBuffers( digis, *(fed_buffers.get()) );

  // write FEDRawDataCollection to the Event
  iEvent.put( fed_buffers );

}
