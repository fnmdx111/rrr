//
// Created by wo on 5/18/16.
//

#ifndef RRR_PORTABLE_H
#define RRR_PORTABLE_H

#include <xmmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>

typedef __m128 fp4v;
typedef __m128i ptr4v;
typedef __m128i i4v;
typedef float  fp1v;
#define MUL_fp4v(a, b) _mm_mul_ps(a, b)
#define ADD_fp4v(a, b) _mm_add_ps(a, b)
#define SUB_fp4v(a, b) _mm_sub_ps(a, b)
#define DIV_fp4v(a, b) _mm_div_ps(a, b)
#define SET_fp4v(a, b, c, d) _mm_set_ps(a, b, c, d)
#define RSQRT_fp4v(a) _mm_rsqrt_ps(a)
#define RCP_fp4v(a) _mm_rcp_ps(a)
#define LT_fp4v(a, b) _mm_cmplt_ps(a, b)
#define MMOV_fp4v(a, m, b) _mm_mask_mov_ps(a, m, b)
#define GT_fp4v(a, b) _mm_cmplt_ps(a, b)
#define AND_fp4v(a, b) _mm_and_ps(a, b)
#define ANDN_fp4v(a, b) _mm_andnot_ps(a, b)
#define OR_fp4v(a, b) _mm_or_ps(a, b)
#define SET1_fp4v(a) _mm_set1_ps(a)
#define ZERO_fp4v() SET1_fp4v(0.0)
#define SELECT2_fp4v(m, n, a) OR_fp4v(ANDN_fp4v(m, n), AND_fp4v(m, a))
// SELECT2_fp4v selects bits from #n# whose mask is 0, and from #a# whose mask
// is 1.
#define SELECT1_fp4v AND_fp4v
#define MIN_fp4v(a, b) _mm_min_ps(a, b)
#define CMP_MASK_fp4v(a, b, c) _mm_cmp_ps_mask(a, b, c)
#define LT_MASK_fp4v(a, b) CMP_MASK_fp4v(a, b, 17)
#define GT_MASK_fp4v(a, b) CMP_MASK_fp4v(a, b, 30)
#define XOR_fp4v(a, b) _mm_xor_ps(a, b)
#define NEG_fp4v(a)  SUB_fp4v(SET1_fp4v(0.0), a)
#define SQRT_fp4v(a) _mm_sqrt_ps(a)
#define EXTR_fp4v(a, i) _mm_extract_ps(a, i)

#define CVT_FP2I_fp4v(a) _mm_cvtps_epi32(a)
#define ALL_ZERO_i4v(a) (_mm_testz_si128(a, a) == 0)
#define FILTER_BY_i4v(cond, a) _mm_andnot_si128((__m128i) (cond), a)

#define FPFMT "%f"

#include <float.h>
#define FP1V_MAX FLT_MAX

#endif //RRR_PORTABLE_H
