//
// Created by mikhail on 8/1/21.
//

#ifndef FLOW_CALCULATOR_SRC_FUNCTIONS_HPP_
#define FLOW_CALCULATOR_SRC_FUNCTIONS_HPP_

#include <TObject.h>
#include <TFile.h>
#include "correlation.h"

class Functions {
public:
  Functions() = delete;
  ~Functions() = default;

  template<size_t N>
  static Correlation<N> Resolution3S(const Correlation<N>& first, const Correlation<N>& second, const Correlation<N>& third){
    auto result = Sqrt( first * second / third);
    return result;
  }

  template<size_t N>
  static std::vector< Correlation<N> > VectorResolutions3S(TFile* file,
                                                      const std::string& directory,
                                                      const std::string& ep_vector,
                                                      const std::vector<std::string>& res_vectors,
                                                      const std::array<std::string, N>& comp_names){
    std::vector<Correlation<N>> res_vector;
    const auto& qa = ep_vector;
    auto res_v = res_vectors;
    auto it = std::find( res_v.begin(), res_v.end(), qa );
    if (it != res_v.end()){
      res_v.erase(it);
    }
    for( size_t i=0; i<res_v.size(); ++i ){
      auto qb = res_v.at(i);
      for( size_t j=i+1; j<res_v.size(); ++j ){
        auto qc = res_v.at(j);

        auto qa_qb = Correlation(file, directory, std::array{qa, qb}, comp_names);
        auto qa_qc = Correlation(file, directory, std::array{qa, qc}, comp_names);
        auto qb_qc = Correlation(file, directory, std::array{qb, qc}, comp_names);

        if (qa_qb.is_valid() && qa_qc.is_valid() && qb_qc.is_valid()) {
          auto res_qa = Functions::Resolution3S( qa_qb*2, qa_qc*2, qb_qc*2 );
          std::string res_title{};
          res_title.append(qa).append("(").append(qb).append(",").append(qc).append(")");
          res_qa.SetTitle( res_title );
          res_vector.emplace_back(res_qa);
        }
      }
    }
    return res_vector;
  }

  template<size_t N>
  static std::vector<Correlation<N>> VectorResolutions4S(TFile* file,
                                                      const std::string& directory,
                                                      const std::string& ep_vector,
                                                      const std::vector<std::string>& sub_vectors,
                                                      const std::vector<std::string>& res_vectors,
                                                      const std::array<std::string, N>& comp_names){
    std::vector<Correlation<N>> result_vector;
    for( const auto& sub : sub_vectors ){
      auto ep_sub = Correlation(file, directory, std::array{ep_vector, sub}, comp_names)*2.;
      if (!ep_sub.is_valid()) continue;
      auto vec_res_sub = VectorResolutions3S( file, directory, sub, res_vectors, comp_names );
      for( const auto& R1_sub : vec_res_sub ){
        auto res_4sub = ep_sub / R1_sub;
        res_4sub.SetTitle( ep_vector+"."+R1_sub.Title() );
        result_vector.emplace_back( res_4sub );
      }
    }
    return result_vector;
  };
//  static Correlation ExtrapolateToFullEvent(const Correlation& half_event_resolution, double order);
private:
//  static double ResolutionFunction( double chi, double k, double y );
//  static double DichotomyResolutionSolver( double res, double order, std::vector<double> range = {0., 10.} );
};

#endif // FLOW_CALCULATOR_SRC_FUNCTIONS_HPP_
