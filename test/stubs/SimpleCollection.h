#ifndef EventFilter_SiStripRawToDigi_test_stubs_SimpleSiStripCollection_H
#define EventFilter_SiStripRawToDigi_test_stubs_SimpleSiStripCollection_H

#include "TObject.h"
#include <vector>
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSiStripCluster.h"

class SimpleSiStripCollection : public TObject {

 public:

  SimpleSiStripCollection();
  ~SimpleSiStripCollection();

  std::vector<SimpleSiStripCluster> clusters_;

  ClassDef(SimpleSiStripCollection,1)
};

#endif
