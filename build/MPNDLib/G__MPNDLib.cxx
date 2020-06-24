// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__MPNDLib

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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/mpndglobals.hpp"
#include "/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp"

// Header files passed via #pragma extra_include

namespace MPND {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *MPND_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("MPND", 0 /*version*/, "mpndglobals.hpp", 10,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &MPND_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *MPND_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace MPND {
   namespace Detectors {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *MPNDcLcLDetectors_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("MPND::Detectors", 0 /*version*/, "mpndglobals.hpp", 13,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &MPNDcLcLDetectors_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *MPNDcLcLDetectors_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}
}

namespace MPND {
   namespace Filters {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *MPNDcLcLFilters_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("MPND::Filters", 0 /*version*/, "mpndglobals.hpp", 30,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &MPNDcLcLFilters_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *MPNDcLcLFilters_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}
}

namespace ROOT {
   static TClass *MPNDcLcLSimHit_Dictionary();
   static void MPNDcLcLSimHit_TClassManip(TClass*);
   static void *new_MPNDcLcLSimHit(void *p = 0);
   static void *newArray_MPNDcLcLSimHit(Long_t size, void *p);
   static void delete_MPNDcLcLSimHit(void *p);
   static void deleteArray_MPNDcLcLSimHit(void *p);
   static void destruct_MPNDcLcLSimHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MPND::SimHit*)
   {
      ::MPND::SimHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MPND::SimHit));
      static ::ROOT::TGenericClassInfo 
         instance("MPND::SimHit", "simhit.hpp", 15,
                  typeid(::MPND::SimHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MPNDcLcLSimHit_Dictionary, isa_proxy, 4,
                  sizeof(::MPND::SimHit) );
      instance.SetNew(&new_MPNDcLcLSimHit);
      instance.SetNewArray(&newArray_MPNDcLcLSimHit);
      instance.SetDelete(&delete_MPNDcLcLSimHit);
      instance.SetDeleteArray(&deleteArray_MPNDcLcLSimHit);
      instance.SetDestructor(&destruct_MPNDcLcLSimHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MPND::SimHit*)
   {
      return GenerateInitInstanceLocal((::MPND::SimHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MPND::SimHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MPNDcLcLSimHit_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MPND::SimHit*)0x0)->GetClass();
      MPNDcLcLSimHit_TClassManip(theClass);
   return theClass;
   }

   static void MPNDcLcLSimHit_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp");
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_MPNDcLcLSimHit(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::MPND::SimHit : new ::MPND::SimHit;
   }
   static void *newArray_MPNDcLcLSimHit(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::MPND::SimHit[nElements] : new ::MPND::SimHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_MPNDcLcLSimHit(void *p) {
      delete ((::MPND::SimHit*)p);
   }
   static void deleteArray_MPNDcLcLSimHit(void *p) {
      delete [] ((::MPND::SimHit*)p);
   }
   static void destruct_MPNDcLcLSimHit(void *p) {
      typedef ::MPND::SimHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MPND::SimHit

namespace {
  void TriggerDictionaryInitialization_libMPNDLib_Impl() {
    static const char* headers[] = {
"/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/mpndglobals.hpp",
"/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp",
0
    };
    static const char* includePaths[] = {
"/Applications/root_v6.16.00/include",
"/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include",
"/Applications/root_v6.16.00/include",
"/Users/suannchong/Desktop/Geant4/mpnd_suann/build/MPNDLib/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libMPNDLib dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace MPND{class __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(file_name@@@/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp")))  SimHit;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libMPNDLib dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/mpndglobals.hpp"
#include "/Users/suannchong/Desktop/Geant4/mpnd_suann/mpndlib/include/simhit.hpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"", payloadCode, "@",
"MPND::Detectors::ConfigString", payloadCode, "@",
"MPND::Detectors::NConfigs", payloadCode, "@",
"MPND::Filters::NTypes", payloadCode, "@",
"MPND::Filters::TypeString", payloadCode, "@",
"MPND::SimHit", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libMPNDLib",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libMPNDLib_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libMPNDLib_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libMPNDLib() {
  TriggerDictionaryInitialization_libMPNDLib_Impl();
}
