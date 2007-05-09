#ifndef EventFilter_SiStripRawToDigi_SimpleTrack_h
#define EventFilter_SiStripRawToDigi_SimpleTrack_h

#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleParticle.h"

class SimpleTrack : public TObject {

 public:

  SimpleTrack() : inner(), outer(), charge(constants::invalid32) {}

  SimpleTrack(SimpleParticle& Inner, SimpleParticle& Outer, int Charge) :   inner(Inner), outer(Outer), charge(Charge) {}

  virtual ~SimpleTrack() {}

  bool operator < (const SimpleTrack& compare) const {
    if (compare.inner.pt < inner.pt) return true;
    return false;
  }

  SimpleParticle inner;
  SimpleParticle outer;
  int charge;

  ClassDef(SimpleTrack,1)
};

#endif
