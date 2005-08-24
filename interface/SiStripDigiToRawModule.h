#ifndef SiStripDigiToRawModule_H
#define SiStripDigiToRawModule_H

// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
#include <string>

class SiStripDigiToRaw;
class SiStripUtility;

/**
   \class SiStripDigiToRawModule 
   \brief A plug-in EDProducer module that takes a StripDigiCollection
   as input and creates an EDProduct in the form of a
   FEDRawDataCollection. 
   \author R.Bainbridge
   \version 0.1
   \date 09/08/05
   A plug-in EDProducer module that takes a StripDigiCollection as
   input and creates an EDProduct in the form of a
   FEDRawDataCollection. 
   Nota bene: this is a PROTOTYPE IMPLEMENTATION!
*/
class SiStripDigiToRawModule : public edm::EDProducer {
  
public:
  
  /** */
  explicit SiStripDigiToRawModule( const edm::ParameterSet& );
  /** */
  ~SiStripDigiToRawModule();

  /** */
  virtual void beginJob( const edm::EventSetup& );
  /** */
  virtual void endJob();
  
  /** method that retrieves a StripDigiCollection from the Event and
      creates an EDProduct in the form of a FEDRawDataCollection. */
  virtual void produce( edm::Event&, const edm::EventSetup& );
  
private:
  
  /** object that converts StripDigi's to FEDRawData objects */
  SiStripDigiToRaw* digiToRaw_;
  /** utility class providing dummy Digis, FED buffers, cabling map */
  SiStripUtility* utility_;

  /** event counter */
  unsigned long event_;

  /** defines the FED readout path: VME or SLINK */
  std::string fedReadoutPath_;
  /** defines level of verbosity for this class (0=silent -> 3=debug) */
  int verbosity_;
  
};

#endif // SiStripDigiToRawModule_H
