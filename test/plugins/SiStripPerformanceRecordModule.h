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
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h" 
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

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

  void timer(uint32_t);
  void mc(const edm::Handle<edm::HepMCProduct>&);
  void electrons(const edm::Handle<reco::ElectronCollection>&);
  void sistripclusters(const edm::Handle< RefGetter >&);
  void sistripclusters(const edm::Handle< DSV >&);
  void sistripchannels(const edm::Handle< RefGetter >&);
  void sistripchannels();
  void reset();

  //Configurations
  bool sistripDemand_;
  std::string sistripClustersModuleLabel_;
  std::string sistripClustersProductLabel_;
  bool recordMc_;
  std::string mcModuleLabel_;
  std::string mcProductLabel_;
  bool recordElectrons_;
  std::string electronsModuleLabel_;
  std::string electronsProductLabel_;
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
