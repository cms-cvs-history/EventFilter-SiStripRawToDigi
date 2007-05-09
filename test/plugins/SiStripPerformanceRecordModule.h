#ifndef EventFilter_SiStripRawToDigi_SiStripPerformanceRecordModule_H
#define EventFilter_SiStripRawToDigi_SiStripPerformanceRecordModule_H

// FWCore
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Data Formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripCommon/interface/SiStripRefGetter.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h" 

//CalibFormats
#include "CalibFormats/SiStripObjects/interface/SiStripRegionCabling.h"

// Root
#include "TFile.h"
#include "TTree.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/EventData.h"
#include <string>
#include <vector>

// stl
#include <iostream>

class SiStripPerformanceRecordModule : public edm::EDAnalyzer {

  public:

  typedef edm::DetSet<SiStripCluster> DetSet;
  typedef std::vector< edm::DetSet<SiStripCluster> > DSV;
  typedef edm::SiStripRefGetter< SiStripCluster, edm::SiStripLazyGetter<SiStripCluster> > RefGetter;

  SiStripPerformanceRecordModule(const edm::ParameterSet&);
  ~SiStripPerformanceRecordModule();

  void beginJob( const edm::EventSetup& );
  void endJob();
  void analyze( const edm::Event&, const edm::EventSetup& );

 private:

  void timer(uint32_t event);
  void electrons(const edm::Handle<reco::PixelMatchGsfElectronCollection>&);
  void sclusters(const edm::Handle<reco::SuperClusterCollection>&);
  void sistripclusters(const edm::Handle< RefGetter >&);
  void sistripclusters(const edm::Handle< DSV >&);
  void sistripchannels(const edm::Handle< RefGetter >&);
  void sistripchannels();
  void reset();

  //Configurations
  bool demand_;
  std::vector< std::string > unpackingModuleLabels_;
  std::string filename_;
  std::string treename_;

  //Cabling
  edm::ESHandle<SiStripRegionCabling> cabling_;

  //Debug
  double meantime_;

  //Performance record
  TFile* file_;
  TTree* tree_;
  EventData* data_;
  unsigned int event_;
  double time_;
  unsigned int nchans_;
  unsigned int nunpackedchans_;
};

#endif
