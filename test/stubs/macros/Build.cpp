{

gROOT->Reset("a");
gSystem->AddIncludePath("-I/$CMSSW_BASE/src/");
gSystem->AddIncludePath("-I/$CMSSW_RELEASE_BASE/src/");
gSystem->Load("$CMSSW_BASE/lib/slc4_ia32_gcc345/libtestEventFilterSiStripRawToDigi");
gSystem->CompileMacro("PerformanceAnalysis.cc","fkg","libPerformanceAnalysis");
gSystem->CompileMacro("PerformanceOverlay.cc","fkg","libPerformanceOverlay");

}