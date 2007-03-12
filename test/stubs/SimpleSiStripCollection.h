#ifndef EventFilter_SiStripRawToDigi_test_stubs_SimpleSiStripCollection_H
#define EventFilter_SiStripRawToDigi_test_stubs_SimpleSiStripCollection_H

#include "TObject.h"
#include <vector>
#include <string>
#include "EventFilter/SiStripRawToDigi/test/stubs/SimpleSiStripCluster.h"

class SimpleSiStripCollection : public TObject {

 public:

  SimpleSiStripCollection(std::string="");
  ~SimpleSiStripCollection();

  std::vector<SimpleSiStripCluster> clusters_;
  std::string label_;

  ClassDef(SimpleSiStripCollection,1)
};

#endif
