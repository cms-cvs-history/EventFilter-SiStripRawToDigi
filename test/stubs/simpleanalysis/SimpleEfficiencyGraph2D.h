#ifndef simpleanalysis_SimpleEfficiencyGraph2D_H
#define simpleanalysis_SimpleEfficiencyGraph2D_H

class SimpleEfficiencyGraph2D : public TObject {

 public:

  SimpleEfficiencyGraph2D() : selected_(0), all_(0), efficiency_(0), xpoints_(0), ypoints_(0), xmin_(0), xmax_(0), ymin_(0), ymax_(0) 
  { 
    selected_ = new TGraph2D();
    all_ = new TGraph2D();
    efficiency_ = new TGraph2DErrors();
  }

  SimpleEfficiencyGraph2D(Int_t xpoints, Int_t ypoints, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax) : selected_(0), all_(0), efficiency_(0), xpoints_(xpoints), ypoints_(ypoints), xmin_(xmin), xmax_(xmax), ymin_(ymin), ymax_(ymax)
  {
    selected_ = new TGraph2D(xpoints_*ypoints_);
    all_ = new TGraph2D(xpoints_*ypoints_);
    efficiency_ = new TGraph2DErrors(xpoints_*ypoints_);
    
    for (Int_t iypoint = 0; iypoint < ypoints_; iypoint++) {
      for (Int_t ixpoint = 0; ixpoint < xpoints_; ixpoint++) {
	
	Double_t x = xmin + ixpoint*(xmax-xmin)/(Double_t)xpoints_;
	Double_t y = ymin + iypoint*(ymax-ymin)/(Double_t)ypoints_;
	
	selected_->SetPoint(iypoint*xpoints_+ixpoint,x,y,0.);
	all_->SetPoint(iypoint*xpoints_+ixpoint,x,y,0.);
	efficiency_->SetPoint(iypoint*xpoints_+ixpoint,x,y,0.);
      }
    }
  }

  ~SimpleEfficiencyGraph2D() 
  {
    if (selected_) delete selected_;
    if (all_) delete all_;
    if (efficiency_) delete efficiency_;
  }

  void calculate() 
    {
      for (Int_t iypoint = 0; iypoint < ypoints_; iypoint++) {
	for (Int_t ixpoint = 0; ixpoint < xpoints_; ixpoint++) {
	  
	  if (all_->GetZ()[iypoint*xpoints_+ixpoint]) efficiency_->GetZ()[iypoint*xpoints_+ixpoint] = selected_->GetZ()[iypoint*xpoints_+ixpoint]/ all_->GetZ()[iypoint*xpoints_+ixpoint];
	  efficiency_->GetEZ()[iypoint*xpoints_+ixpoint] = SimpleEfficiency::defficiency(efficiency_->GetZ()[iypoint*xpoints_+ixpoint], all_->GetZ()[iypoint*xpoints_+ixpoint]);     
	}
      }
    }
  
  void select(Int_t ixpoint,Int_t iypoint,bool valid=true) 
    {
      all_->GetZ()[iypoint*xpoints_+ixpoint]++;
      if (valid) selected_->GetZ()[iypoint*xpoints_+ixpoint]++;
    }

  TGraph2DErrors* const get() {return efficiency_;}
  
  static bool comparable(SimpleEfficiencyGraph2D* one, SimpleEfficiencyGraph2D* two) 
    {    
      if ((one->xpoints() == two->xpoints()) &&
	  (one->ypoints() == two->ypoints()) &&
	  (one->xmin() == two->xmin()) &&
	  (one->xmax() == two->xmax()) &&
	  (one->ymin() == two->ymin()) &&
	  (one->ymax() == two->ymax())) return true;
    return false;
    }
  

  Double_t xcut(Int_t ixpoint) {return xmin_ + ixpoint*(xmax_-xmin_)/xpoints_;}
  
  Double_t ycut(Int_t iypoint) {return ymin_ + iypoint*(ymax_-ymin_)/ypoints_;}
  
  Int_t xpoint(Double_t cut) {return (Int_t)((cut - xmin_)*xpoints_/(xmax_-xmin_) + .5);}
  
  Int_t ypoint(Double_t cut) {return (Int_t)((cut - ymin_)*ypoints_/(ymax_-ymin_) + .5);}
  
  Int_t xpoints() {return xpoints_;}

  Int_t ypoints() {return ypoints_;}

  Double_t xmin() {return xmin_;}

  Double_t xmax() {return xmax_;}

  Double_t ymin() {return ymin_;}

  Double_t ymax() {return ymax_;}

 private:
  
  TGraph2D* selected_;
  TGraph2D* all_;
  TGraph2DErrors* efficiency_;
  Int_t xpoints_;
  Int_t ypoints_;
  Double_t xmin_;
  Double_t xmax_;
  Double_t ymin_;
  Double_t ymax_;

  ClassDef(SimpleEfficiencyGraph2D,1)
    
};

#endif
