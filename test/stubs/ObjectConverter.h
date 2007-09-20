#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/HLTenums.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleAnalysis.h"

namespace objectconverter {

  /** Generator level */

  SimpleGenParticle particle(const HepMC::GenParticle&, const edm::ESHandle<ParticleDataTable>&);

  SimpleGenJet jet(const reco::GenJet&);

  SimpleMet met(const reco::GenMET&);

  /** Reconstructed */

  SimpleSiStripDigi sistripdigi(const SiStripDigi&, const unsigned int);

  SimpleSiStripCluster sistripcluster(const SiStripCluster&);

  SimpleTrack track(const reco::Track&);

  SimpleJet jet(const reco::JetTag&, const reco::JetTag&);  

  SimpleElectron electron(const reco::PixelMatchGsfElectron&, const reco::ClusterShapeRef&);

  SimpleMet met(const reco::CaloMET&, const reco::CaloMET&);

  SimpleTrigger trigger(const edm::TriggerResults&);

  /** Reconstructed - from four vectors */

  SimpleElectron electron(const reco::Candidate&);

  SimpleMuon muon(const reco::Candidate&);

  SimpleJet jet(const reco::Candidate&);
}
