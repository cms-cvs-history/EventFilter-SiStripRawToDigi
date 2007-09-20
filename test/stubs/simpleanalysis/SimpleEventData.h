#ifndef simpleanalysis_SimpleEventData_H
#define simpleanalysis_SimpleEventData_H

class SimpleEventData : public TObject {

 public:

  SimpleEventData() : mc_(), mcjets_(), mcmet_(0.,0.), sistripdigis_(), sistripclusters_(), tracks_(), electrons_(), muons_(), jets_(), trigger_(), met_(0.,0.), event_(0), channel_() {}
  virtual ~SimpleEventData() {}
   
  /** Monte Carlo */

  std::vector<SimpleGenParticle>& mc() {return mc_;}
  std::vector<SimpleGenJet>& mcjets() {return mcjets_;}
  SimpleMet& mcmet() {return mcmet_;}

  /** Reconstructed */

  std::vector<SimpleSiStripDigi>& sistripdigis() {return sistripdigis_;}
  std::vector<SimpleSiStripCluster>& sistripclusters() {return sistripclusters_;}
  std::vector<SimpleTrack>& tracks() {return tracks_;}
  std::vector<SimpleElectron>& electrons() {return electrons_;}
  std::vector<SimpleMuon>& muons() {return muons_;}
  std::vector<SimpleJet>& jets() {return jets_;}
  SimpleTrigger& trigger() {return trigger_;}
  SimpleMet& met() {return met_;}

  /** General */

  unsigned int& event() {return event_;}
  std::string& channel() {return channel_;}
  
  void clear() {
    mc_.clear();
    mcjets_.clear();
    mcmet_.reset();
    sistripdigis_.clear();
    sistripclusters_.clear();
    tracks_.clear();
    electrons_.clear();
    muons_.clear();
    jets_.clear();
    trigger_.reset();
    met_.reset();
  }

  void order() {
    std::sort(mc_.begin(),mc_.end());
    std::sort(mcjets_.begin(),mcjets_.end());
    std::sort(sistripdigis_.begin(),sistripdigis_.end());
    std::sort(sistripclusters_.begin(),sistripclusters_.end());
    std::sort(tracks_.begin(),tracks_.end());
    std::sort(electrons_.begin(),electrons_.end());
    std::sort(muons_.begin(),muons_.end());
    std::sort(jets_.begin(),jets_.end());
  }
  
  unsigned int mcjetnum(const double cut) {
    unsigned int count=0;
    for (unsigned int i = 0; i < mcjets_.size(); i++) {
      if (mcjets_[i].hcluster().et() > cut) count++;
      else {break;}
    }
    return count;
  }

  unsigned int mcjettagnum(const double cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < jets_.size(); i++) {
      if ((mcjets_[i].hcluster().et() > cut) && (mcjets_[i].tag())) count++;
      else {break;}
    }
    return count;
  }

  unsigned int sistripdiginum(const unsigned short cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < sistripdigis_.size(); i++) {
      if (sistripdigis_[i].amplitude() > cut) count++;
    }
    return count;
  }

  unsigned int sistripclusternum(const unsigned short cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < sistripclusters_.size(); i++) {
      if (sistripclusters_[i].amplitudes() > cut) count++;
    }
    return count;
  }

  unsigned int tracknum(const double cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < tracks_.size(); i++) {
      if (tracks_[i].innerPt() > cut) count++;
      else {break;}
    }
    return count;
  }

  unsigned int electronnum(const double cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < electrons_.size(); i++) {
      if ((electrons_[i].scluster().et() > cut) && (electrons_[i].tag())) count++;
      else {break;}
    }
    return count;
  }

  unsigned int muonnum(const double cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < electrons_.size(); i++) {
      if ((muons_[i].track().innerPt() > cut) && (muons_[i].tag())) count++;
      else {break;}
    }
    return count;
  }
  
  unsigned int jetnum(const double cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < jets_.size(); i++) {
      if (jets_[i].hcluster().et() > cut) count++;
      else {break;}
    }
    return count;
  }
  
  unsigned int jettagnum(const double cut=0) {
    unsigned int count=0;
    for (unsigned int i = 0; i < jets_.size(); i++) {
      if ((jets_[i].hcluster().et() > cut) && (jets_[i].tag())) count++;
      else {break;}
    }
    return count;
  }
  
  double dielectron(const double cut) {return ((electronnum(cut) >= 2)) ? SimpleElectron::parentMass(electrons_[0],electrons_[1]) : constants::invalid;}

 private:
 
 /** Monte Carlo */
 
 std::vector<SimpleGenParticle> mc_;
 std::vector<SimpleGenJet> mcjets_;
 SimpleMet mcmet_;
 
 /** Reconstructed */
 
 std::vector<SimpleSiStripDigi> sistripdigis_;
 std::vector<SimpleSiStripCluster> sistripclusters_;
 std::vector<SimpleTrack> tracks_;
 std::vector<SimpleElectron> electrons_;
 std::vector<SimpleMuon> muons_;
 std::vector<SimpleJet> jets_;
 SimpleTrigger trigger_;
 SimpleMet met_;
 
 /** General */
 
 unsigned int event_;
 std::string channel_;
 
 ClassDef(SimpleEventData,1)
   
 };

#endif
