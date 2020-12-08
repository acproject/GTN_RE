#ifndef GTN_RE_GLOBAL_H
#define GTN_RE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GTN_RE_LIBRARY)
#  define GTN_RESHARED_EXPORT Q_DECL_EXPORT
#else
#  define GTN_RESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GTN_RE_GLOBAL_H
