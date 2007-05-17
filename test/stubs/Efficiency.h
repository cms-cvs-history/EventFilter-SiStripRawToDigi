#ifndef EventFilter_SiStripRawToDigi_Efficiency_H
#define EventFilter_SiStripRawToDigi_Efficiency_H

#include "TH1F.h"
#include "TGraphErrors.h"
#include <iostream>
#include "EventFilter/SiStripRawToDigi/test/stubs/Utility.h"

class Efficiency : public TObject {

 public:
  
  Efficiency() : nbins_(0), xmin_(0), xmax_(0), selected(0), all(0), efficiency(0) { 
    selected = new TH1F();
    all = new TH1F();
    efficiency = new TH1F();
  }

  Efficiency(const char* name, const char* title, Int_t bins, Double_t xmin, Double_t xmax) : nbins_(bins), xmin_(xmin), xmax_(xmax), selected(0), all(0), efficiency(0) {
    selected = new TH1F("Efficiency::selected","Efficiency::selected",bins,xmin,xmax);
    all = new TH1F("Efficiency::all","Efficiency::all",bins,xmin,xmax);
    efficiency = new TH1F(name,title,bins,xmin,xmax);
  }
 
  ~Efficiency() {
    if (selected) delete selected;
    if (all) delete all;
    if (efficiency) delete efficiency;
  }

  void calculate() {
    for (Int_t ibin = 0; ibin < nbins_; ibin++) {
      Double_t value = 0.;
      if (all->GetBinContent(ibin+1))
	value = selected->GetBinContent(ibin+1)/all->GetBinContent(ibin+1);
      Double_t error = dEfficiency(value,all->GetBinContent(ibin+1));
      efficiency->SetBinContent(ibin+1,value);
      efficiency->SetBinError(ibin+1,error);
    }
  }

  void select(Int_t bin, bool valid=true) {
    all->AddBinContent(bin);
    if (valid) selected->AddBinContent(bin);
  }

  void select(Double_t value, bool valid=true) {
    all->Fill(value);
    if (valid) selected->Fill(value);
  }

  TH1F* const get() {
    return efficiency;
  }

  static bool comparable(Efficiency* one,
			 Efficiency* two) {
    if ((one->bins() == two->bins()) &&
	(one->xmin() == two->xmin()) &&
	(one->xmax() == two->xmax())) return true; 
    return false;
  }
  
  static void compare(TGraphErrors* result,
		      Efficiency* one,
		      Efficiency* two) {
    
    if (!comparable(one,two) ||
	result->GetN() != one->bins()) {
      std::cout << "Efficiency::" 
		<< __func__
		<< "]"
		<< "Efficiency objects incomparable."
		<< std::endl;
      return;

      for (Int_t ipoint = 0; ipoint < result->GetN(); ipoint++) {
	//value
	result->GetX()[ipoint] = one->efficiency->GetBinContent(ipoint+1);
	result->GetY()[ipoint] = two->efficiency->GetBinContent(ipoint+1);
	//error
	result->GetEX()[ipoint] = one->efficiency->GetBinError(ipoint+1);
	result->GetEY()[ipoint] = two->efficiency->GetBinError(ipoint+1);
      }
    }
  }
  
  Int_t bins() {
    return nbins_;
  }
  
  Double_t xmin() {
    return xmin_;
  }
  
  Double_t xmax() {
    return xmax_;
  }

 private:
  
  Int_t nbins_;
  Double_t xmin_;
  Double_t xmax_;
  TH1F* selected;
  TH1F* all;
  TH1F* efficiency;
  
  ClassDef(Efficiency,1)
    
};

#endif
