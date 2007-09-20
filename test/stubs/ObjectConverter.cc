#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"

#include "EventFilter/SiStripRawToDigi/test/stubs/ObjectConverter.h"

SimpleGenParticle objectconverter::particle(const HepMC::GenParticle& particle, const edm::ESHandle<ParticleDataTable>& pdt) 
{
  return SimpleGenParticle(particle.momentum().perp(), -1.0*log(tan(0.5*(particle.momentum().theta()))), particle.momentum().phi(), 0., 0., 0., particle.pdg_id(), static_cast<int>(pdt->particle(particle.pdg_id())->charge()));
}

SimpleGenJet objectconverter::jet(const reco::GenJet& jet) 
{
  SimpleHCluster simplehcluster(constants::invalid, jet.energy(), jet.eta(), jet.phi());
  return SimpleGenJet(jet.vertex().X(), jet.vertex().Y(), jet.vertex().Z(), simplehcluster);
}

SimpleMet objectconverter::met(const reco::GenMET& genmet) 
{
  return SimpleMet(constants::invalid,genmet.et());
}

SimpleSiStripDigi objectconverter::sistripdigi(const SiStripDigi& digi, const unsigned int detid) 
{
  return SimpleSiStripDigi(detid,digi.strip(),digi.adc());
}

SimpleSiStripCluster objectconverter::sistripcluster(const SiStripCluster& cluster) 
{
  return SimpleSiStripCluster(cluster.geographicalId(), cluster.firstStrip(), cluster.amplitudes().size(), cluster.barycenter());
}

SimpleTrack objectconverter::track(const reco::Track& track) 
{
  return SimpleTrack(track.momentum().Rho(), track.momentum().Eta(), track.momentum().Phi(), track.vertex().X(), track.vertex().Y(), track.vertex().Z(), track.outerMomentum().Rho(), track.outerMomentum().Eta(), track.outerMomentum().Phi(), track.outerPosition().X(), track.outerPosition().Y(), track.outerPosition().Z(), track.charge(), track.found());
}
  
SimpleTrack objectconverter::track(const reco::PixelMatchGsfElectron& electron)
{
  return SimpleTrack(electron.trackMomentumAtVtx().Rho(), electron.trackMomentumAtVtx().Eta(), electron.trackMomentumAtVtx().Phi(), electron.TrackPositionAtVtx().X(), electron.TrackPositionAtVtx().Y(), electron.TrackPositionAtVtx().Z(), electron.trackMomentumAtCalo().Rho(), electron.trackMomentumAtCalo().Eta(), electron.trackMomentumAtCalo().Phi(), electron.TrackPositionAtCalo().X(), electron.TrackPositionAtCalo().Y(), electron.TrackPositionAtCalo().Z(), electron.gsfTrack().get()->charge(), electron.gsfTrack().get()->found());
}

std::vector<SimpleTrack> objectconverter::tracks(const reco::JetTag& jet)
{
  std::vector<SimpleTrack> simpletracks;
  for (unsigned int i=0;i<jet.tracks().size();i++) {
    const reco::Track& tk = *(jet.tracks()[i]); 
    simpletracks.push_back(track(tk));
  }
  return simpletracks;
}

SimpleSCluster objectconverter::supercluster(const reco::PixelMatchGsfElectron& electron, const reco::ClusterShapeRef& shape)
{
   return SimpleSCluster(electron.superCluster().get()->rawEnergy(), electron.caloEnergy(), electron.superCluster().get()->eta(), electron.superCluster().get()->phi(), electron.hadronicOverEm(), shape->e2x2(), shape->e3x3(), shape->e5x5(), shape->eMax(), shape->covEtaEta());
}

SimpleHCluster objectconverter::hcluster(const reco::JetTag& jet, const reco::JetTag& rawjet)
{
 return SimpleHCluster(rawjet.jet().get()->energy(),jet.jet().get()->energy(),jet.jet().get()->eta(),jet.jet().get()->phi());
}

SimpleJet objectconverter::jet(const reco::JetTag& jet, const reco::JetTag& rawjet) 
{
  return SimpleJet(jet.jet().get()->vertex().X(), jet.jet().get()->vertex().Y(), jet.jet().get()->vertex().Z(), hcluster(jet,rawjet), tracks(jet), jet.discriminator());
}

SimpleMet objectconverter::met(const reco::CaloMET& rawmet, const reco::CaloMET& met) 
{
  return SimpleMet(rawmet.et(),met.et());
}

SimpleElectron objectconverter::electron(const reco::PixelMatchGsfElectron& electron, const reco::ClusterShapeRef& shape) 
{
  return SimpleElectron(track(electron),supercluster(electron,shape),electron.classification());
}

SimpleTrigger objectconverter::trigger(const edm::TriggerResults& trigger) 
{
  SimpleTrigger simpletrigger(trigger.size());
  for (unsigned int i=0;i<trigger.size();i++) {
    simpletrigger.set(i,(trigger.at(i).state() == edm::hlt::Pass));
  }
  return simpletrigger;
}

SimpleElectron objectconverter::electron(const reco::Candidate& candidate) 
{
  SimpleSCluster simplescluster(candidate.energy(), constants::invalid, candidate.eta(), candidate.phi(), constants::invalid, constants::invalid, constants::invalid, constants::invalid, constants::invalid, constants::invalid);
  return SimpleElectron(SimpleTrack(),simplescluster,constants::invalid32);
}

SimpleMuon objectconverter::muon(const reco::Candidate& candidate) {
  SimpleTrack track(candidate.pt(),candidate.eta(),candidate.phi(),candidate.vx(),candidate.vy(),candidate.vz(),constants::invalid,constants::invalid,constants::invalid,constants::invalid,constants::invalid,constants::invalid,constants::invalid32,constants::invalid16);
  return SimpleMuon(track);
}

SimpleJet objectconverter::jet(const reco::Candidate& candidate) {
  SimpleHCluster simplehcluster(candidate.energy(),constants::invalid,candidate.eta(),candidate.phi());
  return SimpleJet(candidate.vx(),candidate.vy(),candidate.vz(),simplehcluster,std::vector<SimpleTrack>(),constants::invalid);
}
