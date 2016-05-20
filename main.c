#include <stdio.h>
#include "libs/vecp.h"

int
main()
{
  MAKE_VP4(vs1);
  MAKE_VP4(vs2);

  vp4_load4(vs1,
            0.1, 0.2, 0.3,
            0.3, 0.2, 0.1,
            0.2, 0.2, 0.2,
            0.3, 0.1, 0.2);
  vp4_load4(vs2,
            0.3, 0.3, 0.3,
            0.2, 0.2, 0.2,
            0.1, 0.1, 0.1,
            0.1, 0.2, 0.3);

  fp4v ret = vp4_dot(vs1, vs2);

  printf("%f %f %f %f\n",
         ATTR_FV4(ret, 0), ATTR_FV4(ret, 1),
         ATTR_FV4(ret, 2), ATTR_FV4(ret, 3));

  MAKE_VP4(vs3);
  vp4_cross(vs3, vs1, vs2);
  vp4_normalize(vs3);

  FREE_VP4(vs1);
  FREE_VP4(vs2);
  FREE_VP4(vs3);

  return 0;
}
