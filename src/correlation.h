//
// Created by Misha on 8/18/2023.
//

#ifndef CALC2_CORRELATION_H
#define CALC2_CORRELATION_H

#include <array>
#include <exception>
#include <optional>
#include <string>
#include <numeric>
#include <utility>

#include <TFile.h>

#include <DataContainer.hpp>


/// @brief Class for handling the correlations of the components of u and Q-vectors 
///  and perform the arithmetic operations
/// @param N is a number of correlations to be bind in single Correlation<N> class
template <size_t N>
class Correlation {
public:
  /// @brief The exception thrown in a case is input file is not found or cannot be opened
  class input_file_is_nullptr : public std::exception{
    public:
    input_file_is_nullptr(const std::string& function) : function_(function) {
      msg_ = "Correlation::" + function_+ "() " + msg_;
    }
    ~input_file_is_nullptr() = default;
    const char* what() const throw () override {
      return msg_.c_str();
    }
    private:
    std::string function_;
    std::string msg_{ "Input file is nullptr" };
  };
  /// @brief The exception thrown in case if oppened file does not contain the correlation
  class no_object_in_file : public std::exception{
    public:
    no_object_in_file(const std::string& function, const std::string& obj_name) : function_(function), obj_name_(obj_name) {
      msg_ = "Correlation::" + function_+ "() " + msg_ + " " + obj_name;
    }
    ~no_object_in_file() = default;
    const char* what() const throw () override {
      return msg_.c_str();
    }
    const std::string& obj_name(){ return obj_name_; }
    private:
    std::string function_;
    std::string obj_name_;
    std::string msg_{ "No object in a file with name" };
  };
  /// @brief Default constructor
  Correlation() = default;
  /// @brief Constructor wrapping a number of correlations in a single Correlation object.
  /// @param title is a name of the correlation. Used when saving the objects to a file
  /// @param component_names is an array of correlation components names
  /// @param components is an array of correlations to be bound in a single object
  /// @param vector_names contains the titles of vectors used to build the correlation
  Correlation( 
    const std::string& title,
    const std::array<std::string, N>& component_names,
    const std::array<Qn::DataContainerStatCalculate, N>& components,
    const std::vector<std::string>& vector_names 
    ) : title_{ title }, 
        component_names_{ component_names }, 
        components_{ components }, 
        vector_names_{vector_names} {}
  /// @brief Constructor initializing the inner structure reading the objects form a file
  /// @param file is a pointer to a file from which the objects are read
  /// @param directory is a name of the directory containing the objects
  /// @param vector_names contains the titles of the vectors used to calculate the correlations.
  /// @param component_names should contain the components of the correlation
  /// Correlation names fetched from the file will be formed as follows:
  /// directory/vector_names[0].vector_names[1]...vector_names[M].component_names[0]
  /// directory/vector_names[0].vector_names[1]...vector_names[M].component_names[1]
  /// ...
  /// directory/vector_names[0].vector_names[1]...vector_names[M].component_names[N-1]
  Correlation( TFile* file,
               const std::string& directory,
               const std::vector<std::string>& vector_names,
               const std::array<std::string, N>& component_names ) :
               vector_names_{ vector_names },
               component_names_{ component_names } {
    if( !file )
      throw input_file_is_nullptr(__func__);
    int idx{};
    for (const auto &component : component_names) {
      std::string name = directory + "/";
      for( const auto& vec : vector_names ){
        name+=vec+".";
      }
      if( !component.empty() ) name+=component;
      else name.pop_back();
      auto component_obj = ReadFromFile( file, name );
      if( !component_obj )
        throw no_object_in_file(__func__, name);
      components_[idx] = component_obj.value();
      idx++;
    }
  };
  /// @brief Same as the constructor initializing the inner structure reading the objects form a file.
  /// @brief But in cas if the correlation vector_names[0].vector_names[1] is not found it will try to fetch vector_names[1].vector_names[0]
  /// @param file is a pointer to a file from which the objects are read
  /// @param directory is a name of the directory containing the objects
  /// @param vector_names contains the titles of the vectors used to calculate the correlations.
  /// @param component_names should contain the components of the correlation
  /// Correlation names fetched from the file will be formed as follows:
  /// directory/vector_names[0].vector_names[1]...vector_names[M].component_names[0]
  /// directory/vector_names[0].vector_names[1]...vector_names[M].component_names[1]
  /// ...
  /// directory/vector_names[0].vector_names[1]...vector_names[M].component_names[N-1]
  Correlation( TFile* file,
               const std::string& directory,
               const std::array<std::string, 2>& vector_names,
               const std::array<std::string, N>& component_names ) :
               vector_names_{ vector_names.data(), vector_names.data()+2 },
               component_names_{ component_names } {

    if( !file )
      throw input_file_is_nullptr(__func__);
    int idx{};
    for (const auto &component : component_names){
      std::string name{};
      name.append(directory).append("/").append(vector_names[0]).append(".").append(vector_names[1]);
      if( !component.empty() ) name.append(".").append(component);
      std::string alt_name{};
      alt_name.append(directory).append("/").append(vector_names[1]).append(".").append(vector_names[0]);
      if( !component.empty() ) alt_name.append(".").append(component);
      auto component_obj = ReadFromFile( file, name ); 
      if ( !component_obj ) 
        component_obj = ReadFromFile( file, alt_name );
      if( !component_obj )
        throw no_object_in_file(__func__, name);
      components_[idx] = component_obj.value();
      idx++;
    }
  };
  /// @brief Default copy constructor
  Correlation( const Correlation& ) = default;
  /// @brief Default copy assignment operator
  Correlation& operator=( const Correlation& ) = default;
  /// @brief Default move constructor
  Correlation( Correlation&& )  noexcept = default;
  /// @brief Default move assignment operator
  Correlation& operator=( Correlation&& )  noexcept = default;
  /// @brief Default destructor
  ~Correlation() = default;
  [[nodiscard]] const std::string& Title() const { return title_; }
  void SetTitle( const std::string& title ){ title_ = title; }
  /// @brief Rebins all the containing correlations along the provided axes 
  void Rebin( const std::vector<Qn::AxisD>& rebin_axes ){
    std::for_each( components_.begin(), components_.end(), [&rebin_axes]( auto& component ){
      for( const auto& axis : rebin_axes ){
        component = component.Rebin( axis );
      }
    } );
  }
  /// @brief Projects all the containing correlations onto the provided axes 
  void Project( const std::vector<std::string>& projection_axes ){
    std::for_each( components_.begin(), components_.end(), [&projection_axes]( auto& component ){
      component = component.Projection( projection_axes );
    } );
  }
  /// @brief Index access operator. 
  Qn::DataContainerStatCalculate& operator[]( size_t idx ){ return components_.at(idx); }
  [[nodiscard]] constexpr size_t Size() const  { return N; };
  void Save( const std::string& name ) const {
    std::for_each( components_.begin(), components_.end(), [ this, name, idx=0 ]( const auto& component ) mutable {
      component.Write( std::data(name+"."+component_names_.at(idx)) );
      idx++;
    } );
  }

