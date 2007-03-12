#ifndef EventFilter_SiStripRawToDigi_test_stubs_SiStripPerformanceRecordModule_H
#define EventFilter_SiStripRawToDigi_test_stubs_SiStripPerformanceRecordModule_H

// edm
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// cabling
#include "CondFormats/DataRecord/interface/SiStripFedCablingRcd.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"

//root
#include "TFile.h"
#include "TTree.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSiStripCollection.h"
#include <string>
#include <vector>

//std
#include <iostream>

class SiStripPerformanceRecordModule : public edm::EDAnalyzer {

  public:

  SiStripPerformanceRecordModule(const edm::ParameterSet&);
  ~SiStripPerformanceRecordModule();

  void beginJob( const edm::EventSetup& );
  void endJob();
  void analyze( const edm::Event&, const edm::EventSetup& );

 private:

  void reset();
  void print();

  //Configurations
  std::string inputModuleLabel_;
  std::string inputProductLabel_;
  std::vector< std::string > unpackingModuleLabels_;
  bool verbose_;
  std::string filename_;
  std::string treename_;

  //Cabling
  edm::ESHandle<SiStripFedCabling> fedcabling_;

  //Debug
  double meantime_;

  //Performance record
  TFile* file_;
  TTree* tree_;
  SimpleSiStripCollection* clusts_;
  unsigned int event_;
  double time_;
  unsigned int nchannels_;
  unsigned int nunpacked_;
};

#endif
