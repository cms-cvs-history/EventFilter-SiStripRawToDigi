#include "EventFilter/SiStripRawToDigi/interface/SiStripDigiToRawModule.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripDigiToRaw.h"
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/SiStripDigi/interface/StripDigiCollection.h"
#include "CondFormats/DataRecord/interface/SiStripReadoutCablingRcd.h"
#include "CondFormats/SiStripObjects/interface/SiStripReadoutCabling.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripRawToDigi.h"
#include <cstdlib>

// -----------------------------------------------------------------------------
// Constructor
SiStripDigiToRawModule::SiStripDigiToRawModule( const edm::ParameterSet& pset ) :
  digiToRaw_(0),
  eventCounter_(0),
  productLabel_(""),
  verbosity_(0)
{
  std::cout << "[SiStripDigiToRawModule::SiStripDigiToRawModule]"
	    << " Constructing object..." << std::endl;
  
  // Set some private data members 
  productLabel_ = pset.getParameter<std::string>("EDProductLabel");
  verbosity_    = pset.getParameter<int>("Verbosity");
  
  // Create instance of DigiToRaw formatter
  digiToRaw_ = new SiStripDigiToRaw( verbosity_ );
  digiToRaw_->fedReadoutMode( pset.getParameter<std::string>("FedReadoutMode") );
  digiToRaw_->fedReadoutPath( pset.getParameter<std::string>("FedReadoutPath") );

  // Define EDProduct type
  produces<FEDRawDataCollection>();

}

// -----------------------------------------------------------------------------
// Destructor
SiStripDigiToRawModule::~SiStripDigiToRawModule() {
  std::cout << "[SiStripDigiToRawModule::~SiStripDigiToRawModule]"
	    << " Destructing object..." << std::endl;
  if ( digiToRaw_ ) delete digiToRaw_;
}

// -----------------------------------------------------------------------------
// Produces a FEDRawDataCollection
void SiStripDigiToRawModule::produce( edm::Event& iEvent, 
				      const edm::EventSetup& iSetup ) {

  // Some debug
  eventCounter_++; 
  if (verbosity_>0) std::cout << "[SiStripDigiToRawModule::produce] "
 			      << "event number: " 
			      << eventCounter_ << std::endl;
  
  // Retrieve readout cabling map from EventSetup
  edm::ESHandle<SiStripReadoutCabling> cabling;
  iSetup.get<SiStripReadoutCablingRcd>().get( cabling );
  
  // Retrieve input data from Event, a StripDigiCollection
  edm::Handle<StripDigiCollection> digis;
  //iEvent.getByLabel( productLabel_, digis );
  iEvent.getByType( digis );

  // Create EDProduct, a FEDRawDataCollection
  std::auto_ptr<FEDRawDataCollection> buffers( new FEDRawDataCollection );

  // Use DigiToRaw formatter to fill FEDRawDataCollection
  digiToRaw_->createFedBuffers( cabling, digis, buffers );

    // Attach FEDRawDataCollection to Event
  //std::cout << "before" << std::endl;
  iEvent.put( buffers );
  //std::cout << "after" << std::endl;

}
