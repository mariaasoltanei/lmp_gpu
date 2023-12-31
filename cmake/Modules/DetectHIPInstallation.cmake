if(NOT DEFINED HIP_PATH)
    if(NOT DEFINED ENV{HIP_PATH})
        message(FATAL_ERROR "HIP support requires HIP_PATH to be defined.\n"
        "Either pass the HIP_PATH as a CMake option via -DHIP_PATH=... or set the HIP_PATH environment variable.")
    else()
        set(HIP_PATH $ENV{HIP_PATH} CACHE PATH "Path to HIP installation")
    endif()
endif()
if(NOT DEFINED ROCM_PATH)
    if(NOT DEFINED ENV{ROCM_PATH})
        set(ROCM_PATH "/opt/rocm" CACHE PATH "Path to ROCm installation")
    else()
        set(ROCM_PATH $ENV{ROCM_PATH} CACHE PATH "Path to ROCm installation")
    endif()
endif()
list(APPEND CMAKE_PREFIX_PATH ${HIP_PATH} ${ROCM_PATH})
