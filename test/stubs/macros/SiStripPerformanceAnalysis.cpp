{

TFile* file = new TFile("","UPDATE");

SiStripPerformanceAnalysis one(file,"");
one.book(); one.format(); one.timing(); one.save(); one.unbook();

SiStripPerformanceAnalysis two(file,"");
two.book(); two.format(); two.timing(); two.save(); two.unbook();

SiStripPerformanceAnalysis three(file,""); 
three.book(); three.format(); three.timing(); three.save(); three.unbook();

SiStripPerformanceAnalysis four(file,"");
four.book(); four.format(); four.timing(); four.save(); four.unbook();

SiStripPerformanceAnalysis five(file,"");
five.book(); five.format(); five.timing(); five.save(); five.unbook();

SiStripPerformanceFigures figures(file);

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
