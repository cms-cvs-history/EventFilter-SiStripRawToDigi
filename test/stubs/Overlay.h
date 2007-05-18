#ifndef EventFilter_SiStripRawToDigi_Overlay_H
#define EventFilter_SiStripRawToDigi_Overlay_H

#include "TLegend.h"
#include "TMarker.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TH1.h"
#include "TCanvas.h"
#include <string>
#include <algorithm>

class Overlay {

 public:

  Overlay(): canvas_(0), legend_(0), labOne_(""), labTwo_(""), labThree_(""), labFour_(""), labFive_("") {

    canvas_ = new TCanvas("", "",200,10,700,500);
    legend_ = new TLegend(0.7,0.56,0.9,0.69);
    legend_->SetFillColor(kWhite);
  }
  
  ~Overlay() {
    delete canvas_;
    delete legend_;
  }

  void label(std::string labOne="", std::string labTwo="", std::string labThree="", std::string labFour="", std::string labFive="") {
    labOne_ = labOne;
    labTwo_ = labTwo;
    labThree_ = labThree;
    labFour_ = labFour;
    labFive_ = labFive;
  }
  
  void graphs(TGraph* const one, TGraph* const two=0, TGraph* const three=0, TGraph* const four=0,TGraph* const five=0, bool legend=false, string option="") {
    
  canvas_->Clear();
  legend_->Clear();
  //canvas_->SetGrid();
  canvas_->SetLogx(0);
  canvas_->SetLogy(0);

  //Format graph and build legend
  if (one) {
    one->SetMarkerColor(kBlue);
    one->Draw(("AP"+option).c_str());
    if (legend) legend_->AddEntry(one,labOne_.c_str(),"p");}

  if (two) {
    two->SetMarkerColor(kRed);
    two->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(two,labTwo_.c_str(),"p");}

  if (three) {
    three->SetMarkerColor(kBlack);
    three->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(three,labThree_.c_str(),"p");}

  if (four) {
    four->SetMarkerColor(kGreen);
    four->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(four,labThree_.c_str(),"p");}

  if (five) {
    five->SetMarkerColor(kMagenta);
    five->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(five,labThree_.c_str(),"p");}  
  
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
 
 void histos(TH1* const one,TH1* const two=0,TH1* const three=0,TH1* const four=0,TH1* const five=0,bool legend=false,string option="") {

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

  if (four) {
    four->SetLineColor(kGreen);
    four->SetMarkerColor(kBlack);
    if (legend) legend_->AddEntry(four,labThree_.c_str(),"l");}

  if (five) {
    five->SetLineColor(kMagenta);
    five->SetMarkerColor(kBlack);
    if (legend) legend_->AddEntry(five,labThree_.c_str(),"l");}

  //Draw
  Int_t maxtwo = 0, maxthree = 0, maxfour = 0, maxfive = 0;
  if (two) maxtwo = two->GetMaximumBin();
  if (three) maxthree = three->GetMaximumBin();
  if (four) maxthree = four->GetMaximumBin();
  if (five) maxthree = five->GetMaximumBin();

  Int_t yrange[] = {one->GetMaximumBin(),maxtwo,maxthree,maxfour,maxfive};
  Int_t maxy = max_element(yrange,yrange+5)-yrange;

  if (maxy == 0) {
    one->Draw(option.c_str());
    if (two) two->Draw(("SAME"+option).c_str());
    if (three) three->Draw(("SAME"+option).c_str());
    if (four) four->Draw(("SAME"+option).c_str());
    if (five) five->Draw(("SAME"+option).c_str());
  }

  else if (maxy == 1) {
    two->Draw(option.c_str());
    one->Draw(("SAME"+option).c_str());
    if (three) three->Draw(("SAME"+option).c_str());
    if (four) four->Draw(("SAME"+option).c_str());
    if (five) five->Draw(("SAME"+option).c_str());
  }

  else if (maxy == 2) {
    three->Draw(option.c_str());
    one->Draw(("SAME"+option).c_str());
    if (two) two->Draw(("SAME"+option).c_str());
    if (four) four->Draw(("SAME"+option).c_str());
    if (five) five->Draw(("SAME"+option).c_str());
  }

  else if (maxy == 3) {
    four->Draw(option.c_str());
    one->Draw(("SAME"+option).c_str());
    if (two) two->Draw(("SAME"+option).c_str());
    if (three) three->Draw(("SAME"+option).c_str());
    if (five) five->Draw(("SAME"+option).c_str());
  }

  else if (maxy == 4) {
    five->Draw(option.c_str());
    one->Draw(("SAME"+option).c_str());
    if (two) two->Draw(("SAME"+option).c_str());
    if (three) three->Draw(("SAME"+option).c_str());
    if (four) four->Draw(("SAME"+option).c_str());
  }

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
  std::string labFour_;
  std::string labFive_;
};

#endif
