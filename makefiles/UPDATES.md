Update log v0.6:
    - Added gcc, clang, clang-msvc and msvc toolchain support with TOOLCHAIN variable.
    - Moved targets.mk content into config.mk
    - Splitted Makefile into several .mk files in makefiles/
    - Removed Update logs embedded in Makefile
	- Github repo created
	- ENABLE-SANITIZERS forces debug mode
	- Improved documentation
	- Added README.md
	- Added DISABLE_DEPRACATED_WARNINGS in config.mk

Update log v0.5:
	- Changed project layout:
		~ Release binaries lives in bin/ obj/ lib/
		~ Debug binaries lives in debug/bin/ debug/obj/ debug/lib/
	- Build commands now are sumarized using echo:
		~ Object prints compiler and src file
		~ Executable prints compiler and output file
		~ Library prints linker and output file
	- Cleaner structure
	- CVERSION moved to config.mk
	- Deleted USER_LIBS and USER_LIB_PATHS, read targets.mk for more info
	- Renamed USER_CFLAGS and USER_INCLUDES to EXTRA_CFLAGS and EXTRA_INCLUDES
	- Deleted USER_ prefix for USER_PRE_BUILD, USER_POST_BUILD and USER_ENABLE_*
	- Added documentation in targets.mk and config.mk
	- Each .c file in test folder is compiled as an executable into bin/test, read targets.mk for   more info
	- INCLUDES, LIBS and LIBS_PATH dont require manual -I, -l and -L prefix
	- Added debug, tests and tests-debug phonies
	- TODO: Add shared libraries compilation
	
Update log v0.4:
    - Update logs were introduced
    - Toolchain variables now are setted with ?=
    - RELEASE_CFLAGS uses -O2 instead of -O3
    - Standard build mode changed to release
    - Now is possible to compile multiple targets
    - TODO: Add shared libraries compilation