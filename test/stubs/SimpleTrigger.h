
#ifndef EventFilter_SiStripRawToDigi_SimpleTrigger_h
#define EventFilter_SiStripRawToDigi_SimpleTrigger_h

#include "TObject.h"
#include <string>
#include <cmath>

class SimpleTrigger : public TObject {

 public:

  SimpleTrigger(unsigned int num=0) : trigger_(num,false) {;}
  ~SimpleTrigger() {}

  void set(unsigned short bit, bool trigger=true) {trigger_[bit]=trigger;}
  void reset() {trigger_.assign(trigger_.size(),false);}
  const bool get(unsigned short bit) const {return trigger_[bit];}

 private:

  std::vector<bool> trigger_;

  ClassDef(SimpleTrigger,1)
};

#endif
