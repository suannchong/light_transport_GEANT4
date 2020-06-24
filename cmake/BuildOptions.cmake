# Lists of flags for various build types
set(GCC_OPT_FLAG "-O2")
set(GCC_DEB_FLAG "-g")
#set(WARN_FLAGS "-Wall -Wextra -Wpedantic -Weffc++ -Wdouble-promotion -Wformat -Wswitch-default -Wswitch-enum -Wsync-nand -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wtrampolines -Wfloat-equal -Wshadow -Wunsafe-loop-optimizations -Wpointer-arith -Wtype-limits -Wcast-qual -Wcast-align -Wwrite-strings -Wconditionally-supported -Wconversion -Wzero-as-null-pointer-constant -Wdate-time -Wuseless-cast -Wenum-compare -Wlogical-op -Wmissing-declarations -Wmissing-field-initializers -Wnormalized -Wopenmp-simd -Wpacked -Wpadded -Wredundant-decls -Winline -Winvalid-pch -Wvector-operation-performance -Wvla -Wdisabled-optimization -Wopenmp-simd")
set(WARN_FLAGS "-Wall -Wextra -Wpedantic")
set(WARN_OPT_FLAGS "-fstrict-aliasing -Wstrict-aliasing -fstrict-overflow -Wstrict-overflow=5 -ftree-vrp -Warray-bounds=2 -fsized-deallocation -Wsized-deallocation -flto-odr-type-merging -Wno-odr -fstack-protector -Wstack-protector")

# a little hack to make Qt Creator a little happier when it deals with
# different build types using this as a project file
# check for each supported build type and add the appropriate g++ flags
if(BUILD_TYPE STREQUAL "Release")
  add_definitions(${GCC_OPT_FLAG})
elseif(BUILD_TYPE STREQUAL "Debug")
  add_definitions(${GCC_DEB_FLAG})
elseif(BUILD_TYPE STREQUAL "OptDebug")
  add_definitions(${GCC_DEB_FLAG})
  add_definitions(${GCC_OPT_FLAG})
elseif(BUILD_TYPE STREQUAL "Warn")
  add_definitions(${WARN_FLAGS})
elseif(BUILD_TYPE STREQUAL "Warn_Opt")
  add_definitions(${WARN_FLAGS})
  add_definitions(${WARN_OPT_FLAGS})
else()
  add_definitions(${WARN_FLAGS})
  add_definitions(${GCC_DEB_FLAG})
  add_definitions(${GCC_OPT_FLAG})
endif(BUILD_TYPE STREQUAL "Release")
