#ifndef EventFilter_SiStripRawToDigi_test_stubs_SimpleSiStripCluster_H
#define EventFilter_SiStripRawToDigi_test_stubs_SimpleSiStripCluster_H

#include "TObject.h"
#include <vector>

class SimpleSiStripCluster : public TObject {

 public:

  SimpleSiStripCluster();
  ~SimpleSiStripCluster();

  unsigned int detId_;
  unsigned short firstStrip_;
  std::vector<double> amplitudes_;

  ClassDef(SimpleSiStripCluster,1)
};

#endif
