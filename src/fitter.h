#ifndef FLOW_CALCULATOR_SRC_FITTER_HPP_
#define FLOW_CALCULATOR_SRC_FITTER_HPP_

#include "src/correlation.h"
#include <Axis.hpp>
#include <DataContainer.hpp>
#include <StatCalculate.hpp>
#include <Statistics.hpp>
#include <TGraphErrors.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <numeric>

struct BinFit{
  std::vector<double> main_sample_params;
  std::vector<std::vector<double>> subsamples_params;
};

struct FitResult{
  std::vector<BinFit> results;
};

class Fitter{
public:
  Fitter( Correlation<1> corr, std::string fit_axis, std::string slice_axis ) : fit_axis_name_{ std::move(fit_axis) }, slice_axis_name_{ std::move(slice_axis) }, correlation_{corr[0]} {
    assert( correlation_.GetDimension() == 2 );
    auto axes = correlation_.GetAxes();
    auto fit_axis_pos = std::find_if( axes.begin(), axes.end(), [this]( const auto& a ){ return a.Name() == fit_axis_name_; } );
    assert(  fit_axis_pos != axes.end() );
    auto slice_axis_pos = std::find_if( axes.begin(), axes.end(), [this]( const auto& a ){ return a.Name() == slice_axis_name_; } );
    assert(  slice_axis_pos != axes.end() );
  }
  
  Fitter(Qn::DataContainerStatCalculate corr, std::string fit_axis, std::string slice_axis) : fit_axis_name_{ std::move(fit_axis) }, slice_axis_name_{ std::move(slice_axis) }, correlation_{corr} {
    assert( correlation_.GetDimension() == 2 );
    auto axes = correlation_.GetAxes();
    auto fit_axis_pos = std::find_if( axes.begin(), axes.end(), [this]( const auto& a ){ return a.Name() == fit_axis_name_; } );
    assert(  fit_axis_pos != axes.end() );
    auto slice_axis_pos = std::find_if( axes.begin(), axes.end(), [this]( const auto& a ){ return a.Name() == slice_axis_name_; } );
    assert(  slice_axis_pos != axes.end() );
  }

  auto Fit( std::vector<TF1*> functions, size_t n_param ) -> void{
    std::for_each( functions.begin(), functions.end(), [n_param]( auto p ){ assert(p); assert( static_cast<size_t>(p->GetNpar()) == n_param); } );
    auto slice_axis = correlation_.GetAxis(slice_axis_name_);
    auto projections = GetProjections( slice_axis );
    if( functions.size() == 1 ){
      auto front = functions.front();
      functions.resize( projections.size() );
      std::for_each( functions.begin(), functions.end(), [front]( auto& p ){ p = front; } );
    }
    fit_parameters_ = FitProjections(projections, functions);

    auto reference = correlation_.Projection( std::vector<std::string>{slice_axis_name_} );
    
    fit_results_ = FillDataContainer( reference , fit_parameters_);
  }

  auto GetFitResults() -> const std::vector<Qn::DataContainerStatCalculate>& {
    return fit_results_;
  }

  auto DumpGraphs( const std::string& file_name ) -> void {
    auto file = new TFile( file_name.c_str(), "RECREATE" );
    file->cd();
    for( auto bin=size_t{}; bin < main_sample_graphs_.size(); ++bin ){
      file->cd();
      auto dir_name = std::string( "bin_" ).append( std::to_string(bin) );
      auto main_sample_name = dir_name + "_main";
      file->mkdir( dir_name.c_str() );
      file->cd( dir_name.c_str() );
      main_sample_graphs_.at(bin)->Write( main_sample_name.c_str() );
      for( auto sample=size_t{}; sample < subsample_graphs_.at(bin).size(); ++sample ){
        auto subsample_name = dir_name + "_" + std::to_string( sample );
        subsample_graphs_.at(bin).at(sample)->Write( subsample_name.c_str() );
      }
    }
    file->Close();
  }

private:
  auto GetProjections( const Qn::AxisD& slice_axis ) -> std::vector<Qn::DataContainerStatCalculate>;

  auto GetSampleGraphs( const Qn::DataContainerStatCalculate& one_dim_correlation ) -> std::vector< std::unique_ptr<TGraphErrors> >;
  
  auto FitSamples( const std::vector< std::unique_ptr<TGraphErrors> >& graphs, TF1* function ) -> std::vector< std::vector<double> >;
  
  auto FitMainSample( const std::unique_ptr<TGraphErrors>& main_sample, TF1* function ) -> std::vector<double>;

  auto FillDataContainer( const Qn::DataContainerStatCalculate& reference, FitResult fit_parameters ) -> std::vector<Qn::DataContainerStatCalculate>;

  auto FitBin( Qn::DataContainerStatCalculate& bin, TF1* function ) -> BinFit;

  auto FitProjections( std::vector<Qn::DataContainerStatCalculate> projections, std::vector<TF1*> functions ) -> FitResult;

  std::string fit_axis_name_{};
  std::string slice_axis_name_{};
  Qn::DataContainerStatCalculate correlation_;
  std::vector<Qn::DataContainerStatCalculate> fit_results_{};
  FitResult fit_parameters_{};
  std::vector<std::unique_ptr<TGraphErrors>> main_sample_graphs_{};
  std::vector<std::vector<std::unique_ptr<TGraphErrors>>> subsample_graphs_{};
};

#endif // FLOW_CALCULATOR_SRC_FITTER_HPP_