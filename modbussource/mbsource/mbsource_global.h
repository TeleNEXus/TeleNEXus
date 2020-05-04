#ifndef MBSOURCE_GLOBAL_H
#define MBSOURCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MBSOURCE_LIBRARY)
#  define MBSOURCE_EXPORT Q_DECL_EXPORT
#else
#  define MBSOURCE_EXPORT Q_DECL_IMPORT
#endif

#endif // MBSOURCE_GLOBAL_H