  friend Correlation<N> operator+(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] + rhs.components_[i];
    }
    return result;
  }

  friend Correlation<N> operator-(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] - rhs.components_[i];
    }
    return result;
  }

  friend Correlation<N> operator*(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] * rhs.components_[i];
    }
    return result;
  }

  friend Correlation<N> operator*(const Correlation<N>& lhs, const std::array<double, N> & tensor) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] * tensor[i];
    }
    return result;
  }

  friend Correlation<N> operator*(const Correlation<N>& lhs, double scale) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] * scale ;
    }
    return result;
  }

  friend Correlation<N> operator*(double scale, const Correlation<N>& lhs) noexcept {
    return lhs*scale;
  }

  friend Correlation<N> operator/(const Correlation<N>& lhs, const Correlation<N> & rhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] / rhs.components_[i];
    }
    return result;
  }

  friend Correlation<N> operator/(const Correlation<N>& lhs, const std::array<double, N> & tensor) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] / tensor[i];
    }
    return result;
  }


  friend Correlation<N> operator/(const Correlation<N>& lhs, double scale) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = lhs.components_[i] / scale;
    }
    return result;
  }

  friend Correlation<N> Sqrt(const Correlation<N>& lhs) noexcept {
    auto result = Correlation(lhs);
    for( size_t i=0; i<N; ++i ){
      result.components_[i] = Sqrt( lhs.components_[i] );
    }
    return result;
  }
  /// @brief Factory returning the std::optional<Correlation<M>>. Use to avoid handling the exceptions
  template<size_t M>
  friend std::optional< Correlation<M> > MakeCorrelation( TFile* file, 
                                                          const std::string& directory, 
                                                          const std::array<std::string, 2>& vector_names, 
                                                          const std::array<std::string, M>& component_names ) noexcept;
  
private:
  std::string title_;
  std::array<std::string, N> component_names_;
  std::array<Qn::DataContainerStatCalculate, N> components_;
  std::vector<std::string> vector_names_;

  static std::optional<Qn::DataContainerStatCalculate> ReadFromFile( TFile* file, const std::string& name ) noexcept {
    Qn::DataContainerStatCalculate* container{nullptr};
    file->GetObject(name.c_str(), container);
    if (container) 
      return std::make_optional(*container);
    Qn::DataContainerStatCollect *container_collect{nullptr};
    file->GetObject(name.c_str(), container_collect);
    if (container_collect) 
      return std::make_optional(Qn::DataContainerStatCalculate(*container_collect));      
    return {};
  }

};

template<size_t M>
std::optional< Correlation<M> > MakeCorrelation( 
  TFile* file,
  const std::string& directory,
  const std::array<std::string, 2>& vector_names,
  const std::array<std::string, M>& component_names ) noexcept {
  if( !file ){
    std::cerr << "Warning: Input file is nullptr" << "\n";
    return {};
  }
  std::array<Qn::DataContainerStatCalculate, M> components{};
  size_t idx{};
  for (const auto &component : component_names){
    std::string name{};
    name.append(directory).append("/").append(vector_names[0]).append(".").append(vector_names[1]).append(".").append(component);
    std::string alt_name{};
    alt_name.append(directory).append("/").append(vector_names[1]).append(".").append(vector_names[0]).append(".").append(component);
    auto component_obj = Correlation<M>::ReadFromFile( file, name );
    if ( !component_obj ) {  
      component_obj = Correlation<M>::ReadFromFile( file, alt_name );
    }
    if( !component_obj ){
      std::cerr << "Warning: There is no object in the file with the name " << name << "\n";
      std::cerr << "Disregaring the whole correlation" << "\n";
      return {};
    }
    components[idx] = component_obj.value();
    idx++;
  }
  Correlation<M> result{};
  result.title_ = "";
  result.component_names_ = component_names;
  result.vector_names_ = std::vector<std::string>(vector_names.data(), vector_names.data()+2);
  result.components_ = std::move(components);
  return std::make_optional( result );
}

#endif //CALC2_CORRELATION_H
