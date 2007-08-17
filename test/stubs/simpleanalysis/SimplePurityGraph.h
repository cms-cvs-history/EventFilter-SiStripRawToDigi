#ifndef simpleanalysis_SimplePurityGraph_H
#define simpleanalysis_SimplePurityGraph_H

class SimplePurityGraph : public TObject {
  
 public:

  SimplePurityGraph() : significance_(0), purity_(0), points_(0), xmin_(0),xmax_(0)
  { 
    significance_ = new TGraphErrors();
    purity_ = new TGraphErrors();
  }
  
  SimplePurityGraph(SimpleEfficiencyGraph* x, SimpleEfficiencyGraph* y, SimpleEfficiencyGraph* z, double cx, double cy, double cz) : significance_(0), purity_(0), points_(x->points()), xmin_(x->xmin()), xmax_(x->xmax())
  {
    if (!SimpleEfficiencyGraph::comparable(x,y) || !SimpleEfficiencyGraph::comparable(y,z)) return;
    
    purity_ = new TGraphErrors(points_);
    significance_ = new TGraphErrors(points_);
    
    for (Int_t ipoint = 0; ipoint < points_; ipoint++) {
      
      purity_->SetPoint(ipoint,cut(ipoint),purity(x->get()->GetY()[ipoint], y->get()->GetY()[ipoint], z->get()->GetY()[ipoint],cx, cy, cz));
      purity_->SetPointError(ipoint,cut(ipoint),dpurity(x->get()->GetY()[ipoint], y->get()->GetY()[ipoint], z->get()->GetY()[ipoint], x->get()->GetEY()[ipoint], y->get()->GetEY()[ipoint], z->get()->GetEY()[ipoint], cx, cy, cz));
      significance_->SetPoint(ipoint,cut(ipoint),significance(x->get()->GetY()[ipoint], y->get()->GetY()[ipoint], z->get()->GetY()[ipoint],cx, cy, cz));
      significance_->SetPointError(ipoint,cut(ipoint),dsignificance(x->get()->GetY()[ipoint], y->get()->GetY()[ipoint], z->get()->GetY()[ipoint], x->get()->GetEY()[ipoint], y->get()->GetEY()[ipoint], z->get()->GetEY()[ipoint], cx, cy, cz));
    }
  }
  
  ~SimplePurityGraph() 
  {
    if (purity_) delete purity_;
    if (significance_) delete significance_;
  }
  
  TGraphErrors* const purity() {return purity_;}
  
  TGraphErrors* const significnace() {return significance_;}
  
  static void compare(TGraphErrors* result, SimpleEfficiencyGraph* efficiency, SimplePurityGraph* purity) 
    {
      if (!comparable(result,purity) || !comparable(efficiency,purity)) return;
   
      for (Int_t ipoint = 0; ipoint < result->GetN(); ipoint++) {
	result->GetX()[ipoint] = efficiency->get()->GetY()[ipoint];
	result->GetY()[ipoint] = purity->purity()->GetY()[ipoint];
	result->GetEX()[ipoint] = efficiency->get()->GetEY()[ipoint];
	result->GetEY()[ipoint] = purity->purity()->GetEY()[ipoint];
      }
    }
  
  
  static bool comparable(SimpleEfficiencyGraph* efficiency, SimplePurityGraph* purity) 
    { 
      if ((efficiency->points() == purity->points()) &&
	  (efficiency->xmin() == purity->xmin()) &&
	  (efficiency->xmax() == purity->xmax())) return true;
      return false;
    }
  
  static bool comparable(TGraphErrors* graph, SimplePurityGraph* purity) 
    {
      if (graph->GetN() == purity->points()) return true;
      return false;
    }
  
  static bool comparable(SimplePurityGraph* one, SimplePurityGraph* two) 
    {
      if ((one->points() == two->points()) &&
	  (one->xmin() == two->xmin()) &&
	  (one->xmax() == two->xmax())) return true;
      return false;
    }
  
  Double_t cut(Int_t ipoint) {return xmin_ + ipoint*(xmax_-xmin_)/points_;}
  
  Int_t point(Double_t cut) {return (Int_t)((cut - xmin_)*points_/(xmax_-xmin_) + .5);}
  
  Int_t points() {return points_;}
  
  Double_t xmin() {return xmin_;}

  Double_t xmax() {return xmax_;}
  
  static double purity(double x, double y, double z, double cx, double cy, double cz) {return ((x * cx) + (y * cy) + (z * cz) > 0.) ? x * cx / ((x * cx) + (y * cy) + (z * cz)) : 0.;} 
  
  static double significance(double x, double y, double z, double cx, double cy, double cz) {return ((y * cy) + (z * cz) > 0.) ?  x * cx / sqrt((y * cy) + (z * cz)) : 0.;}
  
  static double dpurity(double x, double y, double z, double dx, double dy, double dz, double cx, double cy, double cz) 
    {
      double denom = (x*cx + (y*cy + z*cz)) * (x*cx + (y*cy + z*cz));
      return ((denom) > 0.) ? sqrt((cx * cx * x * dx) * (cx * cx * x * dx)/(denom*denom) + (cx * cy * x * dy) * (cx * cy * x * dy) / (denom*denom)  + (cx * cz * x * dz) * (cx * cz * x * dz) / (denom*denom)) : 0.;
    }
  
  static double dsignificance(double x, double y, double z, double dx, double dy, double dz, double cx, double cy, double cz) { return 0.;}
  
 private:
  
  TGraphErrors* significance_;
  TGraphErrors* purity_;
  Int_t points_;
  Double_t xmin_;
  Double_t xmax_;
  
  ClassDef(SimplePurityGraph,1)
    
};
    
#endif
