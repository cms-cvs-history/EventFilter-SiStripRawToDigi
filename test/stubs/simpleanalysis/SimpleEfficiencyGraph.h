#ifndef simpleanalysis_SimpleEfficiencyGraph_H
#define simpleanalysis_SimpleEfficiencyGraph_H

class SimpleEfficiencyGraph : public TNamed {
  
public:
  
  SimpleEfficiencyGraph() : TNamed(), selected(0), all(0), efficiency(0), points_(0), xmin_(0), xmax_(0) 
    { 
      selected = new TGraph();
      all = new TGraph();
      efficiency = new TGraphErrors();
    }
  
  SimpleEfficiencyGraph(const char* name, const char* title, Int_t points, Double_t xmin, Double_t xmax) : TNamed(name,title), selected(0), all(0), efficiency(0), points_(points), xmin_(xmin), xmax_(xmax) 
    {
      selected = new TGraph(points);
      all = new TGraph(points);
      efficiency = new TGraphErrors(points);
      
      for (Int_t ipoint = 0; ipoint < points; ipoint++) {
	Double_t x = xmin + ipoint*(xmax-xmin)/(Double_t)points;
	selected->SetPoint(ipoint,x,0.);
	all->SetPoint(ipoint,x,0.);
	efficiency->SetPoint(ipoint,x,0.);
      }
    }
  
  ~SimpleEfficiencyGraph() 
    {
      if (selected) delete selected;
      if (all) delete all;
      if (efficiency) delete efficiency;
    }
  
  void calculate() 
    {
      for (Int_t ipoint = 0; ipoint < points_; ipoint++) {	
	if (all->GetY()[ipoint]) efficiency->GetY()[ipoint] = selected->GetY()[ipoint]/all->GetY()[ipoint]; 
	efficiency->GetEY()[ipoint] = SimpleEfficiency::defficiency(efficiency->GetY()[ipoint],all->GetY()[ipoint]);
      }
    }
  
  void select(Int_t ipoint, bool valid=true) 
    { 
      all->GetY()[ipoint]++;
      if (valid) selected->GetY()[ipoint]++;
    }
  
  TGraphErrors* const get() {return efficiency;}
  
  static void compare(TGraphErrors* result, SimpleEfficiencyGraph* one, SimpleEfficiencyGraph* two) 
    {
      if (!comparable(result,one) || !comparable(one,two)) return;
      
      for (Int_t ipoint = 0; ipoint < result->GetN(); ipoint++) {
	
	result->GetX()[ipoint] = one->efficiency->GetY()[ipoint];
	result->GetY()[ipoint] = two->efficiency->GetY()[ipoint];
	result->GetEX()[ipoint] = one->efficiency->GetEY()[ipoint];
	result->GetEY()[ipoint] = two->efficiency->GetEY()[ipoint];
      }
    }
  
  
  static bool comparable(SimpleEfficiencyGraph* one, SimpleEfficiencyGraph* two) 
    { 
      if ((one->points() == two->points()) &&
	  (one->xmin() == two->xmin()) &&
	  (one->xmax() == two->xmax())) return true;
      return false;
    }
  
  static bool comparable(TGraphErrors* graph, SimpleEfficiencyGraph* efficiency) 
    {
      if (graph->GetN() == efficiency->points())
	return true;
      return false;
    }
  
  Double_t cut(Int_t ipoint) {return xmin_ + ipoint*(xmax_-xmin_)/points_;}
  
  Int_t point(Double_t cut) {return (Int_t)((cut - xmin_)*points_/(xmax_-xmin_) + .5);}
  
  Int_t points() {return points_;}
  
  Double_t xmin() {return xmin_;}
  
  Double_t xmax() {return xmax_;}
  
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
