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
//#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "DQM/HLTEvF/interface/PathTimerService.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/ObjectConverter.h"

class SiStripPerformance : public edm::EDAnalyzer {

  public:

  typedef SiStripRegionCabling::Cabling Cabling;
  typedef SiStripRegionCabling::ElementCabling ElementCabling;
  typedef edm::SiStripRefGetter<SiStripCluster> RefGetter;

  SiStripPerformance(const edm::ParameterSet&);
  ~SiStripPerformance();

  void beginJob( const edm::EventSetup& );
  void endJob();
  void analyze( const edm::Event&, const edm::EventSetup& );

 private:

  /** Timers */
  static const double moduletimer(HLTPerformanceInfo&, const std::vector<std::string>&);
  static const double pathtimer(HLTPerformanceInfo&, const std::vector<std::string>&);

  /** Channels */
  static const uint32_t channels(const SiStripRegionCabling&);
  static const uint32_t channels(const SiStripRegionCabling&, const RefGetter&);

  /** Event record methods */
  void particles(const edm::Handle<edm::HepMCProduct>&);
  void jets(const edm::Handle< std::vector<reco::GenJet> >&);
  void sistripdigis(const edm::Handle< edm::DetSetVector<SiStripDigi> >&);
  void sistripdigis(const edm::Handle< edm::DetSetVector<SiStripDigi> >&, const edm::Handle< RefGetter >&);
  void sistripclusters(const edm::Handle<RefGetter>&);
  void sistripclusters(const edm::Handle< edm::DetSetVector<SiStripCluster> >&);
  void electrons(); // const edm::Handle<reco::HLTFilterObjectWithRefs>&); 
  void muons(); // const edm::Handle<reco::HLTFilterObjectWithRefs>&); 
  void jets(); // const edm::Handle<reco::HLTFilterObjectWithRefs>&); 
  void trigger(const edm::Handle<edm::TriggerResults>&);

  /** Input tags */
  edm::InputTag particles_;
  edm::InputTag genJets_;
  edm::InputTag sistripDigis_;
  edm::InputTag sistripClusters_;
  edm::InputTag electronFilter_;
  edm::InputTag muonFilter_;
  edm::InputTag taujetFilter_;
  edm::InputTag bjetFilter_;

  /** Timing */
  std::vector< std::string > timingmodules_;
  std::vector< std::string > timingpaths_;

  /** Setup */
  edm::ESHandle<SiStripRegionCabling> cabling_;	  
  edm::ESHandle<ParticleDataTable> pdt_;
 
  /** Record */
  TFile* file_;
  TTree* tree_;
  SimpleEventData* data_;
  double time_;
  unsigned int nchans_;
  unsigned int nunpackedchans_;
};

#endif
