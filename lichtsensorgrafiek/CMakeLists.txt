# =============================================================
# BESTAND: CMakeLists.txt  (root van het project)
# ROL:     Vertelt ESP-IDF dat dit een project is en waar
#          de broncode te vinden is.
# =============================================================

cmake_minimum_required(VERSION 3.16)

# Projectnaam (vrij te kiezen)
project(lichtsensor_project)

# Verplichte ESP-IDF include
include($ENV{IDF_PATH}/tools/cmake/project.cmake)