{

TRFIOFile* file = new TRFIOFile("rfio:/tmp/pwing/RegionalRawToClusters.root","UPDATE");

PerformanceAnalysis rawtoclusters(file,"0.5_All_50");
rawtoclusters.book();
rawtoclusters.analyze();
rawtoclusters.unbook();

PerformanceAnalysis regionalrawtoclusters(file,"0.01_All_50");
regionalrawtoclusters.book();
regionalrawtoclusters.analyze();
regionalrawtoclusters.unbook();

PerformanceOverlay rawtocluster(file);
rawtocluster.book();
rawtocluster.overlay("TIME");
rawtocluster.overlay("TIMING_VS_OCCUPANCY");
rawtocluster.overlay("TIMING_VS_DIGIS");
rawtocluster.overlay("TIMING_VS_CLUSTERS");
rawtocluster.overlay("TIMING_VS_CLUSTERSIZE");
rawtocluster.overlay("TIMING_VS_FRAC");
rawtocluster.overlay("CLUSTER_WIDTH");
rawtocluster.overlay("CLUSTER_FIRST");
rawtocluster.overlay("CLUSTER_WIDTHVSFIRST");
rawtocluster.overlay("CLUSTER_HOLES");
rawtocluster.overlay("ELECTRON_EFFICIENCYVSPT");
rawtocluster.overlay("ELECTRON_EFFICIENCYVSETA");
rawtocluster.unbook();
new TBrowser;
//file->Close();

}
