#include "EventFilter/SiStripRawToDigi/test/stubs/macros/PerformanceOverlay.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

PerformanceOverlay::PerformanceOverlay(TFile* file) : file_(file), overlay_(0) {}

PerformanceOverlay::~PerformanceOverlay() {}

void PerformanceOverlay::book() {overlay_ = new SimpleOverlay();}

void PerformanceOverlay::unbook() {if (overlay_) delete overlay_;}

void PerformanceOverlay::overlay(std::string path) {
 
  //Read directory
  TDirectory* dir = file_->GetDirectory(path.c_str());

  //Read TH1s from directory  
  std::vector<TH1*> th1s = utility::th1s(dir); 
  th1s.resize(5,0);

  //Record TH1 names
  std::vector<std::string> labels(th1s.size(),"");
  for (uint16_t i=0;i<th1s.size();i++) {
    if (th1s[i]) labels[i]=th1s[i]->GetName();
  }

  //Set Overlay
  overlay_->label(labels[0],labels[1],labels[2],labels[3],labels[4]);
  overlay_->option("E1");
  overlay_->histos(th1s[0],th1s[1],th1s[2],th1s[3],th1s[4]);
}

void PerformanceOverlay::save(std::string path) {

  if (!file_->Cd(path.c_str())) return;
  if (overlay_) overlay_->save("OVERLAY");
}
