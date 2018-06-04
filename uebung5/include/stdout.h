
#ifndef STDOUT_H
#define STDOUT_H

#define NO_COPY_INSTANCE(cls) \
cls(const cls&);\
cls& operator =(const cls&);

#define CEIL_DIV(x, y) (x/y + (x % y != 0))

#ifndef STDOUT
  #ifdef STDOUT_FILENO
  #define STDOUT STDOUT_FILENO
  #else
  #define STDOUT 1
  #endif
#endif
#endif
