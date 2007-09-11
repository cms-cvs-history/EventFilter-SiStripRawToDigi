#ifndef simpleanalysis_SimpleJet_H
#define simpleanalysis_SimpleJet_H

class SimpleJet : public SimpleGenJet {

 public:

  SimpleJet() : SimpleGenJet(), tracks_(), discriminator_(constants::invalid) {;}

  SimpleJet(double vx, double vy, double vz, SimpleHCluster& hcluster, std::vector<SimpleTrack>& tracks, double discriminator) : SimpleGenJet(vx,vy,vz,hcluster), tracks_(tracks), discriminator_(discriminator) {;}

    virtual ~SimpleJet() {;}

  const std::vector<SimpleTrack> tracks() const {return tracks_;}
  const double discriminator() const {return discriminator_;}

 private:

  std::vector<SimpleTrack> tracks_;
  double discriminator_;

  ClassDef(SimpleJet,1)

};

inline bool operator < (const SimpleJet& one,const SimpleJet& two) {
  return ((one.tag() && !two.tag()) || (one.tag() == two.tag() && one.hcluster().et() < two.hcluster().et())) ? true : false;
}

#endif
