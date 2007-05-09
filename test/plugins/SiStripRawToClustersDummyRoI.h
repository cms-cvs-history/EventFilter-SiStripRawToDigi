#ifndef EventFilter_SiStripRawToDigi_SiStripRawToClustersDummyRoI_H
#define EventFilter_SiStripRawToDigi_SiStripRawToClustersDummyRoI_H

//FWCore
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//Data Formats
#include "DataFormats/SiStripCommon/interface/SiStripRefGetter.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

//CalibFormats
#include "CalibFormats/SiStripObjects/interface/SiStripRegionCabling.h"

//stl
#include <string>
#include <memory>
#include "boost/bind.hpp"

/**
   @file EventFilter/SiStripRawToDigi/interface/SiStripRawToClustersDummyRoI.h
   @class SiStripRawToClustersDummyRoI
*/

class SiStripRawToClustersDummyRoI : public edm::EDProducer {
  
 public:

  typedef edm::SiStripRefGetter< SiStripCluster, edm::SiStripLazyGetter<SiStripCluster> > RefGetter;

  SiStripRawToClustersDummyRoI( const edm::ParameterSet& );
  ~SiStripRawToClustersDummyRoI();
  
  virtual void beginJob( const edm::EventSetup& );
  virtual void endJob();
  virtual void produce( edm::Event&, const edm::EventSetup& );
  
 private: 

  /** Method defining regions of interest by event number */
  void event(const uint32_t&,
	     SiStripRegionCabling::Regions&) const;
  
  /** Method defining regions of interest by superclusters */
  void superclusters(const reco::SuperClusterCollection&,
		     SiStripRegionCabling::Regions&) const;

  /** Input label */
  std::string inputModuleLabel_;

  /** Cabling */
  edm::ESHandle<SiStripRegionCabling> cabling_;

  /** Configurable to vary dummy regions of interest with event number.*/
  uint32_t period_;

};

#endif //  EventFilter_SiStripRawToDigi_SiStripRawToClustersDummyRoI_H

