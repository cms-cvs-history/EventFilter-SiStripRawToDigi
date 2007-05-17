#ifndef EventFilter_SiStripRawToDigi_Overlay_H
#define EventFilter_SiStripRawToDigi_Overlay_H

#include "TLegend.h"
#include "TMarker.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TH1.h"
#include "TCanvas.h"
#include <string>

class Overlay {

 public:

  Overlay(): canvas_(0), legend_(0), labOne_(""), labTwo_(""), labThree_("") {

    canvas_ = new TCanvas("", "",200,10,700,500);
    legend_ = new TLegend(0.7,0.56,0.9,0.69);
    legend_->SetFillColor(kWhite);
  }
  
  ~Overlay() {
    delete canvas_;
    delete legend_;
  }

  void label(std::string labOne="",std::string labTwo="",std::string labThree="") {
    labOne_ = labOne;
    labTwo_ = labTwo;
    labThree_ = labThree;
  }
  
  void graphs(TGraph* const one,TGraph* const two,TGraph* const three,bool legend=false,string option="") {
    
  canvas_->Clear();
  legend_->Clear();
  //canvas_->SetGrid();
  canvas_->SetLogx(0);
  canvas_->SetLogy(0);

  //Format graph and build legend
  if (one) {
    one->SetMarkerColor(kBlue);
    if (legend) legend_->AddEntry(one,labOne_.c_str(),"p");}

  if (two) {
    two->SetMarkerColor(kRed);
    if (legend) legend_->AddEntry(two,labTwo_.c_str(),"p");}

  if (three) {
    three->SetMarkerColor(kBlack);
    if (legend) legend_->AddEntry(three,labThree_.c_str(),"p");}
 
  one->Draw(("AP"+option).c_str());
  if (two) two->Draw(("P"+option).c_str());
  if (three) three->Draw(("P"+option).c_str());  
  
  //Draw legend
  if (legend) legend_->Draw();
}

void Overlay::graphs2D(TGraph2D* const graph,string title="", string xtitle="", string ytitle="", string ztitle="",string option="") {

  canvas_->Clear();
  legend_->Clear();
  //canvas_->SetGrid();
  canvas_->SetLogx(0);
  canvas_->SetLogy(0);

  if (graph) {
    graph->Draw(option.c_str());
    graph->SetTitle(title.c_str());
    graph->GetXaxis()->SetTitle(xtitle.c_str());
    graph->GetYaxis()->SetTitle(ytitle.c_str());
    graph->GetZaxis()->SetTitle(ztitle.c_str());
  }

}
 
 void histos(TH1* const one,TH1* const two,TH1* const three,bool legend=false,string option="") {

  canvas_->Clear();
  legend_->Clear();
  //canvas_->SetGrid();
  canvas_->SetLogx(0);
  canvas_->SetLogy(0);

  //Format histo and build legend
  if (one) {
    one->SetLineColor(kRed);
    one->SetMarkerColor(kRed);
    if (legend) legend_->AddEntry(one,labOne_.c_str(),"l");}

  if (two) {
    two->SetLineColor(kBlue);
    two->SetMarkerColor(kBlue);
    if (legend) legend_->AddEntry(two,labTwo_.c_str(),"l");}

  if (three) {
    three->SetLineColor(kBlack);
    three->SetMarkerColor(kBlack);
    if (legend) legend_->AddEntry(three,labThree_.c_str(),"l");}

  //Draw axis.
  Double_t maxY = 0.;
  bool ytwo = false, ythree = false;

  if (one->GetBinContent(one->GetMaximumBin()) > maxY) 
    maxY = one->GetBinContent(one->GetMaximumBin());
  
  if (two &&
      (two->GetBinContent(two->GetMaximumBin()) > maxY)) {
    maxY = two->GetBinContent(two->GetMaximumBin());
    ytwo = true;}

  if (three &&
      (three->GetBinContent(three->GetMaximumBin()) > maxY)) {
    maxY = three->GetBinContent(three->GetMaximumBin());
    ytwo = false;
    ythree = true;}

  if (ytwo) two->Draw(option.c_str());
  else if (ythree) three->Draw(option.c_str());
  else one->Draw(option.c_str());

  //Draw overlayed graphs.
  if (ytwo ||
      ythree) {
    one->Draw(("SAME"+option).c_str());}

  if (two &&
      !ytwo) {
    two->Draw(("SAME"+option).c_str());}

  if (three &&
      !ythree) {
    three->Draw(("SAME"+option).c_str());}  

  //Draw legend
  if (legend) legend_->Draw();
}
 
 void marker(Double_t x,Double_t y) {
   
   //NB memory leak here....
   TMarker* marker = new TMarker(x,y,24);
   marker->SetMarkerColor(kGreen);
   marker->SetMarkerSize(2.);
   canvas_->cd();
   marker->Draw();
 }
 
 void save(string name) {
   canvas_->SetName(name.c_str());
   canvas_->Write(name.c_str(), TObject::kOverwrite);
 }
 

  TCanvas* canvas_;

 private:

  TLegend* legend_;
  std::string labOne_;
  std::string labTwo_;
  std::string labThree_;

};

#endif
