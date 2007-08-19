{

TFile* file = new TFile("","UPDATE");

PerformanceAnalysis one(file,"");
one.book(); one.format(); one.analyze(); one.save(); one.unbook();

PerformanceAnalysis two(file,"");
two.book(); two.format(); two.analyze(); two.save(); two.unbook();

PerformanceAnalysis three(file,""); 
three.book(); three.format(); three.analyze(); three.save(); three.unbook();

PerformanceAnalysis four(file,"");
four.book(); four.format(); four.analyze(); four.save(); four.unbook();

PerformanceAnalysis five(file,"");
five.book(); five.format(); five.analyze(); five.save(); five.unbook();

PerformanceOverlay figures(file);

figures.book();
figures.read("");
figures.read("");
figures.read("");
figures.read("");
figures.read("");
figures.overlay();
figures.unbook();

file->Close();

}
