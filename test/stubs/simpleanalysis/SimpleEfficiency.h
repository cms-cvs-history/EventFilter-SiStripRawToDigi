#ifndef simpleanalysis_SimpleEfficiency_H
#define simpleanalysis_SimpleEfficiency_H

class SimpleEfficiency : public TNamed {

 public:
  
  /** Constructors */

  SimpleEfficiency() : TNamed(), nbins_(0), xmin_(0), xmax_(0), selected_(0), all_(0), efficiency_(0) 
    { 
      selected_ = new TH1F();
      all_ = new TH1F();
      efficiency_ = new TH1F();
    }

  SimpleEfficiency(const char* name, const char* title, Int_t bins, Double_t xmin, Double_t xmax) : TNamed(name,title), nbins_(bins), xmin_(xmin), xmax_(xmax), selected_(0), all_(0), efficiency_(0) 
    {
      selected_ = new TH1F("SimpleEfficiency::selected","SimpleEfficiency::selected",bins,xmin,xmax);
      all_ = new TH1F("SimpleEfficiency::all","SimpleEfficiency::all",bins,xmin,xmax);
      efficiency_ = new TH1F(name,title,bins,xmin,xmax);
    }
  
  /** Destructor */

  ~SimpleEfficiency() 
    {
      if (selected_) delete selected_;
      if (all_) delete all_;
      if (efficiency_) delete efficiency_;
    }

  /** Calculate final efficiency */

  void calculate() 
    {
      for (int ibin = 0; ibin < nbins_; ibin++) {
	double all = all_->GetBinContent(ibin+1);
	double selected = selected_->GetBinContent(ibin+1)/all;
	if (all) {
	  efficiency_->SetBinContent(ibin+1,selected);
	  efficiency_->SetBinError(ibin+1,defficiency(selected,all));
	}
      }
    }

  /** Selection */

  void select(int bin, bool valid=true) 
    {
      all_->AddBinContent(bin);
      if (valid) selected_->AddBinContent(bin);
    }
  
  void select(double value, bool valid=true) 
    {
      all_->Fill(value);
      if (valid) selected_->Fill(value);
    }
  
  /** Comparison */

  static TGraphErrors* compare(SimpleEfficiency* one, SimpleEfficiency* two) 
    {
      TGraphErrors* result = new TGraphErrors(one->bins()); 
      for (Int_t ipoint = 0; ipoint < result->GetN(); ipoint++) {
	result->GetX()[ipoint] = one->efficiency_->GetBinContent(ipoint+1);
	result->GetY()[ipoint] = two->efficiency_->GetBinContent(ipoint+1);
	result->GetEX()[ipoint] = one->efficiency_->GetBinError(ipoint+1);
	result->GetEY()[ipoint] = two->efficiency_->GetBinError(ipoint+1);
      }
      return result;
    }

  /** Divide */

  void divide(SimpleEfficiency* denom) {efficiency_->Divide(denom->get());}

  /** Getter */

  TH1F* const get() {return efficiency_;}

  /** Utility */
  
  int bins() {return nbins_;}
  
  double xmin() {return xmin_;}
  
  double xmax() {return xmax_;}
  
  static double defficiency(double efficiency, double total) {return (total) ? sqrt(efficiency * (1. - efficiency)/total) : 0.;}
  
 private:
  
  int nbins_;
  double xmin_;
  double xmax_;
  TH1F* selected_;
  TH1F* all_;
  TH1F* efficiency_;
  
  ClassDef(SimpleEfficiency,1)
    
};

#endif
