#ifndef simpleanalysis_SimpleSiStripDigi_H
#define simpleanalysis_SimpleSiStripDigi_H

class SimpleSiStripDigi : public TObject {

 public:

  /** Constructors */

  SimpleSiStripDigi() :   detid_(constants::invalid32), strip_(constants::invalid16), amplitude_(constants::invalid16) {}

  SimpleSiStripDigi(unsigned int detid, unsigned short strip, unsigned short amplitude) :   detid_(detid), strip_(strip), amplitude_(amplitude) {}

  /** Desctructor */

  virtual ~SimpleSiStripDigi() {}

  /** Getters */

  const unsigned int detid() const {return detid_;}

  const unsigned short strip() const {return strip_;}

  const unsigned short amplitude() const {return amplitude_;}
  
 private: 

  unsigned int detid_;
  unsigned short strip_;
  unsigned short amplitude_;

  ClassDef(SimpleSiStripDigi,1)
};

inline bool operator < (const unsigned int detid,const SimpleSiStripDigi& digi) 
{
  return (detid < digi.detid()) ? true : false;
}

inline bool operator < (const SimpleSiStripDigi& digi,const unsigned int detid) 
{
  return (digi.detid() < detid) ? true : false;
}

inline bool operator < (const SimpleSiStripDigi& one,const SimpleSiStripDigi& two) 
{
  return ((one.detid() < two.detid()) || (one.detid() == two.detid() && one.strip() < two.strip())) ? true : false;
}

#endif
