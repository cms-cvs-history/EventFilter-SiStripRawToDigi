#ifndef simpleanalysis_SimpleMet_H
#define simpleanalysis_SimpleMet_H

class SimpleMet : public TObject {

 public:

  /** Constructors */

  SimpleMet() :  rawet_(constants::invalid), et_(constants::invalid) {}

  SimpleMet(double rawet, double et) : rawet_(rawet), et_(et) {}

  /** Destructor */

  virtual ~SimpleMet() {}

  /** Getters */

  const double rawet() const {return rawet_;}

  const double et() const {return et_;} 

  /** Setter */

  void reset() {rawet_ = constants::invalid; et_ = constants::invalid;}

 private:

  double rawet_;
  double et_;

  ClassDef(SimpleMet,1)
};

  inline bool operator < (const SimpleMet& one, const SimpleMet& two)
{
  return (one.et() < two.et()) ? true : false;
}

#endif
