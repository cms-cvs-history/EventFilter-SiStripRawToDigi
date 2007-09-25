#ifndef simpleanalysis_SimpleConstants_H
#define simpleanalysis_SimpleConstants_H

namespace constants {

  static const unsigned int invalid32 = 0xFFFFFFFF;
  static const unsigned int invalid16 = 0xFFFF;
  static const double invalid = -9999999.;
  static const double large = 9999999.;
  static const double dRelectron = 0.4;
  static const double dRmuon = 0.4;
  static const double dRtau = 0.4;
  static const double dRjet = 0.4;
  static const double tracker = 2.4;
  static const double radius = 0.1365;
  static const double length = 3.20;
  static const double endcap = 1.469;
  static const double field = 4.; 
  static const double rbarm = 1.31;
  static const double zendm = 3.15;
  static const double bend = 0.3*4*rbarm/2.0;
  static const double c = 300000000.;
  static const double pi = 3.1415927;
}

#endif
