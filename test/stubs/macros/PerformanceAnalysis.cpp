{

TRFIOFile* file = new TRFIOFile("rfio:/castor/cern.ch/user/p/pwing/LowLevelRecon/RegionalRawToClusters.root","UPDATE");

PerformanceAnalysis one(file,"tree");
one.book();
one.analyze();
one.unbook();
/*
PerformanceAnalysis two(file,"0.01_All_50");
two.book();
two.analyze();
two.unbook();
*/
PerformanceOverlay figures(file);
figures.book();
figures.overlay("TIME");
figures.overlay("TIMING_VS_OCCUPANCY");
figures.overlay("TIMING_VS_DIGIS");
figures.overlay("TIMING_VS_CLUSTERS");
figures.overlay("TIMING_VS_CLUSTERSIZE");
figures.overlay("TIMING_VS_FRAC");
figures.overlay("CLUSTER_WIDTH");
figures.overlay("CLUSTER_FIRST");
figures.overlay("CLUSTER_WIDTHVSFIRST");
figures.overlay("CLUSTER_HOLES");
figures.overlay("ELECTRON_EFFICIENCYVSPT");
figures.overlay("ELECTRON_EFFICIENCYVSETA");
figures.unbook();

new TBrowser;
//file->Close();

}
