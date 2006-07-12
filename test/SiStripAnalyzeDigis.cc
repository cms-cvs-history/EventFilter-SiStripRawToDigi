#include "EventFilter/SiStripRawToDigi/test/SiStripAnalyzeDigis.h"
//
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//
#include "CondFormats/DataRecord/interface/SiStripFedCablingRcd.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CalibFormats/SiStripObjects/interface/SiStripFecCabling.h"
//
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigis.h"
#include "DataFormats/SiStripDigi/interface/SiStripEventSummary.h"
//
#include <boost/cstdint.hpp>
#include <sstream>
#include <iomanip>

using namespace std;

// -----------------------------------------------------------------------------
//
SiStripAnalyzeDigis::SiStripAnalyzeDigis( const edm::ParameterSet& pset ) :
  inputModuleLabel_( pset.getParameter<string>( "InputModuleLabel" ) )
  //fedReadoutMode_( pset.getUntrackedParameter<string>("CommissioningTask","UNDEFINED") ),
{
  edm::LogVerbatim("SiStripAnalyzeDigis")
    << "[SiStripAnalyzeDigis::SiStripAnalyzeDigis]"
    << " Constructing object...";
}

// -----------------------------------------------------------------------------
//
SiStripAnalyzeDigis::~SiStripAnalyzeDigis() {
  edm::LogVerbatim("SiStripAnalyzeDigis")
    << "[SiStripAnalyzeDigis::~SiStripAnalyzeDigis]"
    << " Destructing object...";
}

// -----------------------------------------------------------------------------
// 
void SiStripAnalyzeDigis::beginJob( const edm::EventSetup& setup ) {
  edm::LogVerbatim("SiStripAnalyzeDigis")
    << "[SiStripAnalyzeDigis::beginJob]";
}

// -----------------------------------------------------------------------------
// 
void SiStripAnalyzeDigis::endJob() {
  stringstream ss;
  ss << "[SiStripAnalyzeDigis::endJob]"
     << " TrivialAnalysis results:"
     << " events: " << anal_.events_
     << " feds: " << anal_.feds_
     << " channels: " << anal_.channels_
     << " strips: " << anal_.strips_
     << " digis: " << anal_.digis_;
  ss << "\n pos : ";
  for ( uint16_t ii = 0; ii < 1024; ii+=64 ) { ss << setw(4) << ii << " "; }
  ss << "\n freq: ";
  for ( uint16_t ii = 0; ii < 1024; ii+=64 ) { ss << setw(4) << anal_.pos_[ii] << " "; }
  ss << "\n adc : ";
  for ( uint16_t ii = 0; ii < 1024; ii+=64 ) { ss << setw(4) << ii << " "; }
  ss << "\n freq: ";
  for ( uint16_t ii = 0; ii < 1024; ii+=64 ) { ss << setw(4) << anal_.adc_[ii] << " "; }
  edm::LogVerbatim("SiStripAnalyzeDigis") << ss.str();
}

// -----------------------------------------------------------------------------
//
void SiStripAnalyzeDigis::analyze( const edm::Event& event, 
				   const edm::EventSetup& setup ) {
  edm::LogVerbatim("SiStripAnalyzeDigis")
    << "[SiStripAnalyzeDigis::analyze]"
    << " Analyzing run " << event.id().run() 
    << " and event " << event.id().event();

  edm::Handle<SiStripEventSummary> summary;
  event.getByLabel( inputModuleLabel_, summary );
  cout << "ptr: " << &(*summary) << endl;

//   // Retrieve digis ojbect(s) 
//   edm::Handle< SiStripDigis > digis;
//   //event.getByLabel( inputModuleLabel_, "StripDigis", digis );
//   event.getByLabel( inputModuleLabel_, digis );
  
//   if ( &(*digis) == 0 ) {
//     edm::LogError("SiStripAnalyzeDigis")
//       << "[SiStripAnalyzeDigis::analyze]"
//       << " NULL pointer to SiStripDigis!"
//       << " (run " << event.id().run() 
//       << " and event " << event.id().event() << ")";
//     return;
//   }

  // Retrieve FED (reatout) and FEC (control) cabling
  edm::ESHandle<SiStripFedCabling> fed_cabling;
  setup.get<SiStripFedCablingRcd>().get( fed_cabling ); 
  //SiStripFecCabling* fec_cabling = new SiStripFecCabling( *fed_cabling );

  anal_.events_++;
  vector<uint16_t>::const_iterator ifed = fed_cabling->feds().begin();
  for ( ; ifed != fed_cabling->feds().end(); ifed++ ) {
    anal_.feds_++;
    for ( uint16_t ichan = 0; ichan < sistrip::FEDCH_PER_FED; ichan++ ) { 
      anal_.channels_++;
      for ( uint16_t istrip = 0; istrip < sistrip::STRIPS_PER_FEDCH; ichan++ ) { 
	anal_.strips_++;
	uint16_t adc = 0;//digis->adc( *ifed, ichan, istrip );
	if ( adc != SiStripDigis::invalid_ ) {
	  anal_.digis_++;
	  anal_.pos_[istrip]++;
	  anal_.adc_[adc]++;
	}
      }
    }
  }
  
}
