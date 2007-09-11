#ifndef simpleanalysis_SimplePurityGraph2D
#define simpleanalysis_SimplePurityGraph2D

class SimplePurityGraph2D : public TNamed {

 public:

  /** Constructors */

  SimplePurityGraph2D() : TNamed(), significance_(0), purity_(0), xpoints_(0), ypoints_(0), xmin_(0), xmax_(0), ymin_(0), ymax_(0)
    { 
      significance_ = new TGraph2DErrors(xpoints_*ypoints_);
      purity_ = new TGraph2DErrors(xpoints_*ypoints_);
    }

  SimplePurityGraph2D(const char* name, const char* title, SimpleEfficiencyGraph2D* one, SimpleEfficiencyGraph2D* two, SimpleEfficiencyGraph2D* three, double cone, double ctwo, double cthree) : TNamed(name,title), significance_(0), purity_(0), xpoints_(one->xpoints()), ypoints_(one->ypoints()), xmin_(one->xmin()), xmax_(one->xmax()), ymin_(one->ymin()), ymax_(one->ymax())
    {
      
      significance_ = new TGraph2DErrors(xpoints_*ypoints_);
      purity_ = new TGraph2DErrors(xpoints_*ypoints_);
      
      for (int iypoint = 0; iypoint < ypoints_; iypoint++) {
	for (int ixpoint = 0; ixpoint < xpoints_; ixpoint++) {	
	  int ipoint = iypoint*xpoints_+ixpoint;
	  double zone = one->get()->GetZ()[ipoint];
	  double ztwo = two->get()->GetZ()[ipoint];
	  double zthree = three->get()->GetZ()[ipoint];
	  double done = one->get()->GetEZ()[ipoint];
	  double dtwo = two->get()->GetEZ()[ipoint];
	  double dthree = three->get()->GetEZ()[ipoint];
	  double x = xcut(ixpoint);
	  double y = ycut(iypoint);
	  purity_->SetPoint(ipoint,x,y,SimplePurityGraph::purity(zone,ztwo,zthree,cone,ctwo,cthree));
	  significance_->SetPoint(ipoint,x,y,SimplePurityGraph::significance(zone,ztwo,zthree,cone,ctwo,cthree));
	  purity_->SetPointError(ipoint,x,y,SimplePurityGraph::dpurity(zone,ztwo,zthree,done,dtwo,dthree,cone,ctwo,cthree));
	  significance_->SetPointError(ipoint,x,y,SimplePurityGraph::dsignificance(zone,ztwo,zthree,done,dtwo,dthree,cone,ctwo,cthree));
	}
      }
    }
  
  /** Destructor */
  
  ~SimplePurityGraph2D() 
    {  
      if (purity_) delete purity_;
      if (significance_) delete significance_;
    }
  
  /** Getters */
  
  TGraph2DErrors* const purity() {return purity_;}

  TGraph2DErrors* const significnace() {return significance_;}
  
  /** Utility */
  double xcut(int ixpoint) {return xmin_ + ixpoint*(xmax_-xmin_)/xpoints_;}
  
  double ycut(int iypoint) {return ymin_ + iypoint*(ymax_-ymin_)/ypoints_;}
  
  int xpoint(double cut) {return static_cast<int>((cut - xmin_)*xpoints_/(xmax_-xmin_) + .5);}
  
  int ypoint(double cut) {return static_cast<int>((cut - ymin_)*ypoints_/(ymax_-ymin_) + .5);}
  
  int xpoints() {return xpoints_;}
  
  int ypoints() {return ypoints_;}
  
  double xmin() {return xmin_;}
  
  double xmax() {return xmax_;}
  
  double ymin() {return ymin_;}
  
  double ymax() {return ymax_;}
  
 private:
  
  TGraph2DErrors* significance_;
  TGraph2DErrors* purity_;

  int xpoints_;
  int ypoints_;
  double xmin_;
  double xmax_;
  double ymin_;
  double ymax_;
  
  ClassDef(SimplePurityGraph2D,1)
    
};

#endif
