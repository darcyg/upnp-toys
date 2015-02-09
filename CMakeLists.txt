cmake_minimum_required (VERSION 2.6)
find_library (UPNP_UPNP_LIBRARY upnp)

project (upnp-toys)
add_executable (upnp-discover upnp-discover.c)
target_link_libraries (upnp-discover ${UPNP_UPNP_LIBRARY})
