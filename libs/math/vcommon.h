//
// Created by wo on 5/21/16.
//

#ifndef RRR_VCOMMON_H
#define RRR_VCOMMON_H

#include "vec.h"
#include "vecp.h"

void
vp4_tov1(v31v* r, v34v* p, char type);

void
v1_tovp4(v34v* r, v31v* p);

void
p_v34v(v34v* r);

void
p_v31v(v31v* r);

#endif //RRR_VCOMMON_H
