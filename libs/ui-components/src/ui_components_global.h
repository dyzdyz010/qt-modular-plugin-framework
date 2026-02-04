#pragma once

#include <QtCore/qglobal.h>

#if defined(MPF_UI_COMPONENTS_EXPORTS)
#  define MPF_UI_COMPONENTS_EXPORT Q_DECL_EXPORT
#else
#  define MPF_UI_COMPONENTS_EXPORT Q_DECL_IMPORT
#endif
