#ifndef simpleanalysis_SimpleMet_H
#define simpleanalysis_SimpleMet_H

class SimpleMet : public TObject {

 public:

  SimpleMet() :  rawet_(constants::invalid), et_(constants::invalid) {}
  SimpleMet(double rawet, double et) : rawet_(rawet), et_(et) {}
  virtual ~SimpleMet() {}

  const double rawet() const {return rawet_;}
  const double et() const {return et_;} 
  void reset() {rawet_ = constants::invalid; et_ = constants::invalid;}

  bool operator < (const SimpleMet& compare) const {
    if (compare.et() < et()) return true;
    return false;
  }

 private:

  double rawet_;
  double et_;

  ClassDef(SimpleMet,1)
};

#endif
