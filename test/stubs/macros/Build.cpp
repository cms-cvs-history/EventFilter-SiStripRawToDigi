{
gROOT->Reset();
gSystem->AddIncludePath("-I/$CMSSW_BASE/src/");
gSystem->Load("$CMSSW_BASE/lib/slc4_ia32_gcc345/libtestEventFilterSiStripRawToDigi.so");
gSystem->CompileMacro("Plots.cc","fkg","libPlots");
gSystem->CompileMacro("ObjectFilter.cc","fkg","libObjectFilter");
gSystem->CompileMacro("PerformanceAnalysis.cc","fkg","libPerformanceAnalysis");
gSystem->CompileMacro("PerformanceOverlay.cc","fkg","libPerformanceOverlay");
}
