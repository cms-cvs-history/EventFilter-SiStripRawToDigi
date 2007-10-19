#ifndef simpleanalysis_SimplePurityGraph_H
#define simpleanalysis_SimplePurityGraph_H

class SimplePurityGraph : public TNamed {
  
 public:

  /** Constructors */

  SimplePurityGraph() : TNamed(), significance_(0), purity_(0), points_(0), xmin_(0), xmax_(0)
  { 
    significance_ = new TGraphErrors();
    purity_ = new TGraphErrors();
  }

  SimplePurityGraph(const char* name, const char* title, SimpleEfficiencyGraph* one, SimpleEfficiencyGraph* two, SimpleEfficiencyGraph* three, double cone, double ctwo, double cthree) : TNamed(name,title), significance_(0), purity_(0), points_(one->points()), xmin_(one->xmin()), xmax_(one->xmax())
  {    
    purity_ = new TGraphErrors(points_);
    significance_ = new TGraphErrors(points_);
    
    for (int ipoint = 0; ipoint < points_; ipoint++) {
      double yone = one->get()->GetY()[ipoint];
      double ytwo = two->get()->GetY()[ipoint];
      double ythree = three->get()->GetY()[ipoint];
      double done = one->get()->GetEY()[ipoint];
      double dtwo = two->get()->GetEY()[ipoint];
      double dthree = three->get()->GetEY()[ipoint];
      double x = cut(ipoint);
      purity_->SetPoint(ipoint,x,purity(yone,ytwo,ythree,cone,ctwo,cthree));
      purity_->SetPointError(ipoint,x,dpurity(yone,ytwo,ythree,done,dtwo,dthree,cone,ctwo,cthree));
      significance_->SetPoint(ipoint,x,significance(yone,ytwo,ythree,cone,ctwo,cthree));
      significance_->SetPointError(ipoint,x,dsignificance(yone,ytwo,ythree,done,dtwo,dthree,cone,ctwo,cthree));
    }
  }
  
  /** Destructor */

  ~SimplePurityGraph() 
  {
    if (purity_) delete purity_;
    if (significance_) delete significance_;
  }
  
  /** Getters */

  TGraphErrors* const purity() {return purity_;}
  
  TGraphErrors* const significance() {return significance_;}
  
  /** Comparison */

  static TGraphErrors* compare(SimpleEfficiencyGraph* efficiency, SimplePurityGraph* purity) 
    {
      TGraphErrors* result = new TGraphErrors(efficiency->get()->GetN());
      for (int ipoint = 0; ipoint < efficiency->get()->GetN(); ipoint++) {
	result->GetX()[ipoint] = efficiency->get()->GetY()[ipoint];
	result->GetY()[ipoint] = purity->purity()->GetY()[ipoint];
	result->GetEX()[ipoint] = efficiency->get()->GetEY()[ipoint];
	result->GetEY()[ipoint] = purity->purity()->GetEY()[ipoint];
      }
      return result;
    }
  
  /** Utility */

  double cut(Int_t ipoint) {return xmin_ + ipoint*(xmax_-xmin_)/points_;}
  
  int point(Double_t cut) {return (Int_t)((cut - xmin_)*points_/(xmax_-xmin_) + .5);}
  
  int points() {return points_;}
  
  double xmin() {return xmin_;}

  double xmax() {return xmax_;}
  
  static double purity(double one, double two, double three, double cone, double ctwo, double cthree) 
    {
      return ((one * cone) + (two * ctwo) + (three * cthree) > 0.) ? one * cone / ((one * cone) + (two * ctwo) + (three * cthree)) : 0.;
    } 
  
  static double significance(double one, double two, double three, double cone, double ctwo, double cthree) 
    {
      return ((two * ctwo) + (three * cthree) > 0.) ?  one * cone / sqrt((two * ctwo) + (three * cthree)) : 0.;
    }
  
  static double dpurity(double yone, double ytwo, double ythree, double done, double dtwo, double dthree, double cone, double ctwo, double cthree) 
    {
      double denom = (yone*cone + (ytwo*ctwo + ythree*cthree)) * (yone*cone + (ytwo*ctwo + ythree*cthree));
      return ((denom) > 0.) ? sqrt((cone * cone * yone * done) * (cone * cone * yone * done)/(denom*denom) + (cone * ctwo * yone * dtwo) * (cone * ctwo * yone * dtwo) / (denom*denom)  + (cone * cthree * yone * dthree) * (cone * cthree * yone * dthree) / (denom*denom)) : 0.;
    }
  
  static double dsignificance(double yone, double ytwo, double ythree, double done, double dtwo, double dthree, double cone, double ctwo, double cthree) 
    { 
      //@@ Add implementation!
      return yone*ytwo*ythree*done*dtwo*dthree*cone*ctwo*cthree*0.;
    }
  
 private:
  
  TGraphErrors* significance_;
  TGraphErrors* purity_;
  Int_t points_;
  Double_t xmin_;
  Double_t xmax_;
  
  ClassDef(SimplePurityGraph,1)
    
};
    
#endif
