#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DETECTOR_LIB)
#  define DETECTOR_EXPORT Q_DECL_EXPORT
# else
#  define DETECTOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define DETECTOR_EXPORT
#endif
