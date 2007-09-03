#ifndef simpleanalysis_SimplePurityGraph2D
#define simpleanalysis_SimplePurityGraph2D

class SimplePurityGraph2D : public TNamed {

 public:

  SimplePurityGraph2D() : TNamed(), significance_(0), purity_(0), xpoints_(0), ypoints_(0), xmin_(0), xmax_(0), ymin_(0), ymax_(0)
    { 
      significance_ = new TGraph2DErrors(xpoints_*ypoints_);
      purity_ = new TGraph2DErrors(xpoints_*ypoints_);
    }

  SimplePurityGraph2D(const char* name, const char* title, SimpleEfficiencyGraph2D* x, SimpleEfficiencyGraph2D* y, SimpleEfficiencyGraph2D* z, double cx, double cy, double cz) : TNamed(name,title), significance_(0), purity_(0), xpoints_(x->xpoints()), ypoints_(x->ypoints()), xmin_(x->xmin()), xmax_(x->xmax()), ymin_(x->ymin()), ymax_(x->ymax())
    {
      if (!SimpleEfficiencyGraph2D::comparable(x,y) || !SimpleEfficiencyGraph2D::comparable(y,z)) return;
      
      significance_ = new TGraph2DErrors(xpoints_*ypoints_);
      purity_ = new TGraph2DErrors(xpoints_*ypoints_);
      
      for (Int_t iypoint = 0; iypoint < ypoints_; iypoint++) {
	for (Int_t ixpoint = 0; ixpoint < xpoints_; ixpoint++) {	
	  Int_t ipoint = iypoint*xpoints_+ixpoint;
	  
	  purity_->SetPoint(ipoint, xcut(ixpoint), ycut(iypoint), SimplePurityGraph::purity(x->get()->GetZ()[ipoint], y->get()->GetZ()[ipoint], z->get()->GetZ()[ipoint], cz, cy, cz));
	  significance_->SetPoint(ipoint, xcut(ixpoint), ycut(iypoint), SimplePurityGraph::significance(x->get()->GetZ()[ipoint], y->get()->GetZ()[ipoint], z->get()->GetZ()[ipoint], cz, cy, cz));
	  purity_->SetPointError(ipoint, xcut(ixpoint), ycut(iypoint), SimplePurityGraph::dpurity(x->get()->GetY()[ipoint], y->get()->GetY()[ipoint], z->get()->GetY()[ipoint], x->get()->GetEY()[ipoint], y->get()->GetEY()[ipoint], z->get()->GetEY()[ipoint], cx, cy, cz));
	significance_->SetPointError(ipoint, xcut(ixpoint), ycut(iypoint), SimplePurityGraph::dsignificance(x->get()->GetY()[ipoint], y->get()->GetY()[ipoint], z->get()->GetY()[ipoint], x->get()->GetEY()[ipoint], y->get()->GetEY()[ipoint], z->get()->GetEY()[ipoint], cx, cy, cz)); 
	}
      }
    }

~SimplePurityGraph2D() 
{  
  if (purity_) delete purity_;
  if (significance_) delete significance_;
}

TGraph2DErrors* const purity() {return purity_;}

TGraph2DErrors* const significnace() {return significance_;}

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

  TGraph2DErrors* significance_;
  TGraph2DErrors* purity_;

  Int_t xpoints_;
  Int_t ypoints_;
  Double_t xmin_;
  Double_t xmax_;
  Double_t ymin_;
  Double_t ymax_;
  
  ClassDef(SimplePurityGraph2D,1)
    
};

#endif
