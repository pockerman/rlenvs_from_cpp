find_program(
  SPHINX_EXECUTABLE
  NAMES sphinx-build
  DOC "Path to sphinx-build executable")
  
# handle REQUIRD and QUIET arguments, set SPHINX_FOUND variable
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
Sphinx "Unable to locate sphinx-build executable" SPHINX_EXECUTABLE)
