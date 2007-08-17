#ifndef simpleanalysis_SimpleOverlay_H
#define simpleanalysis_SimpleOverlay_H

class SimpleOverlay {
  
 public:

  SimpleOverlay(): tcanvas_(0), tlegend_(0), option_(""), legend_(true), color_(true), marker_(true), labels_()
    {
      tcanvas_ = new TCanvas("", "",200,10,700,500);
      tlegend_ = new TLegend(0.7,0.56,0.9,0.69);
    }
  
  ~SimpleOverlay() { delete tcanvas_; delete tlegend_; }

  /** getters */

  TCanvas* canvas() {return tcanvas_;}
  
  /** setters */

  void option(std::string option) {option_ = option;}
  
  void legend(bool set = true) {legend_ = set;}
  
  void color(bool set = true) {color_ = set;}

  void marker(bool set = true) {marker_ = set;}

  void label(std::vector<std::string> labels) {labels_ = labels;}

  void label(std::string one="", std::string two="", std::string three="", std::string four="", std::string five="") 
    {
      labels_.clear();
      labels_.push_back(one);
      labels_.push_back(two);
      labels_.push_back(three);
      labels_.push_back(four);
      labels_.push_back(five);
    }

 void loga(unsigned int base)
   {
     tcanvas_->SetLogx(base);
     tcanvas_->SetLogy(base);
   }

 void reset() 
   {
     tcanvas_->Clear();
     tlegend_->Clear();
     loga(0);
   }
  
 /** style */
 
 Int_t color(unsigned int index) {return index+1;}
 
 Int_t marker(unsigned int index) {return index+20;}

  /** overlay */

  void graphs(std::vector<TGraph*> tgraphs)
    {
      for (unsigned int i=0;i<tgraphs.size();i++) {
	tgraphs[i]->SetMarkerColor(color(i));
	tlegend_->AddEntry(tgraphs[i],labels_[i].c_str(),"p");
	(!i) ? tgraphs[i]->Draw(("AP"+option_).c_str()) : tgraphs[i]->Draw(("P"+option_).c_str());
      }
      if (legend_) tlegend_->Draw();
    }

  void graphs(TGraph* one, TGraph* two=0, TGraph* three=0, TGraph* four=0,TGraph* five=0) 
    {
      std::vector<TGraph*> tgraphs;
      if (one) tgraphs.push_back(one);
      if (two) tgraphs.push_back(two);
      if (three) tgraphs.push_back(three);
      if (four) tgraphs.push_back(four);
      if (five) tgraphs.push_back(five);
      graphs(tgraphs);
    }
  
  void graphs2D(TGraph2D* graph, std::string title="", std::string xtitle="", std::string ytitle="", std::string ztitle="") 
    {
      graph->Draw(option_.c_str());
      graph->SetTitle(title.c_str());
      graph->GetXaxis()->SetTitle(xtitle.c_str());
      graph->GetYaxis()->SetTitle(ytitle.c_str());
      graph->GetZaxis()->SetTitle(ztitle.c_str());
    }

  static bool StrictWeakOrdering(TH1* one, TH1* two) {return one->GetMaximum() > two->GetMaximum();}

  void histos(std::vector<TH1*> th1s)
    {
      std::sort(th1s.begin(),th1s.end(),StrictWeakOrdering);
      for (unsigned int i=0;i<th1s.size();i++) {
	th1s[i]->SetLineColor(color(i));
	th1s[i]->SetMarkerColor(color(i));
	tlegend_->AddEntry(th1s[i],labels_[i].c_str(),"l");
	(i) ? th1s[i]->Draw(option_.c_str()) : th1s[i]->Draw(("SAME"+option_).c_str());
      }
      if (legend_) tlegend_->Draw();
    }


  void histos(TH1* one, TH1* two=0, TH1* three=0, TH1* four=0, TH1* five=0) 
    {
      std::vector<TH1*> th1s;
      if (one) th1s.push_back(one);
      if (two) th1s.push_back(two);
      if (three) th1s.push_back(three);
      if (four) th1s.push_back(four);
      if (five) th1s.push_back(five);
      histos(th1s);
    }
  
  TMarker* marker(Double_t x,Double_t y,Int_t mark) 
    {
      TMarker* marker = new TMarker(x,y,mark);
      marker->Draw();
      return marker;
    }
  
/** save */

 void save(std::string name) 
   {
     tcanvas_->SetName(name.c_str());
     tcanvas_->Write(name.c_str(), TObject::kOverwrite);
   }
 
 private:
 
 TCanvas* tcanvas_;
 TLegend* tlegend_;
 std::string option_;
 bool legend_;
 bool color_;
 bool marker_;
 std::vector<std::string> labels_;
};

#endif
