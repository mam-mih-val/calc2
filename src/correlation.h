//
// Created by Misha on 8/18/2023.
//

#ifndef CALC2_CORRELATION_H
#define CALC2_CORRELATION_H

#include <array>
#include <string>
#include <numeric>

#include <TFile.h>

#include <DataContainer.hpp>
#include <utility>

template <size_t N>
class Correlation {
public:
  Correlation() = delete;
  Correlation( TFile* file,
               const std::string& directory,
               const std::vector<std::string>& vector_names,
               const std::array<std::string, N>& component_names ) :
               vector_names_{ vector_names },
               component_names_{ component_names } {
    if( !file )
      throw std::runtime_error( std::string( "Error: Input file is nullptr" ) );
    int idx{};
    for (const auto &component : component_names) {
      std::string name = directory + "/";
      for( const auto& vec : vector_names ){
        name+=vec+".";
      }
      if( !component.empty() ) name+=component;
      else name.pop_back();
      components_[idx] = ReadFromFile( file, name );
      idx++;
    }
  };
  Correlation( TFile* file,
               const std::string& directory,
               const std::array<std::string, 2>& vector_names,
               const std::array<std::string, N>& component_names ) :
               vector_names_{ vector_names.data(), vector_names.data()+2 },
               component_names_{ component_names } {

    if( !file )
      throw std::runtime_error( std::string( "Error: Input file is nullptr" ) );
    int idx{};
    for (const auto &component : component_names){
      std::string name{};
      name.append(directory).append("/").append(vector_names[0]).append(".").append(vector_names[1]);
      if( !component.empty() ) name.append(".").append(component);
      components_[idx] = ReadFromFile( file, name );
      if (!components_[idx]) {
        std::string alt_name{};
        alt_name.append(directory).append("/").append(vector_names[1]).append(".").append(vector_names[0]);
        if( !component.empty() ) alt_name.append(".").append(component);
        components_[idx] = ReadFromFile( file, alt_name );
      }
      idx++;
    }
  };
  Correlation( const Correlation& ) = default;
  Correlation& operator=( const Correlation& ) = default;
  Correlation( Correlation&& )  noexcept = default;
  Correlation& operator=( Correlation&& )  noexcept = default;
  ~Correlation() = default;
  [[nodiscard]] const std::string& Title() const { return title_; }
  void SetTitle( const std::string& title ){ title_ = title; }
  void Rebin( const std::vector<Qn::AxisD>& rebin_axes ){
    std::for_each( components_.begin(), components_.end(), [&rebin_axes]( auto& component ){
      for( const auto& axis : rebin_axes ){
        if (component) component.value() = component.value().Rebin( axis );
      }
    } );
  }
  void Project( const std::vector<std::string>& projection_axes ){
    std::for_each( components_.begin(), components_.end(), [&projection_axes]( auto& component ){
        if (component )component.value() = component.value().Projection( projection_axes );
    } );
  }
  bool is_valid(){
    for (const auto &component : components_){
      if (!component) return false;
    }
    return true;
  }
  Qn::DataContainerStatCalculate& operator[]( size_t idx ){ return components_.at(idx).value_or(Qn::DataContainerStatCalculate{}); }
  [[nodiscard]] constexpr size_t Size() const  { return N; };
  void Save( const std::string& name ) const {
    std::for_each( components_.begin(), components_.end(), [ this, name, idx=0 ]( const auto& component ) mutable {
      if (component) component.value().Write( std::data(name+"."+component_names_.at(idx)) );
      idx++;
    } );
  }

  friend Correlation<N> operator+(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i] && rhs.components_[i])
        result.components_[i].value() = lhs.components_[i].value() + rhs.components_[i].value();
    }
    return result;
  }

  friend Correlation<N> operator-(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i] && rhs.components_[i])
        result.components_[i].value() = lhs.components_[i].value() - rhs.components_[i].value();
    }
    return result;
  }

  friend Correlation<N> operator*(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i] && rhs.components_[i])
        result.components_[i].value() = lhs.components_[i].value() * rhs.components_[i].value();
    }
    return result;
  }

  friend Correlation<N> operator*(const Correlation<N>& lhs, const std::array<double, N> & tensor) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i])
        result.components_[i].value() = lhs.components_[i].value() * tensor.at[i];
    }
    return result;
  }

  friend Correlation<N> operator*(const Correlation<N>& lhs, double scale) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i])
      result.components_[i].value() = lhs.components_[i].value() * scale ;
    }
    return result;
  }

  friend Correlation<N> operator*(double scale, const Correlation<N>& lhs) noexcept {
    return lhs*scale;
  }

  friend Correlation<N> operator/(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i] && rhs.components_[i])
        result.components_[i].value() = lhs.components_[i].value() / rhs.components_[i].value();
    }
    return result;
  }

  friend Correlation<N> operator/(const Correlation<N>& lhs, const std::array<double, N> & tensor) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i])
        result.components_[i].value() = lhs.components_[i].value() / tensor[i];
    }
    return result;
  }


  friend Correlation<N> operator/(const Correlation<N>& lhs, double scale) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i])
        result.components_[i] = lhs.components_[i].value() / scale;
    }
    return result;
  }

  friend Correlation<N> Sqrt(const Correlation<N>& lhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      if (lhs.components_[i])
        result.components_[i].value() = Sqrt( lhs.components_[i].value() );
    }
    return result;
  }


private:
  std::string title_;
  std::array<std::string, N> component_names_;
  std::array<std::optional<Qn::DataContainerStatCalculate>, N> components_;
  std::vector<std::string> vector_names_;
  std::array<bool, N> is_component_valid_;

  std::optional<Qn::DataContainerStatCalculate> ReadFromFile( TFile* file, const std::string& name ){
    using namespace std::literals::string_literals;
    Qn::DataContainerStatCalculate* container{nullptr};
    file->GetObject(name.c_str(), container);
    if (container) return std::make_optional(*container);
    else {
      Qn::DataContainerStatCollect *container_collect{nullptr};
      file->GetObject(name.c_str(), container_collect);
      if (container_collect) return std::make_optional(Qn::DataContainerStatCalculate(*container_collect));
      else return {};
    }
  }
};


#endif //CALC2_CORRELATION_H
