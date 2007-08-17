#ifndef simpleanalysis_SimpleJet_H
#define simpleanalysis_SimpleJet_H

class SimpleJet : public SimpleGenJet {

 public:

  SimpleJet() : SimpleGenJet(), tracks_(), discriminator_(constants::invalid) {;}

  SimpleJet(double vx, double vy, double vz, SimpleHCluster& hcluster, std::vector<SimpleTrack>& tracks, double discriminator) : SimpleGenJet(vx,vy,vz,hcluster), tracks_(tracks), discriminator_(discriminator) {;}

    virtual ~SimpleJet() {;}

  const std::vector<SimpleTrack> tracks() const {return tracks_;}
  const double discriminator() const {return discriminator_;}

  bool operator < (const SimpleJet& compare) const {

    if (tag() && !compare.tag()) return true;
    else if (compare.tag() && !tag()) return false;
    else if (compare.hcluster().et() < hcluster().et()) return true;
    else return false;
  }

 private:

  std::vector<SimpleTrack> tracks_;
  double discriminator_;

  ClassDef(SimpleJet,1)

};

#endif
