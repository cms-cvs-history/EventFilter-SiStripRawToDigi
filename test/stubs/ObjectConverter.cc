#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/CaloMET.h"

#include "EventFilter/SiStripRawToDigi/test/stubs/ObjectConverter.h"

SimpleGenParticle objectconverter::particle(const HepMC::GenParticle& particle, const edm::ESHandle<ParticleDataTable>& pdt) {

  return SimpleGenParticle(particle.momentum().perp(), -1.0*log(tan(0.5*(particle.momentum().theta()))), particle.momentum().phi(), 0./*particle.production_vertex()->position().x()*/, 0./*particle.production_vertex()->position().y()*/, 0./*particle.production_vertex()->position().z()*/, particle.pdg_id(), static_cast<int>(pdt->particle(particle.pdg_id())->charge()));
}

SimpleGenJet objectconverter::jet(const reco::GenJet& jet) {
  
  SimpleHCluster simplehcluster(constants::invalid, jet.energy(), jet.eta(), jet.phi());
  return SimpleGenJet(jet.vertex().X(), jet.vertex().Y(), jet.vertex().Z(), simplehcluster);
}

SimpleMet objectconverter::met(const reco::GenMET& genmet) {
  return SimpleMet(constants::invalid,genmet.et());
}

SimpleSiStripDigi objectconverter::sistripdigi(const SiStripDigi& digi, const unsigned int detid) {
  return SimpleSiStripDigi(detid,digi.strip(),digi.adc());
}

SimpleSiStripCluster objectconverter::sistripcluster(const SiStripCluster& cluster) {
  return SimpleSiStripCluster(cluster.geographicalId(), cluster.firstStrip(), cluster.amplitudes().size(), cluster.barycenter());
}

SimpleTrack objectconverter::track(const reco::Track& track) {
  return SimpleTrack(track.momentum().Rho(), track.momentum().Eta(), track.momentum().Phi(), track.vertex().X(), track.vertex().Y(), track.vertex().Z(), track.outerMomentum().Rho(), track.outerMomentum().Eta(), track.outerMomentum().Phi(), track.outerPosition().X(), track.outerPosition().Y(), track.outerPosition().Z(), track.charge(), track.found());
}
  
SimpleJet objectconverter::jet(const reco::JetTag& jet, const reco::JetTag& rawjet) {
  
  std::vector<SimpleTrack> simpletracks;
  for (edm::RefVector<TrackCollection>::iterator itrack = jet.tracks().begin();itrack != jet.tracks().end();itrack++) {
    simpletracks.push_back(SimpleTrack(itrack->get()->momentum().Rho(), itrack->get()->momentum().Eta(), itrack->get()->momentum().Phi(), itrack->get()->vertex().X(), itrack->get()->vertex().Y(), itrack->get()->vertex().Z(), itrack->get()->outerMomentum().Rho(), itrack->get()->outerMomentum().Eta(), itrack->get()->outerMomentum().Phi(), itrack->get()->outerPosition().X(), itrack->get()->outerPosition().Y(), itrack->get()->outerPosition().Z(), itrack->get()->charge(), itrack->get()->found()));	
  }
  
  SimpleHCluster simplehcluster(rawjet.jet().get()->energy(),jet.jet().get()->energy(),jet.jet().get()->eta(),jet.jet().get()->phi());

  return SimpleJet(jet.jet().get()->vertex().X(), jet.jet().get()->vertex().Y(), jet.jet().get()->vertex().Z(), simplehcluster, simpletracks, jet.discriminator());
}

SimpleJet objectconverter::jet(const reco::CaloJet& jet, const edm::RefVector<TrackCollection>& trackcollection) {

  std::vector<SimpleTrack> simpletracks;
  for (edm::RefVector<TrackCollection>::const_iterator itrack=trackcollection.begin();itrack!=trackcollection.end();++itrack) {
    simpletracks.push_back(track(**itrack));
  }
  
  SimpleHCluster simplehcluster(jet.energy(),constants::invalid,jet.eta(),jet.phi());

  return SimpleJet(jet.vertex().X(),jet.vertex().Y(),jet.vertex().Z(),simplehcluster,simpletracks,constants::invalid);
}

SimpleElectron objectconverter::electron(const reco::PixelMatchGsfElectron& electron, const reco::ClusterShapeRef& shape) {
  
  SimpleSCluster simplescluster(electron.superCluster().get()->rawEnergy(), electron.caloEnergy(), electron.superCluster().get()->eta(), electron.superCluster().get()->phi(), electron.hadronicOverEm(), shape->e2x2(), shape->e3x3(), shape->e5x5(), shape->eMax(), shape->covEtaEta());
  
  SimpleTrack simpletrack(electron.trackMomentumAtVtx().Rho(), electron.trackMomentumAtVtx().Eta(), electron.trackMomentumAtVtx().Phi(), electron.TrackPositionAtVtx().X(), electron.TrackPositionAtVtx().Y(), electron.TrackPositionAtVtx().Z(), electron.trackMomentumAtCalo().Rho(), electron.trackMomentumAtCalo().Eta(), electron.trackMomentumAtCalo().Phi(), electron.TrackPositionAtCalo().X(), electron.TrackPositionAtCalo().Y(), electron.TrackPositionAtCalo().Z(), electron.gsfTrack().get()->charge(), electron.gsfTrack().get()->found());
  
  return SimpleElectron(simpletrack,simplescluster,electron.classification());
}

SimpleElectron objectconverter::electron(const reco::Electron& electron) {

  SimpleSCluster simplescluster(electron.superCluster()->rawEnergy(), electron.superCluster()->energy(), electron.superCluster()->eta(), electron.superCluster()->phi(), constants::invalid, constants::invalid, constants::invalid, constants::invalid, constants::invalid, constants::invalid);
  SimpleTrack simpletrack = track(*(electron.track()));

  return SimpleElectron(simpletrack,simplescluster,constants::invalid32);
}

SimpleMet objectconverter::met(const reco::CaloMET& rawmet, const reco::CaloMET& met) {
  return SimpleMet(rawmet.et(),met.et());
}

SimpleTrigger objectconverter::trigger(const edm::TriggerResults& trigger) {
  
  SimpleTrigger simpletrigger(trigger.size());
  for (unsigned int i=0;i<trigger.size();i++) {
    simpletrigger.set(i,(trigger.at(i).state() == edm::hlt::Pass));
  }
  return simpletrigger;
}
