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
#include "DataFormats/SiStripCommon/test/SiStripLazyGetter.h"
#include "DataFormats/SiStripCommon/test/SiStripRefGetter.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"

//Calib Formats
#include "CalibTracker/SiStripConnectivity/test/SiStripRegionCabling.h"

// Root
#include "TFile.h"
#include "TTree.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSiStripCollection.h"
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

  void clusters(const RefGetter&);
  void clusters(const DSV&);
  void convert(const SiStripCluster&,SimpleSiStripCluster&);
  void reset();

  //Configurations
  bool demand_;
  std::string inputModuleLabel_;
  std::string inputProductLabel_;
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
  SimpleSiStripCollection* clusts_;
  unsigned int event_;
  double time_;
  unsigned int nchans_;
  unsigned int nunpackedchans_;
};

#endif
