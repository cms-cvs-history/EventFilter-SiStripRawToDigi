#ifndef EventFilter_SiStripRawToDigi_SiStripRawToClusters_H
#define EventFilter_SiStripRawToDigi_SiStripRawToClusters_H

#include "EventFilter/SiStripRawToDigi/test/stubs/SiStripRawToClustersLazyUnpacker.h"

//FWCore
#include "FWCore/Framework/interface/EDProducer.h"

//Data Formats
#include "DataFormats/SiStripCommon/test/SiStripLazyGetter.h"
#include "DataFormats/SiStripCommon/test/SiStripRefGetter.h"

//Calib Tracker
#include "CalibTracker/SiStripConnectivity/test/SiStripRegionCabling.h"
#include "CalibTracker/SiStripConnectivity/test/SiStripRegionCablingRcd.h"

//stl
#include <string>
#include <memory>
#include "boost/bind.hpp"

/**
   @file EventFilter/SiStripRawToDigi/interface/SiStripRawToClusters.h
   @class SiStripRawToClusters
*/

class SiStripRawToClusters : public edm::EDProducer {
  
 public:

  typedef edm::SiStripRefGetter< SiStripCluster, edm::SiStripLazyGetter<SiStripCluster> > RefGetter;
  SiStripRawToClusters( const edm::ParameterSet& );
  ~SiStripRawToClusters();
  
  virtual void beginJob( const edm::EventSetup& );
  virtual void endJob();
  virtual void produce( edm::Event&, const edm::EventSetup& );
  
 private: 

  //Record of all region numbers
  std::vector<uint32_t> allregions_;

  //Raw data labels
  std::string productLabel_;
  std::string productInstance_;

  //Cabling
  edm::ESHandle<SiStripRegionCabling> cabling_;

  //Clusterizer Factory
  SiStripClusterizerFactory* clusterizer_;

  //Fed
  int16_t dumpFrequency_;
  int16_t triggerFedId_;
};

#endif //  EventFilter_SiStripRawToDigi_SiStripRawToClusters_H

