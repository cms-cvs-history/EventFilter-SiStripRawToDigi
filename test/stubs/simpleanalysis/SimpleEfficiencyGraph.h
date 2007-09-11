#ifndef simpleanalysis_SimpleEfficiencyGraph_H
#define simpleanalysis_SimpleEfficiencyGraph_H

class SimpleEfficiencyGraph : public TNamed {
  
public:
  
  /** Constructors */

  SimpleEfficiencyGraph() : TNamed(), selected(0), all(0), efficiency(0), points_(0), xmin_(0), xmax_(0) 
    { 
      selected = new TGraph();
      all = new TGraph();
      efficiency = new TGraphErrors();
    }
  
  SimpleEfficiencyGraph(const char* name, const char* title, int points, double xmin, double xmax) : TNamed(name,title), selected(0), all(0), efficiency(0), points_(points), xmin_(xmin), xmax_(xmax) 
    {
      selected = new TGraph(points);
      all = new TGraph(points);
      efficiency = new TGraphErrors(points);
      
      for (int ipoint = 0; ipoint < points; ipoint++) {
	double x = xmin + ipoint*(xmax-xmin)/static_cast<double>(points);
	selected->SetPoint(ipoint,x,0.);
	all->SetPoint(ipoint,x,0.);
	efficiency->SetPoint(ipoint,x,0.);
      }
    }
  
  /** Destructor */

  ~SimpleEfficiencyGraph() 
    {
      if (selected) delete selected;
      if (all) delete all;
      if (efficiency) delete efficiency;
    }
  
  /** Calculate final efficiency */

  void calculate() 
    {
      for (int ipoint = 0; ipoint < points_; ipoint++) {	
	if (all->GetY()[ipoint]) efficiency->GetY()[ipoint] = selected->GetY()[ipoint]/all->GetY()[ipoint]; 
	efficiency->GetEY()[ipoint] = SimpleEfficiency::defficiency(efficiency->GetY()[ipoint],all->GetY()[ipoint]);
      }
    }
  
  /** Selection */

  void select(int ipoint, bool valid=true) 
    { 
      all->GetY()[ipoint]++;
      if (valid) selected->GetY()[ipoint]++;
    }
  
  /** Getter */

  TGraphErrors* const get() {return efficiency;}
  
  /** Comparison */

  static TGraphErrors* compare(SimpleEfficiencyGraph* one, SimpleEfficiencyGraph* two) 
    { 
      TGraphErrors* result = new TGraphErrors(one->get()->GetN());
      for (int ipoint = 0; ipoint < one->get()->GetN(); ipoint++) {
	result->GetX()[ipoint] = one->efficiency->GetY()[ipoint];
	result->GetY()[ipoint] = two->efficiency->GetY()[ipoint];
	result->GetEX()[ipoint] = one->efficiency->GetEY()[ipoint];
	result->GetEY()[ipoint] = two->efficiency->GetEY()[ipoint];
      }
      return result;
    }
  
  /** Utility */
  
  double cut(Int_t ipoint) {return xmin_ + ipoint*(xmax_-xmin_)/points_;}
  
  int point(Double_t cut) {return (Int_t)((cut - xmin_)*points_/(xmax_-xmin_) + .5);}
  
  int points() {return points_;}
  
  double xmin() {return xmin_;}
  
  double xmax() {return xmax_;}
  
 private:
  
  TGraph* selected;
  TGraph* all;
  TGraphErrors* efficiency;
  Int_t points_;
  Double_t xmin_;
  Double_t xmax_;
  
  ClassDef(SimpleEfficiencyGraph,1)
};
    
#endif
