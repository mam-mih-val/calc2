//
// Created by mikhail on 7/30/21.
//

#ifndef QNANALYSIS_SRC_QNANALYSISCALCULATE_CALCULATORLINKDEF_HPP_
#define QNANALYSIS_SRC_QNANALYSISCALCULATE_CALCULATORLINKDEF_HPP_

#if defined(__ROOTCLING__) || defined(__MAKECINT__)

#pragma link off all class;
#pragma link off all function;
#pragma link off all global;
#pragma link off all typedef;

#pragma link C++ class Correlation<1>+;
#pragma link C++ class Correlation<2>+;
#pragma link C++ class Correlation<3>+;
#pragma link C++ class Correlation<4>+;

#pragma link C++ class Functions+;
#pragma link C++ class Fitter+;

#pragma link C++ class FunctionsNE+;

#endif

#endif//QNANALYSIS_SRC_QNANALYSISCALCULATE_CALCULATORLINKDEF_HPP_
