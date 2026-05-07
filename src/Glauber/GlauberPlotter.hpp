#ifndef CAP__GlauberPlotter
#define CAP__GlauberPlotter
#include "TObject.h"

namespace CAP
{

void draw(double xs,
          int participantColor = 4,
          int participantStyle = 1000,
          int spectatorColor   = 2,
          int spectatorStyle   = 1001);

}

#endif // CAP__GlauberPoint



