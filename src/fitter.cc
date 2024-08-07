#include "fitter.h"

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

  auto Fitter::FillDataContainer( const Qn::DataContainerStatCalculate& reference, const std::vector<std::vector<double>>& par_samples ) -> Qn::DataContainerStatCalculate{
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
      stat.SetVariance(statistics.Variance());
      stat.SetSumWeights(statistics.SumWeights());
      stat.SetSumWeights2(statistics.SumWeights2());
      bin++;
    }
    return container;
  }