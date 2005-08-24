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
#include "Geometry/TrackerSimAlgo/interface/CmsDigiTracker.h"
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
  fedReadoutPath_( conf.getParameter<string>("FedReadoutPath") ),
  verbosity_( conf.getParameter<int>("Verbosity") )
{
  if (verbosity_>1) std::cout << "[SiStripDigiToRawModule] "
			      << "constructing DigiToRaw module..." << endl;
  // specify product type
  produces<raw::FEDRawDataCollection>("FEDRawDataCollection_from_StripDigis");
}

// -----------------------------------------------------------------------------
// destructor
SiStripDigiToRawModule::~SiStripDigiToRawModule() {
  if (verbosity_>1) std::cout << "[SiStripDigiToRawModule] "
			      << "destructing DigiToRaw module..." << endl;
  if ( digiToRaw_ ) delete digiToRaw_;
  if ( utility_ ) delete utility_; 
}

// -----------------------------------------------------------------------------
//
void SiStripDigiToRawModule::beginJob( const edm::EventSetup& iSetup ) {
  if (verbosity_>2) std::cout << "[SiStripDigiToRawModule::beginJob] "
			      << "creating utility object, connections map, DigiToRaw converter..." << endl;

  //@@ cannot presently retrieve connections map from EventSetup!
  //   // retrieve cabling map (ESProduct) 
  //   ESHandle<SiStripConnection> connections;
  //   iSetup.get<TrackerConnectionRecord>().get( connections );
  //   cabling_.reset( connections.product() );
  
  // retrieve "dummy" connections map from utility object 
  utility_ = new SiStripUtility( iSetup );
  SiStripConnection connections;
  utility_->siStripConnection( connections );
  
  // some debug:
  vector<unsigned short> feds;
  map<unsigned short, cms::DetId> partitions;
  connections.getConnectedFedNumbers( feds );
  connections.getDetPartitions( partitions );
  std::cout << "number of feds: " << feds.size() 
	    << ", number of partitions: " << partitions.size() << endl;

  // create instance of DigiToRaw converter
  digiToRaw_ = new SiStripDigiToRaw( connections );
  // digiToRaw_->fedReadoutMode( fedReadoutMode_ );
  // digiToRaw_->fedReadoutPath( fedReadoutPath_ );

}

// -----------------------------------------------------------------------------
//
void SiStripDigiToRawModule::endJob() { 
  if (verbosity_>2) std::cout << "[SiStripDigiToRawModule::endJob] "
			      << "cuurently does nothing..." << endl;
}

// -----------------------------------------------------------------------------
// produces a FEDRawDataCollection
void SiStripDigiToRawModule::produce( edm::Event& iEvent, 
				      const edm::EventSetup& iSetup ) {
  if (verbosity_>2) std::cout << "[SiStripDigiToRawModule::produce] "
			      << "creates \"dummy\" StripDigiCollection as " << endl
			      << "input to DigiToRaw converter and writes "
			      << "\"FedRawDataCollection\" product to Event ..." << endl;
  
  event_++; // increment event counter
  if (verbosity_>0) std::cout << "[SiStripDigiToRawModule::produce] "
			      << "event number: " << event_ << endl;
  
  //   // retrieve collection of StripDigi's from Event
  //   edm::Handle<StripDigiCollection> input;
  //   e.getByLabel("StripDigiConverter", input);
  
  // retrieve "dummy" collection of StripDigi from utility object
  StripDigiCollection digis;
  utility_->stripDigiCollection( digis );
  
//   // some debug:
//   std::vector<unsigned int> dets = digis.detIDs();
//   for ( unsigned int idet = 0; idet < dets.size(); idet++ ) {
//     const StripDigiCollection::Range digi_range = dets.get( idet ); 
//     StripDigiCollection::ContainerIterator idigi;
//     int ndigi = 0;
//     for ( idigi = digi_range.first; idigi != digi_range.second; idigi++ ) { ndigi++; }
//     if ( !ndigi ) { std::cout << "DET " << idet << " has zero digis!" << endl; }
//     else { std::cout << "DET " << idet << " has " << ndigi << " digis" << endl; }
//   }
  
  // create product
  raw::FEDRawDataCollection fed_buffers;

  // use DigiToRaw converter to fill FEDRawDataCollection
  digiToRaw_->createFedBuffers( digis, fed_buffers );

  // write FEDRawDataCollection to the Event
  // iEvent.put( &fed_buffers );

}

// -----------------------------------------------------------------------------
// define the class SiStripDigiToRawModule as a plug-in
//DEFINE_FWK_MODULE(SiStripDigiToRawModule)