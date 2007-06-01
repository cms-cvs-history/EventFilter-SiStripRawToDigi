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
#include <iostream>

class Overlay {
  
 public:

  Overlay(): canvas_(0), legend_(0), labOne_(""), labTwo_(""), labThree_(""), labFour_(""), labFive_("") {
    
      canvas_ = new TCanvas("", "",200,10,700,500);
      legend_ = new TLegend(0.7,0.56,0.9,0.69);
      legend_->SetFillColor(kWhite);
    }
  
  ~Overlay() { delete canvas_; delete legend_; }

  void label(std::string labOne="", std::string labTwo="", std::string labThree="", std::string labFour="", std::string labFive="") {
    labOne_ = labOne;
    labTwo_ = labTwo;
    labThree_ = labThree;
    labFour_ = labFour;
    labFive_ = labFive;
  }
  
  void graphs(TGraph* const one, TGraph* const two=0, TGraph* const three=0, TGraph* const four=0,TGraph* const five=0, bool legend=false, std::string option="") {
    
    if (!one) return;
    reset();
    
    //Draw graph and build legend
    one->SetMarkerColor(onecolor);
    one->Draw(("AP"+option).c_str());
    if (legend) legend_->AddEntry(one,labOne_.c_str(),"p");
  
  if (two) {
    two->SetMarkerColor(twocolor);
    two->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(two,labTwo_.c_str(),"p");}

  if (three) {
    three->SetMarkerColor(threecolor);
    three->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(three,labThree_.c_str(),"p");}

  if (four) {
    four->SetMarkerColor(fourcolor);
    four->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(four,labThree_.c_str(),"p");}

  if (five) {
    five->SetMarkerColor(fivecolor);
    five->Draw(("P"+option).c_str());
    if (legend) legend_->AddEntry(five,labThree_.c_str(),"p");}  
  
  //Draw legend
  if (legend) legend_->Draw();
}

void Overlay::graphs2D(TGraph2D* const graph, std::string title="", std::string xtitle="", std::string ytitle="", std::string ztitle="", std::string option="") {

  if (!graph) return;
  reset();
  
  graph->Draw(option.c_str());
  graph->SetTitle(title.c_str());
  graph->GetXaxis()->SetTitle(xtitle.c_str());
  graph->GetYaxis()->SetTitle(ytitle.c_str());
  graph->GetZaxis()->SetTitle(ztitle.c_str());
}
 
 void histos(TH1* const one, TH1* const two=0, TH1* const three=0, TH1* const four=0, TH1* const five=0, bool legend=false, std::string option="") {

   if (!one) return;
   reset();

  //Format histo and build legend
    one->SetLineColor(onecolor);
    one->SetMarkerColor(onecolor);
    if (legend) legend_->AddEntry(one,labOne_.c_str(),"l");

  if (two) {
    two->SetLineColor(twocolor);
    two->SetMarkerColor(twocolor);
    if (legend) legend_->AddEntry(two,labTwo_.c_str(),"l");}

  if (three) {
    three->SetLineColor(threecolor);
    three->SetMarkerColor(threecolor);
    if (legend) legend_->AddEntry(three,labThree_.c_str(),"l");}

  if (four) {
    four->SetLineColor(fourcolor);
    four->SetMarkerColor(fourcolor);
    if (legend) legend_->AddEntry(four,labFour_.c_str(),"l");}

  if (five) {
    five->SetLineColor(fivecolor);
    five->SetMarkerColor(fivecolor);
    if (legend) legend_->AddEntry(five,labFive_.c_str(),"l");}

  //Draw
  std::vector<Double_t> max(5,0);
  max[0] = one->GetMaximum();
  if (two) max[1] = two->GetMaximum();
  if (three) max[2] = three->GetMaximum();
  if (four) max[3] = four->GetMaximum();
  if (five) max[4] = five->GetMaximum();
  Int_t maxy = max_element(max.begin(),max.end())-max.begin();

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
 
 TMarker* marker(Double_t x,Double_t y,Int_t mark) {
   
   //NB memory leak here....
   TMarker* marker = new TMarker(x,y,mark);
   canvas_->cd();
   marker->Draw();
   return marker;
 }
 
 void save(std::string name) {
   canvas_->SetName(name.c_str());
   canvas_->Write(name.c_str(), TObject::kOverwrite);
 }
 

  TCanvas* canvas_;

 private:

 void reset() {

   canvas_->Clear();
   legend_->Clear();
   canvas_->SetLogx(0);
   canvas_->SetLogy(0);
 }

 static const Int_t onecolor = 1; 
 static const Int_t twocolor = 2;
 static const Int_t threecolor = 104;
 static const Int_t fourcolor = 106; 
 static const Int_t fivecolor = 108;
 
 TLegend* legend_;
 std::string labOne_;
 std::string labTwo_;
 std::string labThree_;
 std::string labFour_;
 std::string labFive_;
};

#endif
