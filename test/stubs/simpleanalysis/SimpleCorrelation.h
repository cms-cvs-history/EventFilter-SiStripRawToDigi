#ifndef simpleanalysis_SimpleCorrelation_H
#define simpleanalysis_SimpleCorrelation_H


class SimpleCorrelation {

 public: 

  SimpleCorrelation() : one_(), two_() {}

  ~SimpleCorrelation() {}

  void update(double one,double two) 
    {
      one_.push_back(one);
      two_.push_back(two);
    }
  
  void clear() 
    {
      one_.clear();
      two_.clear();
    }

  double correlate()
    {
      double sumone=0.;
      double sumtwo=0.;
      double sum2one=0.;
      double sum2two=0.;
      double sum2onetwo=0.;

      for (unsigned int i = 0; i < one_.size(); i++) {
	sumone+=one_[i]; 
	sumtwo+=two_[i]; 
	sum2one+=(one_[i]*one_[i]); 
	sum2two+=(two_[i]*two_[i]); 
	sum2onetwo+=(one_[i]*two_[i]);
      }
      
      return (one_.size()*sum2onetwo - sumone*sumtwo)/sqrt((one_.size()*sum2one - sumone*sumone)*(two_.size()*sum2two - sumtwo*sumtwo)); 
    }
  
 private:

  std::vector<double> one_;
  std::vector<double> two_;
};

#endif
