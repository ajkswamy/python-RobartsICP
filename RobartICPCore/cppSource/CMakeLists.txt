CMAKE_MINIMUM_REQUIRED( VERSION 2.2 )

PROJECT( ASICP_MD_ANN )


INCLUDE_DIRECTORIES(
  # D:/chene/src/ann_1.1.2/include
  )

LINK_DIRECTORIES(
 # D:/chene/src/ann_1.1.2/MS_Win32/dll/Release
  )

set(CMAKE_BUILD_TYPE Debug)

ADD_EXECUTABLE( ASICP_MD_ANN
  asicp_md_main_ANN.cpp
  mathUtils.cpp
  mathUtils.h
  matrix.h
  jacobi.h
  OPP.cpp
  OPP.h
  svd.h
  ASOPP_Major.cpp
  ASOPP_Major.h
  asicp_md_ann.cpp
  asicp_md_ann.h
)
TARGET_LINK_LIBRARIES( ASICP_MD_ANN
  ann
)

ADD_EXECUTABLE( calcFRE
  calcFRE.cpp
  matrix.h
  )
  
TARGET_LINK_LIBRARIES( calcFRE
  ann
  )
