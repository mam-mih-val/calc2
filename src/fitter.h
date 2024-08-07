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
    auto fit_parameters = std::vector< std::vector< std::vector<double> > >(n_param);
    auto bin = size_t{};
    for( const auto& proj : projections ){
      auto sample_graphs = GetSampleGraphs(proj);
      auto fit_param_samples = FitSamples( sample_graphs, functions.at(bin) );
      std::for_each( sample_graphs.begin(), sample_graphs.end(), [this]( auto& g ){ graphs_.emplace_back( std::move(g) ); } );
      for( auto i=size_t{}; i<n_param; ++i ){
        fit_parameters.at(i).push_back(fit_param_samples.at(i) );
      }
    }

    auto reference = correlation_.Projection( std::vector<std::string>{slice_axis_name_} );
    for( auto i=size_t{}; i<n_param; ++i ){
      fit_results_.emplace_back( FillDataContainer(reference, fit_parameters.at(i)) );
    }
  }

  auto GetFitResults() -> const std::vector<Qn::DataContainerStatCalculate>& {
    return fit_results_;
  }

  auto DumpGraphs( const std::string& file_name ) -> void {
    auto file = new TFile( file_name.c_str(), "RECREATE" );
    file->cd();
    std::for_each( graphs_.begin(), graphs_.end(), []( auto& g ){ g->Write(); } );
    file->Close();
  }

private:
  auto GetProjections( const Qn::AxisD& slice_axis ) -> std::vector<Qn::DataContainerStatCalculate>;

  auto GetSampleGraphs( const Qn::DataContainerStatCalculate& one_dim_correlation ) -> std::vector< std::unique_ptr<TGraphErrors> >;
  
  auto FitSamples( const std::vector< std::unique_ptr<TGraphErrors> >& graphs, TF1* function ) -> std::vector< std::vector<double> >;

  auto FillDataContainer( const Qn::DataContainerStatCalculate& reference, const std::vector<std::vector<double>>& par_samples ) -> Qn::DataContainerStatCalculate;

  std::string fit_axis_name_{};
  std::string slice_axis_name_{};
  Qn::DataContainerStatCalculate correlation_;
  std::vector<Qn::DataContainerStatCalculate> fit_results_{};
  std::vector<std::unique_ptr<TGraphErrors>> graphs_{};
};

#endif // FLOW_CALCULATOR_SRC_FITTER_HPP_