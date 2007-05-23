#include "PerformanceOverlay.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/Utility.h"
#include <iostream>

using namespace std;

PerformanceOverlay::PerformanceOverlay(TRFIOFile* file) :

  file_(file),
  overlay_(0)
{;}

PerformanceOverlay::~PerformanceOverlay() {;}

void PerformanceOverlay::book() {
  overlay_ = new Overlay();
}

void PerformanceOverlay::unbook() {
  if (overlay_) delete overlay_;
}

void PerformanceOverlay::overlay(string type) {
 
  TDirectory* dir = file_->GetDirectory(type.c_str());
  if (!dir) return;
  std::vector<TH1*> th1s = utility::th1s(dir);  
  th1s.resize(5,0);

  std::string labelone="", labeltwo="", labelthree="", labelfour="", labelfive="";
  if (th1s[0]) labelone = th1s[0]->GetName();
  if (th1s[1]) labeltwo = th1s[1]->GetName();
  if (th1s[2]) labelthree = th1s[2]->GetName();
  if (th1s[3]) labelfour = th1s[3]->GetName();
  if (th1s[4]) labelfive = th1s[4]->GetName();

  overlay_->label(labelone,labeltwo,labelthree,labelfour,labelfive);
  overlay_->histos(th1s[0],th1s[1],th1s[2],th1s[3],th1s[4],true);
  dir->cd(); overlay_->save("OVERLAY");
}
