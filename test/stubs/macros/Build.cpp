{

gROOT->Reset("a");
gSystem->AddIncludePath("-I/$CMSSW_BASE/src/");
gSystem->AddIncludePath("-I/$CMSSW_RELEASE_BASE/src/");
gSystem->CompileMacro("PerformanceAnalysis.cc","fkg","libPerformanceAnalysis");
gSystem->CompileMacro("PerformanceOverlay.cc","fkg","libPerformanceOverlay");

}
