{

TFile* file = new TFile("file:/tmp/pwing/SiStripPerformance.root","UPDATE");

PerformanceAnalysis one(file,"");
one.book(); one.format(); one.analyze(); one.save(); one.unbook();

PerformanceAnalysis two(file,"");
two.book(); two.format(); two.analyze(); two.save(); two.unbook();

PerformanceAnalysis three(file,""); 
three.book(); three.format(); three.analyze(); three.save(); three.unbook();

PerformanceAnalysis one(file,"");
four.book(); four.format(); four.analyze(); four.save(); four.unbook();

PerformanceAnalysis one(file,"");
five.book(); five.format(); five.analyze(); five.save(); five.unbook();

PerformanceOverlay figures(file);

figures.book();
figures.overlay("/TIME");
figures.save("/TIME");
figures.unbook();

figures.book();
figures.overlay("/TIMING_VS_OCCUPANCY");
figures.save("/TIMING_VS_OCCUPANCY");
figures.unbook();

figures.book();
figures.overlay("/TIMING_VS_DIGIS");
figures.save("/TIMING_VS_DIGIS");
figures.unbook();

figures.book();
figures.overlay("/TIMING_VS_CLUSTERS");
figures.save("/TIMING_VS_CLUSTERS");
figures.unbook();

figures.book();
figures.overlay("/TIMING_VS_CLUSTERSIZE");
figures.save("/TIMING_VS_CLUSTERSIZE");
figures.unbook();

figures.book();
figures.overlay("/TIMING_VS_FRAC");
figures.save("/TIMING_VS_FRAC");
figures.unbook();

figures.book();
figures.overlay("/CLUSTER_WIDTH");
figures.save("/CLUSTER_WIDTH");
figures.unbook();

figures.book();
figures.overlay("/CLUSTER_FIRST");
figures.save("/CLUSTER_FIRST");
figures.unbook();

figures.book();
figures.overlay("/CLUSTER_WIDTHVSFIRST");
figures.save("/CLUSTER_WIDTHVSFIRST");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY_HLT");
figures.save("/EFFICIENCY_HLT");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY");
figures.save("/EFFICIENCY");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY_VSPT_1");
figures.save("/EFFICIENCY_VSPT_1");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY_VSPT_2");
figures.save("/EFFICIENCY_VSPT_2");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY_VSETA_1");
figures.save("/EFFICIENCY_VSETA_1");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY_VSETA_2");
figures.save("/EFFICIENCY_VSETA_2");
figures.unbook();

figures.book();
figures.overlay("/EFFICIENCY_VSETA_3");
figures.save("/EFFICIENCY_VSETA_3");
figures.unbook();

new TBrowser;
//file->Close();

}
