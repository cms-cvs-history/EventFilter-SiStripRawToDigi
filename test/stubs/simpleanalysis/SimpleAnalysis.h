#ifndef simpleanalysis_SimpleAnalysis_H
#define simpleanalysis_SimpleAnalysis_H

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "TKey.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TMarker.h"
#include "TCanvas.h"

#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleConstants.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleOverlay.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleEfficiency.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleEfficiencyGraph.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleEfficiencyGraph2D.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimplePurityGraph.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimplePurityGraph2D.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleCorrelation.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleGenParticle.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleHCluster.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleGenJet.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleSiStripDigi.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleSiStripCluster.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleTrack.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleSCluster.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleElectron.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleMuon.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleJet.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleMet.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleTrigger.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleEventData.h"

#endif
