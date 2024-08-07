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
#include <numeric>

class Fitter{
  Fitter( Correlation<1> corr, std::string fit_axis, std::string slice_axis ) : fit_axis_name_{ std::move(fit_axis) }, slice_axis_name_{ std::move(slice_axis) }, correlation_{corr[0]} {}
  Fitter(Qn::DataContainerStatCalculate corr, std::string fit_axis, std::string slice_axis) : fit_axis_name_{ std::move(fit_axis) }, slice_axis_name_{ std::move(slice_axis) }, correlation_{corr} {
    assert( correlation_.GetDimension() == 2 );
    auto axes = correlation_.GetAxes();
    auto fit_axis_pos = std::find_if( axes.begin(), axes.end(), [this]( const auto& a ){ return a.Name() == fit_axis_name_; } );
    assert(  fit_axis_pos != axes.end() );
    auto slice_axis_pos = std::find_if( axes.begin(), axes.end(), [this]( const auto& a ){ return a.Name() == slice_axis_name_; } );
    assert(  slice_axis_pos != axes.end() );
  }

  auto Fit( std::vector<TF1*> functions, size_t n_param ) -> void{
    std::for_each( functions.begin(), functions.end(), [n_param]( auto p ){ assert(p); assert(p->GetNpar() == n_param); } );
    auto slice_axis = correlation_.GetAxis(slice_axis_name_);
    auto projections = GetProjections( slice_axis );
    auto fit_parameters = std::vector< std::vector< std::vector<double> > >(n_param);
    auto bin = size_t{};
    for( const auto& proj : projections ){
      auto sample_graphs = GetSampleGraphs(proj);
      auto fit_param_samples = FitSamples( sample_graphs, functions.at(bin) );
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

private:
  auto GetProjections( const Qn::AxisD& slice_axis ) -> std::vector<Qn::DataContainerStatCalculate> { 
    auto result = std::vector<Qn::DataContainerStatCalculate>{};
    result.reserve( slice_axis.size() );
    for( size_t bin=0; bin < slice_axis.size(); ++bin ){
      auto lo = slice_axis.GetLowerBinEdge(bin);
      auto up = slice_axis.GetUpperBinEdge(bin);
      auto proj = correlation_.Select( {slice_axis.Name(), 1, lo, up} );
      result.push_back(proj);
    }
    return result;
  }

  auto GetSampleGraphs( const Qn::DataContainerStatCalculate& one_dim_correlation ) -> std::vector< std::unique_ptr<TGraphErrors> > {
    assert( one_dim_correlation.GetDimension() == 1 );
    auto n_samples = one_dim_correlation.At(0).GetSampleMeans().size();
    auto result = std::vector< std::unique_ptr<TGraphErrors> >( n_samples );
    std::for_each( result.begin(), result.end(), [&one_dim_correlation]( auto& p ){ p.reset( new TGraphErrors( one_dim_correlation.size() ) ); } );
    for( size_t sample = 0; sample < n_samples; ++sample ){
      for( size_t bin = 0; bin < one_dim_correlation.size(); ++bin ){
        auto bin_x = one_dim_correlation.GetAxes().at(0).GetBinCenter(bin);
        auto bin_y = one_dim_correlation.At(bin).GetSampleMeans().at(sample);
        result.at(sample)->SetPoint( bin, bin_x, bin_y );
      }
    }
    return result;
  }
  
  auto FitSamples( const std::vector< std::unique_ptr<TGraphErrors> >& graphs, TF1* function ) -> std::vector< std::vector<double> > {
    assert(function);
    std::for_each( graphs.begin(), graphs.end(), [function]( const auto& p ){ assert(p); p->Fit(function); } );
    auto n_par = static_cast<size_t>(function->GetNpar());
    auto fit_params = std::vector< std::vector<double> >(n_par);
    for( size_t sample = 0; sample < graphs.size(); ++sample ){
      auto fit_res = dynamic_cast<TF1*>(graphs.at(sample)->GetListOfFunctions()->At(0));
      for( size_t par=0; par < n_par; ++par ){
        fit_params.at(par).push_back( fit_res->GetParameter(par) );
      }
    }
    return fit_params;
  }

  auto FillDataContainer( const Qn::DataContainerStatCalculate& reference, const std::vector<std::vector<double>>& par_samples ) -> Qn::DataContainerStatCalculate{
    assert(reference.GetDimension() == 1);
    auto axes = reference.GetAxes();
    auto container = Qn::DataContainerStatCalculate{ axes };
    container.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP );
    size_t bin=0;
    for( auto& stat : container ){
      auto means =  par_samples.at(bin);
      auto weights = reference.At(bin).GetSampleWeights();
      assert( means.size() == weights.size() );
      auto statistics = Qn::Statistics{};
      for( size_t i=0; i < means.size(); ++i ){
        statistics.Fill(means.at(i), weights.at(i));
      }
      stat.SetSampleMeans( par_samples.at(bin) );
      stat.SetSampleWeights( reference.At(bin).GetSampleWeights() );
      stat.SetMean( statistics.Mean() );
      bin++;
    }
    return container;
  }

  std::string fit_axis_name_{};
  std::string slice_axis_name_{};
  Qn::DataContainerStatCalculate correlation_;
  std::vector<Qn::DataContainerStatCalculate> fit_results_{};
};

#endif // FLOW_CALCULATOR_SRC_FITTER_HPP_