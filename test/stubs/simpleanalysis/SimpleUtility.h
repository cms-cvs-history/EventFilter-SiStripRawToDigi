#ifndef simpleanalysis_SimpleUtility_H
#define simpleanalysis_SimpleUtility_H

namespace utility {
  
  const std::vector<std::string> path(std::string name) {
    
    std::vector<std::string> names; 
    std::string::const_iterator first = name.begin()-1; 
    std::string::const_iterator second = name.begin()-1;
    
    for (std::string::const_iterator ichar = name.begin(); ichar != name.end(); ichar++) {
      if (ichar == name.end()-1 || *(ichar+1) == std::string("/")) {
	first = second+1; 
	second = ichar+1;
	names.push_back(std::string(first, second));
      }
    }
    return names;
  }
  
  TDirectory* addDir(TDirectory* current,std::string pathname) {
    
    std::vector<std::string> names = path(pathname);
    TDirectory* child = current;
    for (std::vector<std::string>::const_iterator name = names.begin(); name != names.end(); name++) {
      if (child->GetDirectory(name->c_str())) child = child->GetDirectory(name->c_str());
      else child = child->mkdir(name->c_str());
    }
    return child;
  }
  
  const std::vector<TH1*> th1s(TDirectory* directory) {
    
    std::vector<TH1*> th1s;
    if (!directory) return th1s;
    TList* list = directory->GetListOfKeys();
    TObject* obj = list->First();
    while (obj) { 
      TH1* th1 = dynamic_cast<TH1*>(directory->Get(obj->GetName()));
      if (th1) th1s.push_back(th1);
      obj = list->After(obj);
    }  
    return th1s;
  }

  const std::vector<TTree*> ttrees(TDirectory* directory) {
    
    std::vector<TTree*> ttrees;
    if (!directory) return ttrees;
    TList* list = directory->GetListOfKeys();
    TObject* obj = list->First();
    while (obj) { 
      TTree* ttree = dynamic_cast<TTree*>(directory->Get(obj->GetName()));
      if (ttree) {
	ttree->SetName(obj->GetName());
	ttrees.push_back(ttree);
      }
      obj = list->After(obj);
    }  
    return ttrees;
  }
  
  const std::vector<TDirectory*> directories(TDirectory* directory) {
    
    std::vector<TDirectory*> dirs;
    if (!directory) return dirs;
    TList* list = directory->GetListOfKeys();
    TObject* obj = list->First();
    while (obj) { 
      TDirectory* child = dynamic_cast<TDirectory*>(directory->Get(obj->GetName()));
      if (child) dirs.push_back(child);
      obj = list->After(obj);
    }  
    return dirs;
  }
  
  void save(TDirectory* directory, std::string path = ".", std::string format="pdf") {

    const std::vector<TDirectory*> dirs = directories(directory);
    for (unsigned short i=0;i<dirs.size();i++) {
      TList* list = dirs[i]->GetListOfKeys();
      TObject* obj = list->First();
      while (obj) { 
	TCanvas* canvas = dynamic_cast<TCanvas*>(dirs[i]->Get(obj->GetName()));
	if (canvas) {
	  std::string name = path+"/"+canvas->GetName()+"."+format;
	  canvas->SaveAs(name.c_str());
	}
	obj = list->After(obj);
      }
    }
  }
  
  const bool equal(const double& one,const double& two) {
    if (fabs(one - two) < 0.001) return true;
    return false;
  }
  
  void normalise(TH1F* h) {
    if (h->GetSumOfWeights() > 0.)  h->Scale(1./h->GetSumOfWeights());
  }
  
  void normalise(TH1F* h,double scale) {
    if (scale > 0.)  h->Scale(1./scale);
  }
  
}

std::ostream& operator<<(std::ostream& os, const SimpleTrigger& trigger) {
  std::stringstream ss;
  for (unsigned int i=0;i<trigger.size();i++) {
    ss << "|" << trigger.get(i) << "|";
  } 
  os << ss.str();
  return os;
}

#endif
