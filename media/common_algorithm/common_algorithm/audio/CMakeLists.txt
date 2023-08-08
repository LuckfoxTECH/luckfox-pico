#1.cmake version
cmake_minimum_required(VERSION 3.2)

if (AEC_ANR_AGC_ENABLE OR ANR_ENABLE)
	add_subdirectory(rkap_common)
endif()

if (AEC_ANR_AGC_ENABLE)
	add_subdirectory(rkap_3a)
endif()

if (ANR_ENABLE)
	add_subdirectory(rkap_anr)
endif()

if (RKAPPLUS_ENABLE)
	add_subdirectory(rkskv)
endif()

if (RKAPPLUS_AED_ENABLE)
	add_subdirectory(rkaed)
endif()

if (USE_UCLIBC)
	if(RKAPPLUS_AED_ENABLE OR RKAPPLUS_ENABLE)
		add_subdirectory(rkaudio_algorithms)
	endif()
endif()
