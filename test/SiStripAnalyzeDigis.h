#ifndef EventFilter_SiStripRawToDigi_test_SiStripAnalyzeDigis_H
#define EventFilter_SiStripRawToDigi_test_SiStripAnalyzeDigis_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include <boost/cstdint.hpp>
#include <string>
#include <vector>
#include <map>

/**
   @class SiStripAnalyzeDigis
*/
class SiStripAnalyzeDigis : public edm::EDAnalyzer {

 public:
  
  SiStripAnalyzeDigis( const edm::ParameterSet& );
  ~SiStripAnalyzeDigis();
  
  /** Simple container class. */
  class TrivialAnalysis {
  public:
    TrivialAnalysis() : 
      events_(0), feds_(0), channels_(0), strips_(0),
      digis_(0), pos_(1024,0), adc_(1024,0) {;}
    uint32_t events_;
    uint32_t feds_;
    uint32_t channels_;
    uint32_t strips_;
    uint32_t digis_;
    std::vector<uint16_t> pos_;
    std::vector<uint16_t> adc_;
  };
  
  void beginJob( edm::EventSetup const& );
  void analyze( const edm::Event&, const edm::EventSetup& );
  void endJob();
  
 private:

  std::string inputModuleLabel_;
  TrivialAnalysis anal_;

};

#endif // EventFilter_SiStripRawToDigi_test_SiStripAnalyzeDigis_H

