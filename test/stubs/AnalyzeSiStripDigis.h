#ifndef EventFilter_SiStripRawToDigi_test_AnalyzeSiStripDigis_H
#define EventFilter_SiStripRawToDigi_test_AnalyzeSiStripDigis_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripTrivialDigiAnalysis.h"
#include <string>

/**
   @class AnalyzeSiStripDigis 
   @brief Simple class that analyzes Digis produced by RawToDigi unpacker
*/
class AnalyzeSiStripDigis : public edm::EDAnalyzer {

 public:
  
  AnalyzeSiStripDigis( const edm::ParameterSet& );
  ~AnalyzeSiStripDigis();
  
  void beginJob( edm::EventSetup const& );
  void analyze( const edm::Event&, const edm::EventSetup& );
  void endJob();
  
 private:

  std::string inputModuleLabel_;

  bool createDigis_;

  SiStripTrivialDigiAnalysis anal_;

  SiStripTrivialDigiAnalysis vr_p;
  SiStripTrivialDigiAnalysis pr_p;
  SiStripTrivialDigiAnalysis sm_p;
  SiStripTrivialDigiAnalysis zs_p;

  SiStripTrivialDigiAnalysis vr_r;
  SiStripTrivialDigiAnalysis pr_r;
  SiStripTrivialDigiAnalysis sm_r;
  SiStripTrivialDigiAnalysis zs_r;
  
};

#endif // EventFilter_SiStripRawToDigi_test_AnalyzeSiStripDigis_H

