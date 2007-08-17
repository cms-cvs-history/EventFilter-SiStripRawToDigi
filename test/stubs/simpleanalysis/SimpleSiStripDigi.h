#ifndef simpleanalysis_SimpleSiStripDigi_H
#define simpleanalysis_SimpleSiStripDigi_H

class SimpleSiStripDigi : public TObject {

 public:

  SimpleSiStripDigi() :   detId_(constants::invalid32), strip_(constants::invalid16), amplitude_(constants::invalid16) {}

  SimpleSiStripDigi(unsigned int DetId, unsigned short Strip, unsigned short Amplitude) :   detId_(DetId), strip_(Strip), amplitude_(Amplitude) {}

  virtual ~SimpleSiStripDigi() {}

  const unsigned int detId() const {return detId_;}
  const unsigned short strip() const {return strip_;}
  const unsigned short amplitude() const {return amplitude_;}

  bool operator < (const SimpleSiStripDigi& compare) const {
    return ((compare.amplitude_ < amplitude_) || (compare.detId_ == detId_ && compare.strip_ < strip_)) ? true : false;
  }

  unsigned int detId_;
  unsigned short strip_;
  unsigned short amplitude_;

  ClassDef(SimpleSiStripDigi,1)
};

#endif
