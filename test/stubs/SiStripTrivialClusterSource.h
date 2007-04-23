#ifndef EventFilter_SiStripRawToDigi_SiStripTrivialClusterSource_H
#define EventFilter_SiStripRawToDigi_SiStripTrivialClusterSource_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "boost/cstdint.hpp"
#include <algorithm>
#include <utility>
#include <vector>
#include <map>

class FedChannelConnection;

 /**
    @file EventFilter/SiStripRawToDigi/test/stubs/SiStripTrivialClusterSource.h
    @class SiStripTrivialClusterSource

    @brief Creates a DetSetVector of SiStripDigis created using random
    number generators and attaches the collection to the Event. Allows
    to test the final DigiToRaw and RawToDigi/RawToCluster converters.  
*/
class SiStripTrivialClusterSource : public edm::EDProducer {
  
 public:
  
  SiStripTrivialClusterSource( const edm::ParameterSet& );
  ~SiStripTrivialClusterSource();
  
  virtual void beginJob( const edm::EventSetup& );
  virtual void endJob();
  virtual void produce( edm::Event&, const edm::EventSetup& );
  
 private: 

  uint32_t randflat(uint32_t,uint32_t);

  //Configurables
  double minOcc_;
  double maxOcc_;
  uint16_t minCluster_;
  uint16_t maxCluster_;
  bool mixClusters_;
  uint16_t separation_;
  bool maxAdc_;

  //Cabling
  SiStripDetCabling* cabling_;
  std::map<uint32_t, std::vector<FedChannelConnection> > detCabling_;
  std::vector<uint32_t> detids_;
  uint32_t nstrips_;
 
};

class CandidateCluster {

 public:

  CandidateCluster(uint16_t first,uint16_t size, uint16_t separation) :
    first_(first),
    size_(size)
    {;}

  inline bool operator()(const SiStripDigi& digi) {

    return (digi.strip() >= (first_-separation_) &&
	    digi.strip() < first_+size_+separation_);
  }

 private:

  uint16_t first_;
  uint16_t size_;
  uint16_t separation_;
};

#endif // EventFilter_SiStripRawToDigi_SiStripTrivialClusterSource_H
