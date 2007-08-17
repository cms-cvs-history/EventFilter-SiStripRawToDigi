#ifndef simpleanalysis_SimpleTrigger_H
#define simpleanalysis_SimpleTrigger_H

class SimpleTrigger : public TObject {

 public:

  SimpleTrigger(unsigned int size=0) : trigger_(size,false) {;}
  ~SimpleTrigger() {}

  unsigned int size() const {return trigger_.size();}
  void set(unsigned short bit, bool trigger=true) {trigger_[bit]=trigger;}
  void reset() {trigger_.assign(trigger_.size(),false);}
  const bool get(unsigned short bit) const {return trigger_[bit];}
  const std::vector<bool> trigger() const {return trigger_;}

  bool operator == (const SimpleTrigger& compare) const {
    return (compare.trigger() == trigger());
  }

 private:

  std::vector<bool> trigger_;

  ClassDef(SimpleTrigger,1)
};

#endif
