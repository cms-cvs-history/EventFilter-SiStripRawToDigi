#ifndef simpleanalysis_SimpleEfficiency_H
#define simpleanalysis_SimpleEfficiency_H

class SimpleEfficiency : public TObject {

 public:
  
  SimpleEfficiency() : nbins_(0), xmin_(0), xmax_(0), selected(0), all(0), efficiency(0) 
    { 
      selected = new TH1F();
      all = new TH1F();
      efficiency = new TH1F();
    }

  SimpleEfficiency(const char* name, const char* title, Int_t bins, Double_t xmin, Double_t xmax) : nbins_(bins), xmin_(xmin), xmax_(xmax), selected(0), all(0), efficiency(0) 
    {
      selected = new TH1F("SimpleEfficiency::selected","SimpleEfficiency::selected",bins,xmin,xmax);
      all = new TH1F("SimpleEfficiency::all","SimpleEfficiency::all",bins,xmin,xmax);
      efficiency = new TH1F(name,title,bins,xmin,xmax);
    }
  
  ~SimpleEfficiency() 
    {
      if (selected) delete selected;
      if (all) delete all;
      if (efficiency) delete efficiency;
    }

  void calculate() 
    {
      for (Int_t ibin = 0; ibin < nbins_; ibin++) {
	if (all->GetBinContent(ibin+1)) efficiency->SetBinContent(ibin+1,selected->GetBinContent(ibin+1)/all->GetBinContent(ibin+1));
	efficiency->SetBinError(ibin+1,defficiency(selected->GetBinContent(ibin+1)/all->GetBinContent(ibin+1),all->GetBinContent(ibin+1)));
      }
    }

  void select(Int_t bin, bool valid=true) 
    {
      all->AddBinContent(bin);
      if (valid) selected->AddBinContent(bin);
    }
  
  void select(Double_t value, bool valid=true) 
    {
      all->Fill(value);
      if (valid) selected->Fill(value);
    }
  
  TH1F* const get() {return efficiency;}
  
  Int_t bins() {return nbins_;}
  
  Double_t xmin() {return xmin_;}
  
  Double_t xmax() {return xmax_;}

  static bool comparable(SimpleEfficiency* one, SimpleEfficiency* two) 
    {
      if ((one->bins() == two->bins()) &&
	  (one->xmin() == two->xmin()) &&
	  (one->xmax() == two->xmax())) return true; 
      return false;
    }
  
  static void compare(TGraphErrors* result,SimpleEfficiency* one, SimpleEfficiency* two) 
    {
      if (!comparable(one,two) || result->GetN() != one->bins()) return;
      
      for (Int_t ipoint = 0; ipoint < result->GetN(); ipoint++) {
	
	result->GetX()[ipoint] = one->efficiency->GetBinContent(ipoint+1);
	result->GetY()[ipoint] = two->efficiency->GetBinContent(ipoint+1);
	result->GetEX()[ipoint] = one->efficiency->GetBinError(ipoint+1);
	result->GetEY()[ipoint] = two->efficiency->GetBinError(ipoint+1);
      }
    }
  
  static double defficiency(double efficiency, double total) {return (total) ? sqrt(efficiency * (1. - efficiency)/total) : 0.;}
  
 private:
  
  Int_t nbins_;
  Double_t xmin_;
  Double_t xmax_;
  TH1F* selected;
  TH1F* all;
  TH1F* efficiency;
  
  ClassDef(SimpleEfficiency,1)
    
};

#endif
