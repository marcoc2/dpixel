#ifndef _XBR_h
#define _XBR_h

#include <sys/types.h>

struct SDL_Surface;

namespace xbr{
//    void xbr2x(SDL_Surface * input, SDL_Surface * output);
//    void xbr3x(SDL_Surface * input, SDL_Surface * output);
    void xbr4x(u_char* input, u_char* output, u_int inputWidth, u_int inputHeight);
}

#endif
