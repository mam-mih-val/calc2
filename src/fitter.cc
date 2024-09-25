#include "fitter.h"
#include <DataContainer.hpp>
#include <DataContainerHelper.hpp>
#include <algorithm>
#include <cassert>

auto Fitter::FitBin( Qn::DataContainerStatCalculate& bin, TF1* function ) -> BinFit{
  assert(function);
  assert( bin.GetDimension() == 1 );
  auto main_sample_graph = std::unique_ptr<TGraphErrors>( Qn::ToTGraph(bin) );
  auto sample_graphs = GetSampleGraphs( bin );
  auto main_sample_parameters = FitMainSample(main_sample_graph, function);
  auto samples_parameters = FitSamples(sample_graphs, function);
  auto bin_result = BinFit{};
  bin_result.main_sample_params = std::move(main_sample_parameters);
  bin_result.subsamples_params = std::move(samples_parameters);
  main_sample_graphs_.emplace_back( std::move(main_sample_graph) );
  subsample_graphs_.emplace_back( std::move(sample_graphs) );
  return bin_result;
}

auto Fitter::FitMainSample( const std::unique_ptr<TGraphErrors>& main_sample, TF1* function ) -> std::vector<double> {
  assert(function);
  auto n_par = static_cast<size_t>(function->GetNpar());
  auto result = std::vector<double>( n_par );
  main_sample->Fit( function );
  auto fit_res = dynamic_cast<TF1*>(main_sample->GetListOfFunctions()->At(0));
  for( auto i=size_t{}; i < n_par; ++i ){
    result.at(i) = fit_res->GetParameter(i);
  }
  return result;
}

auto Fitter::FitProjections( std::vector<Qn::DataContainerStatCalculate> projections, std::vector<TF1*> functions ) -> FitResult {
  std::for_each( projections.begin(), projections.end(), [](const auto& p){ assert( p.GetDimension() == 1 ); } );
  std::for_each( functions.begin(), functions.end(), [](auto f){ assert( f ); } );
  assert( projections.size() == functions.size() );
  auto result = FitResult{};
  for( auto i=size_t{}; i < projections.size(); ++i ){
    auto bin_result = FitBin( projections.at(i), functions.at(i) );
    result.results.push_back(bin_result);
  }
  return result;
}

auto Fitter::GetProjections( const Qn::AxisD& slice_axis ) -> std::vector<Qn::DataContainerStatCalculate> { 
  auto result = std::vector<Qn::DataContainerStatCalculate>{};
  result.reserve( slice_axis.size() );
  for( size_t bin=0; bin < slice_axis.size(); ++bin ){
    auto lo = slice_axis.GetLowerBinEdge(bin);
    auto up = slice_axis.GetUpperBinEdge(bin);
    auto proj = correlation_.Select( {slice_axis.Name(), 1, lo, up} );
    proj = proj.Projection( {fit_axis_name_} );
    result.push_back(proj);
  }
  return result;
}

auto Fitter::GetSampleGraphs( const Qn::DataContainerStatCalculate& one_dim_correlation ) -> std::vector< std::unique_ptr<TGraphErrors> > {
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
  
auto Fitter::FitSamples( const std::vector< std::unique_ptr<TGraphErrors> >& graphs, TF1* function ) -> std::vector< std::vector<double> > {
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

auto Fitter::FillDataContainer( const Qn::DataContainerStatCalculate& reference, FitResult fit_parameters ) -> std::vector<Qn::DataContainerStatCalculate>{
  assert(reference.GetDimension() == 1);
  auto axes = reference.GetAxes();
  auto result = std::vector<Qn::DataContainerStatCalculate>( fit_parameters.results.front().main_sample_params.size(), Qn::DataContainerStatCalculate{ axes } );
  auto n_bins = fit_parameters.results.size();
  for( auto bin = size_t{}; bin<n_bins; ++bin ){
    auto bin_result = fit_parameters.results.at(bin);
    auto weights = reference.At(bin).GetSampleWeights();
    for( auto par = size_t{}; par < bin_result.main_sample_params.size(); ++par ){
      result.at(par).At(bin).SetMean( bin_result.main_sample_params.at(par) );
      result.at(par).At(bin).SetSampleMeans( bin_result.subsamples_params.at(par) );
      result.at(par).At(bin).SetSampleWeights( std::vector<double>( bin_result.subsamples_params.at(par).size(), 1.0 ) );
      auto statistics = Qn::Statistics{};
      for( size_t j=0; j < bin_result.subsamples_params.at(par).size(); ++j ){
        statistics.Fill(bin_result.subsamples_params.at(par).at(j), weights.at(bin));
      }
      result.at(par).At(bin).SetVariance(statistics.Variance());
      result.at(par).At(bin).SetSumWeights(statistics.SumWeights());
      result.at(par).At(bin).SetSumWeights2(statistics.SumWeights2());
    }
  }
  
  return result;
}