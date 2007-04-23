#ifndef EventFilter_SiStripRawToDigi_SiStripRawToClustersLazyUnpacker_H
#define EventFilter_SiStripRawToDigi_SiStripRawToClustersLazyUnpacker_H

//FWCore
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//Data Formats
#include "DataFormats/SiStripCommon/test/SiStripLazyGetter.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"

//Calib Formats
#include "CalibTracker/SiStripConnectivity/test/SiStripRegionCabling.h"

//Common Tools
#include "CommonTools/SiStripClusterization/interface/SiStripClusterizerFactory.h"

//EventFilter
#include "EventFilter/SiStripRawToDigi/interface/SiStripRawToDigiUnpacker.h"

//Fed9U
#include "Fed9UUtils.hh"

//stl
#include <vector>

class SiStripRawToClustersLazyUnpacker : public edm::SiStripLazyUnpacker<SiStripCluster> {

 public:

  typedef edm::DetSet<SiStripCluster> DetSet;

  SiStripRawToClustersLazyUnpacker(const SiStripRegionCabling&,
				   const SiStripClusterizerFactory&,
				   const FEDRawDataCollection&); 
  
  ~SiStripRawToClustersLazyUnpacker();

  virtual void fill(uint32_t&); 

 private:

  SiStripRawToClustersLazyUnpacker();

  //Raw data
  const FEDRawDataCollection* raw_;

  //Cabling
  const SiStripRegionCabling::RegionCabling* regions_;

  //Clusterizer Factory
  const SiStripClusterizerFactory* clusterizer_;

  //Fed9UEvent cache
  std::vector< Fed9U::Fed9UEvent* > fedEvents_;

  //RawToDigi
  SiStripRawToDigiUnpacker rawToDigi_;

};

#endif //  EventFilter_SiStripRawToDigi_SiStripRawToClustersLazyUnpacker_H
