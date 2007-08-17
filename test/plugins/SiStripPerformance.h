#ifndef EventFilter_SiStripRawToDigi_SiStripPerformance_H
#define EventFilter_SiStripRawToDigi_SiStripPerformance_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CalibFormats/SiStripObjects/interface/SiStripRegionCabling.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripCommon/interface/SiStripRefGetter.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DQM/HLTEvF/interface/PathTimerService.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/ObjectConverter.h"

class SiStripPerformance : public edm::EDAnalyzer {

  public:

  SiStripPerformance(const edm::ParameterSet&);
  ~SiStripPerformance();

  void beginJob( const edm::EventSetup& );
  void endJob();
  void analyze( const edm::Event&, const edm::EventSetup& );

 private:

  //Cabling
  void allchannels();
  void regionalchannels(const edm::Handle< edm::SiStripRefGetter<SiStripCluster> >&);

  //Event
  void timer(const HLTPerformanceInfo&);
  void particles(const edm::Handle<edm::HepMCProduct>&);
  void sistripdigis(const edm::Handle< edm::DetSetVector<SiStripDigi> >&);
  void sistripclusters(const edm::Handle< edm::SiStripRefGetter<SiStripCluster> >&);
  void sistripclusters(const edm::Handle< edm::DetSetVector<SiStripCluster> >&);
  void electrons(const edm::Handle<reco::HLTFilterObjectWithRefs>&);
  void taus(const edm::Handle<IsolatedTauTagInfoCollection>&);
  void trigger(const edm::Handle<edm::TriggerResults>&);

  //Configurations
  std::string sistripDigisModuleLabel_;
  std::string sistripDigisProductLabel_;
  std::string sistripClustersModuleLabel_;
  std::string sistripClustersProductLabel_;
  std::string mcModuleLabel_;
  std::string mcProductLabel_;
  std::string electronsModuleLabel_;
  std::string electronsProductLabel_;
  std::string tausModuleLabel_;
  std::string tausProductLabel_;
  std::vector< std::string > unpackingModuleLabels_;

  //Setup
  edm::ESHandle<SiStripRegionCabling> cabling_;	  
  edm::ESHandle<ParticleDataTable> pdt_;
 
  //Event record
  TFile* file_;
  TTree* tree_;
  SimpleEventData* data_;
  double time_;
  unsigned int nchans_;
  unsigned int nunpackedchans_;
};

#endif
