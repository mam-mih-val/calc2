// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__Calculator
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "src/correlation.h"
#include "src/functions.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *CorrelationlE4gR_Dictionary();
   static void CorrelationlE4gR_TClassManip(TClass*);
   static void delete_CorrelationlE4gR(void *p);
   static void deleteArray_CorrelationlE4gR(void *p);
   static void destruct_CorrelationlE4gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Correlation<4>*)
   {
      ::Correlation<4> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Correlation<4>));
      static ::ROOT::TGenericClassInfo 
         instance("Correlation<4>", "src/correlation.h", 18,
                  typeid(::Correlation<4>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CorrelationlE4gR_Dictionary, isa_proxy, 4,
                  sizeof(::Correlation<4>) );
      instance.SetDelete(&delete_CorrelationlE4gR);
      instance.SetDeleteArray(&deleteArray_CorrelationlE4gR);
      instance.SetDestructor(&destruct_CorrelationlE4gR);

      ::ROOT::AddClassAlternate("Correlation<4>","Correlation<4ul>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Correlation<4>*)
   {
      return GenerateInitInstanceLocal((::Correlation<4>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Correlation<4>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CorrelationlE4gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Correlation<4>*)0x0)->GetClass();
      CorrelationlE4gR_TClassManip(theClass);
   return theClass;
   }

   static void CorrelationlE4gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CorrelationlE3gR_Dictionary();
   static void CorrelationlE3gR_TClassManip(TClass*);
   static void delete_CorrelationlE3gR(void *p);
   static void deleteArray_CorrelationlE3gR(void *p);
   static void destruct_CorrelationlE3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Correlation<3>*)
   {
      ::Correlation<3> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Correlation<3>));
      static ::ROOT::TGenericClassInfo 
         instance("Correlation<3>", "src/correlation.h", 18,
                  typeid(::Correlation<3>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CorrelationlE3gR_Dictionary, isa_proxy, 4,
                  sizeof(::Correlation<3>) );
      instance.SetDelete(&delete_CorrelationlE3gR);
      instance.SetDeleteArray(&deleteArray_CorrelationlE3gR);
      instance.SetDestructor(&destruct_CorrelationlE3gR);

      ::ROOT::AddClassAlternate("Correlation<3>","Correlation<3ul>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Correlation<3>*)
   {
      return GenerateInitInstanceLocal((::Correlation<3>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Correlation<3>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CorrelationlE3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Correlation<3>*)0x0)->GetClass();
      CorrelationlE3gR_TClassManip(theClass);
   return theClass;
   }

   static void CorrelationlE3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CorrelationlE2gR_Dictionary();
   static void CorrelationlE2gR_TClassManip(TClass*);
   static void delete_CorrelationlE2gR(void *p);
   static void deleteArray_CorrelationlE2gR(void *p);
   static void destruct_CorrelationlE2gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Correlation<2>*)
   {
      ::Correlation<2> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Correlation<2>));
      static ::ROOT::TGenericClassInfo 
         instance("Correlation<2>", "src/correlation.h", 18,
                  typeid(::Correlation<2>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CorrelationlE2gR_Dictionary, isa_proxy, 4,
                  sizeof(::Correlation<2>) );
      instance.SetDelete(&delete_CorrelationlE2gR);
      instance.SetDeleteArray(&deleteArray_CorrelationlE2gR);
      instance.SetDestructor(&destruct_CorrelationlE2gR);

      ::ROOT::AddClassAlternate("Correlation<2>","Correlation<2ul>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Correlation<2>*)
   {
      return GenerateInitInstanceLocal((::Correlation<2>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Correlation<2>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CorrelationlE2gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Correlation<2>*)0x0)->GetClass();
      CorrelationlE2gR_TClassManip(theClass);
   return theClass;
   }

   static void CorrelationlE2gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CorrelationlE1gR_Dictionary();
   static void CorrelationlE1gR_TClassManip(TClass*);
   static void delete_CorrelationlE1gR(void *p);
   static void deleteArray_CorrelationlE1gR(void *p);
   static void destruct_CorrelationlE1gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Correlation<1>*)
   {
      ::Correlation<1> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Correlation<1>));
      static ::ROOT::TGenericClassInfo 
         instance("Correlation<1>", "src/correlation.h", 18,
                  typeid(::Correlation<1>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CorrelationlE1gR_Dictionary, isa_proxy, 4,
                  sizeof(::Correlation<1>) );
      instance.SetDelete(&delete_CorrelationlE1gR);
      instance.SetDeleteArray(&deleteArray_CorrelationlE1gR);
      instance.SetDestructor(&destruct_CorrelationlE1gR);

      ::ROOT::AddClassAlternate("Correlation<1>","Correlation<1ul>");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Correlation<1>*)
   {
      return GenerateInitInstanceLocal((::Correlation<1>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Correlation<1>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CorrelationlE1gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Correlation<1>*)0x0)->GetClass();
      CorrelationlE1gR_TClassManip(theClass);
   return theClass;
   }

   static void CorrelationlE1gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Functions_Dictionary();
   static void Functions_TClassManip(TClass*);
   static void delete_Functions(void *p);
   static void deleteArray_Functions(void *p);
   static void destruct_Functions(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Functions*)
   {
      ::Functions *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Functions));
      static ::ROOT::TGenericClassInfo 
         instance("Functions", "src/functions.h", 12,
                  typeid(::Functions), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Functions_Dictionary, isa_proxy, 4,
                  sizeof(::Functions) );
      instance.SetDelete(&delete_Functions);
      instance.SetDeleteArray(&deleteArray_Functions);
      instance.SetDestructor(&destruct_Functions);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Functions*)
   {
      return GenerateInitInstanceLocal((::Functions*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Functions*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Functions_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Functions*)0x0)->GetClass();
      Functions_TClassManip(theClass);
   return theClass;
   }

   static void Functions_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CorrelationlE4gR(void *p) {
      delete ((::Correlation<4>*)p);
   }
   static void deleteArray_CorrelationlE4gR(void *p) {
      delete [] ((::Correlation<4>*)p);
   }
   static void destruct_CorrelationlE4gR(void *p) {
      typedef ::Correlation<4> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Correlation<4>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CorrelationlE3gR(void *p) {
      delete ((::Correlation<3>*)p);
   }
   static void deleteArray_CorrelationlE3gR(void *p) {
      delete [] ((::Correlation<3>*)p);
   }
   static void destruct_CorrelationlE3gR(void *p) {
      typedef ::Correlation<3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Correlation<3>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CorrelationlE2gR(void *p) {
      delete ((::Correlation<2>*)p);
   }
   static void deleteArray_CorrelationlE2gR(void *p) {
      delete [] ((::Correlation<2>*)p);
   }
   static void destruct_CorrelationlE2gR(void *p) {
      typedef ::Correlation<2> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Correlation<2>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CorrelationlE1gR(void *p) {
      delete ((::Correlation<1>*)p);
   }
   static void deleteArray_CorrelationlE1gR(void *p) {
      delete [] ((::Correlation<1>*)p);
   }
   static void destruct_CorrelationlE1gR(void *p) {
      typedef ::Correlation<1> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Correlation<1>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Functions(void *p) {
      delete ((::Functions*)p);
   }
   static void deleteArray_Functions(void *p) {
      delete [] ((::Functions*)p);
   }
   static void destruct_Functions(void *p) {
      typedef ::Functions current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Functions

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 386,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace {
  void TriggerDictionaryInitialization_libCalculator_Impl() {
    static const char* headers[] = {
"src/correlation.h",
"src/functions.h",
0
    };
    static const char* includePaths[] = {
"/mnt/c/Users/Misha/CLionProjects/calc2",
"/home/mikhail/boost_1_80_0",
"/home/mikhail/root-6.24.06/install/include",
"/home/mikhail/QnTools/install/lib/cmake/QnTools/../../../include/QnTools",
"/home/mikhail/root-6.24.06/install/include/",
"/mnt/c/Users/Misha/CLionProjects/calc2/cmake-build-debug/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCalculator dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$src/functions.h")))  Functions;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCalculator dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "src/correlation.h"
#include "src/functions.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Correlation<1>", payloadCode, "@",
"Correlation<2>", payloadCode, "@",
"Correlation<3>", payloadCode, "@",
"Correlation<4>", payloadCode, "@",
"Functions", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCalculator",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCalculator_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCalculator_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCalculator() {
  TriggerDictionaryInitialization_libCalculator_Impl();
}
