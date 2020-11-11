#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MATCHER_LIB)
#  define MATCHER_EXPORT Q_DECL_EXPORT
# else
#  define MATCHER_EXPORT Q_DECL_IMPORT
# endif
#else
# define MATCHER_EXPORT
#endif
