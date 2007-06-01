{

TFile* file = new TFile("file:/tmp/pwing/50x50_FullTracker.root","UPDATE");

PerformanceAnalysis one(file,"Global");
one.book(); one.format(); one.analyze(); one.save(); one.unbook();

PerformanceAnalysis two(file,"0.05");
two.book(); two.format(); two.analyze(); two.save(); two.unbook();

PerformanceAnalysis three(file,"0.1"); 
three.book(); three.format(); three.analyze(); three.save(); three.unbook();

PerformanceAnalysis four(file,"0.15"); 
four.book(); four.format(); four.analyze(); four.save(); four.unbook();

PerformanceAnalysis five(file,"0.2"); 
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
figures.overlay("/ELECTRONVSPT_BARREL");
figures.save("/ELECTRONVSPT_BARREL");
figures.unbook();

figures.book();
figures.overlay("/ELECTRONVSPT_ENDCAP");
figures.save("/ELECTRONVSPT_ENDCAP");
figures.unbook();

figures.book();
figures.overlay("/ELECTRONVSETA_LT100");
figures.save("/ELECTRONVSETA_LT100");
figures.unbook();

figures.book();
figures.overlay("/ELECTRONVSETA_GT100");
figures.save("/ELECTRONVSETA_GT100");
figures.unbook();

new TBrowser;
//file->Close();

}
