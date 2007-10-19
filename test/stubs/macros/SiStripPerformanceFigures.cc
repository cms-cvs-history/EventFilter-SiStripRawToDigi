#include "EventFilter/SiStripRawToDigi/test/stubs/macros/SiStripPerformanceFigures.h"
#include "EventFilter/SiStripRawToDigi/test/stubs/simpleanalysis/SimpleUtility.h"

SiStripPerformanceFigures::SiStripPerformanceFigures(TFile* file) : file_(file), vplots_(), vlabels_(), overlay_(0) {}

SiStripPerformanceFigures::~SiStripPerformanceFigures() {}

void SiStripPerformanceFigures::book() {

  overlay_ = new SimpleOverlay();
  overlay_->option("E1");
  overlay_->color(false);
}

void SiStripPerformanceFigures::read(std::string name) {

  vplots_.push_back(SiStripPerformancePlots());
  vplots_.back().read(utility::addDir(file_,"Plots_"+name));
  vlabels_.push_back(name);
}

void SiStripPerformanceFigures::unbook() {
  if(overlay_) delete overlay_;
}

void SiStripPerformanceFigures::overlay() {

  vplots_.resize(5,SiStripPerformancePlots());
  vlabels_.resize(5,"");
  utility::addDir(file_,"Overlay")->cd();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::TIME);
  overlay_->save("time");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::TIMEVSOCCUPANCY);
  overlay_->save("timeVsoccupancy");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::TIMEVSDIGIS);
  overlay_->save("timeVsdigis");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::TIMEVSCLUSTERS);
  overlay_->save("timeVsclusters");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::TIMEVSCLUSTERSIZE);
  overlay_->save("timeVsclustersize");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::TIMEVSFRAC);
  overlay_->save("timeVsfrac");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::HLT);
  overlay_->save("hlt");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::PT1);
  overlay_->save("pt1");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::PT2);
  overlay_->save("pt2");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::ETA1);
  overlay_->save("eta1");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::ETA2);
  overlay_->save("eta2");
  overlay_->reset();

  overlay_->label(vlabels_[0],vlabels_[1],vlabels_[2],vlabels_[3],vlabels_[4]);
  overlay(SiStripPerformancePlots::ETA3);
  overlay_->save("eta3");
  overlay_->reset();
}

void SiStripPerformanceFigures::overlay(SiStripPerformancePlots::TH1FType type) {

  overlay_->histos(vplots_[0].get(type), vplots_[1].get(type), vplots_[2].get(type), vplots_[3].get(type), vplots_[4].get(type));
}

void SiStripPerformanceFigures::overlay(SiStripPerformancePlots::TProfileType type) {

  overlay_->histos(vplots_[0].get(type), vplots_[1].get(type), vplots_[2].get(type), vplots_[3].get(type), vplots_[4].get(type));
}

void SiStripPerformanceFigures::overlay(SiStripPerformancePlots::EfficiencyType type) {

  std::vector<TH1F*> vth1s(vplots_.size(),static_cast<TH1F*>(0));
  for (unsigned int i=0;i<vplots_.size();i++) {
    unsigned int j=vplots_.size()-1-i;
    if (vplots_[j].get(type)) {
      vplots_[j].get(type)->divide(vplots_[0].get(type));
      vth1s[j] = vplots_[j].get(type)->get();
      /*std::cout << vth1s[j]->GetBinContent(1) << ";" << vth1s[j]->GetBinError(1) << std::endl;*/
    }
  }
  overlay_->histos(vth1s[0],vth1s[1],vth1s[2],vth1s[3],vth1s[4]);
}
