#ifndef EventFilter_SiStripRawToDigi_Utility_H
#define EventFilter_SiStripRawToDigi_Utility_H

#include "DataFormats/SiStripCommon/interface/ConstantsForDqm.h"
#include "TDirectory.h"
#include "TH1F.h"
#include <vector>
#include <string>

TDirectory* addDir(TDirectory* currentDir,std::string path) {

 std::vector<std::string> directories; 
  std::string::const_iterator it = path.begin(), previous_dir = path.begin(),latest_dir = path.begin(); 

 if (*(path.begin()) != sistrip::dir_) {it--; previous_dir--; latest_dir--;}

 while (it != path.end()) {
   it++;
   if (*it == sistrip::dir_) {
     previous_dir = latest_dir; latest_dir = it;
     directories.push_back(std::string(previous_dir+1, latest_dir));}}

 if (latest_dir != (path.end()-1)) {
   previous_dir = latest_dir;
   latest_dir = path.end();
   directories.push_back(std::string(previous_dir+1, latest_dir));}
 
 TDirectory* child = currentDir;
 std::vector<std::string>::const_iterator dir = directories.begin();
 for (; dir != directories.end(); dir++) {
   if (!child->GetDirectory(dir->c_str())) child = child->mkdir(dir->c_str());
   else {child = child->GetDirectory(dir->c_str());}}
 return child;
}

bool equal(const double& one,const double& two) {
  if (fabs(one - two) < 0.001) return true;
  return false;
}

void normalise(TH1F* h) {
 if (h->GetSumOfWeights() > 0.)  h->Scale(1./h->GetSumOfWeights());
}

void normalise(TH1F* h,double scale) {
 if (scale > 0.)  h->Scale(1./scale);
}

double dEfficiency(double efficiency, double ntot) {

  if (ntot) return sqrt(efficiency * (1. - efficiency)/ntot);
  return 0.;
}

double dSoSplusB(Double_t signal,
		 Double_t b1,
		 Double_t b2,
		 Double_t dsignal,
		 Double_t db1,
		 Double_t db2,
		 Double_t signalCS,
		 Double_t b1CS,
		 Double_t b2CS) {
  
  double sig = signal*signalCS;
  double bkgd = b1*b1CS + b2*b2CS;
  double denom = (sig + bkgd) * (sig + bkgd);
  
  if ((sig + bkgd) > 0.) {
    return sqrt((signalCS * signalCS * signal * dsignal) * 
		(signalCS * signalCS * signal * dsignal)/(denom*denom) + 
		(signalCS * b1CS * signal * db1) * (signalCS * b1CS * signal * db1) / (denom*denom)  + 
		(signalCS * b2CS * signal * db2) * (signalCS * b2CS * signal * db2) / (denom*denom));}
  
  else return 0.;
}

double dSignificance() { return 0.;}

#endif
