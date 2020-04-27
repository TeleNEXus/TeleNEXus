#ifndef EXTWIDGLIB_GLOBAL_H
#define EXTWIDGLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EXTWIDGLIB_LIBRARY)
#  define EXTWIDGLIB_EXPORT Q_DECL_EXPORT
#else
#  define EXTWIDGLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // EXTWIDGLIB_GLOBAL_H
