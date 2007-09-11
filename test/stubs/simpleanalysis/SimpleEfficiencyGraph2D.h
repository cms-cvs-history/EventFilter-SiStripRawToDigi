#ifndef simpleanalysis_SimpleEfficiencyGraph2D_H
#define simpleanalysis_SimpleEfficiencyGraph2D_H

class SimpleEfficiencyGraph2D : public TNamed {

 public:

  /** Constructors */

  SimpleEfficiencyGraph2D() : TNamed(), selected_(0), all_(0), efficiency_(0), xpoints_(0), ypoints_(0), xmin_(0), xmax_(0), ymin_(0), ymax_(0) 
  { 
    selected_ = new TGraph2D();
    all_ = new TGraph2D();
    efficiency_ = new TGraph2DErrors();
  }

  SimpleEfficiencyGraph2D(const char* name, const  char* title, Int_t xpoints, Int_t ypoints, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax) : TNamed(name,title), selected_(0), all_(0), efficiency_(0), xpoints_(xpoints), ypoints_(ypoints), xmin_(xmin), xmax_(xmax), ymin_(ymin), ymax_(ymax)
  {
    selected_ = new TGraph2D(xpoints_*ypoints_);
    all_ = new TGraph2D(xpoints_*ypoints_);
    efficiency_ = new TGraph2DErrors(xpoints_*ypoints_);
    
    for (int iypoint = 0; iypoint < ypoints_; iypoint++) {
      for (int ixpoint = 0; ixpoint < xpoints_; ixpoint++) {
	
	double x = xmin + ixpoint*(xmax-xmin)/static_cast<double>(xpoints_);
	double y = ymin + iypoint*(ymax-ymin)/static_cast<double>(ypoints_);
	selected_->SetPoint(iypoint*xpoints_+ixpoint,x,y,0.);
	all_->SetPoint(iypoint*xpoints_+ixpoint,x,y,0.);
	efficiency_->SetPoint(iypoint*xpoints_+ixpoint,x,y,0.);
      }
    }
  }

  /** Desctructor */

  ~SimpleEfficiencyGraph2D() 
  {
    if (selected_) delete selected_;
    if (all_) delete all_;
    if (efficiency_) delete efficiency_;
  }

  /** Calculate final efficiency */

  void calculate() 
    {
      for (int iypoint = 0; iypoint < ypoints_; iypoint++) {
	for (int ixpoint = 0; ixpoint < xpoints_; ixpoint++) {	  
	  double selected = selected_->GetZ()[iypoint*xpoints_+ixpoint];
	  double all = all_->GetZ()[iypoint*xpoints_+ixpoint];
	  if (all > 0.) { 
	    double z = selected/all;
	    efficiency_->GetZ()[iypoint*xpoints_+ixpoint] = z;
	    efficiency_->GetEZ()[iypoint*xpoints_+ixpoint] = SimpleEfficiency::defficiency(z,all);  
	  }   
	}
      }
    }
  
  /** Selection */
  
  void select(int ixpoint, int iypoint, bool valid=true) 
    {
      all_->GetZ()[iypoint*xpoints_+ixpoint]++;
      if (valid) selected_->GetZ()[iypoint*xpoints_+ixpoint]++;
    }

  /** Getter */

  TGraph2DErrors* const get() {return efficiency_;}

  /** Utility */

  double xcut(Int_t ixpoint) {return xmin_ + ixpoint*(xmax_-xmin_)/xpoints_;}
  
  double ycut(Int_t iypoint) {return ymin_ + iypoint*(ymax_-ymin_)/ypoints_;}
  
  int xpoint(Double_t cut) {return (Int_t)((cut - xmin_)*xpoints_/(xmax_-xmin_) + .5);}
  
  int ypoint(Double_t cut) {return (Int_t)((cut - ymin_)*ypoints_/(ymax_-ymin_) + .5);}
  
  int xpoints() {return xpoints_;}

  int ypoints() {return ypoints_;}

  double xmin() {return xmin_;}

  double xmax() {return xmax_;}

  double ymin() {return ymin_;}

  double ymax() {return ymax_;}

 private:
  
  TGraph2D* selected_;
  TGraph2D* all_;
  TGraph2DErrors* efficiency_;
  int xpoints_;
  int ypoints_;
  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;

  ClassDef(SimpleEfficiencyGraph2D,1)
    
};

#endif
