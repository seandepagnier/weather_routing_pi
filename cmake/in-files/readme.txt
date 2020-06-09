The files in this directory are not executed but are transformed or used by the build process.

version.h.in is updated to include the current settings from CMakeLists.txt and put in a location that
is part of the include directory set. Source code that needs these settings just have to use:
#include "version.h"
at/near the top of the source code file.

the wxWTranslateCatalog.h.in file is used to ensure that the plugins language files are accessed rather than
OpenCPN language files. This allows for change of context of words to be applied correctly. Without this there
can be confusion when a language uses a different word translation in different contexts.
