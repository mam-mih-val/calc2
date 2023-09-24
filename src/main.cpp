#include <iostream>
#include <chrono>

#include <TROOT.h>
#include <TSystem.h>

#define CALC_INCLUDE_PATH "/mnt/c/Users/Misha/CLionProjects/calc2/src"
#define QnTools_LIB_PATH "QnTools::Base"


int main(int n_args, char** args) {
  if( n_args < 2 )
    throw std::runtime_error( "No arguments provided" );
//  gInterpreter->Load(QnTools_LIB_PATH);
//  gInterpreter->AddIncludePath(CALC_INCLUDE_PATH);
  auto start = std::chrono::high_resolution_clock::now();
  gROOT->Macro(args[1]);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = end-start;
  std::cout << "Elapsed time: " << std::chrono::duration<double, std::milli>(duration).count() / 1000. << " (s)" << std::endl;
}
