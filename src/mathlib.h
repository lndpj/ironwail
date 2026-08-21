/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2007-2008 Kristian Duske
Copyright (C) 2010-2014 QuakeSpasm developers

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __MATHLIB_H
#define __MATHLIB_H

// mathlib.h

#include <math.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_DIV_180 (M_PI / 180.0) //johnfitz
#define DEG2RAD(a)   ((a) * M_PI_DIV_180)
#define RAD2DEG(a)   ((a) * (180.0 / M_PI))

struct mplane_s;

extern vec3_t vec3_origin;
extern vec4_t vec4_origin;

#define	nanmask		(255 << 23)	/* 7F800000 */
#if 0	/* macro is violating strict aliasing rules */
#define	IS_NAN(x)	(((*(int *) (char *) &x) & nanmask) == nanmask)
#else
static inline int IS_NAN (float x) {
	union { float f; int i; } num;
	num.f = x;
	return ((num.i & nanmask) == nanmask);
}
#endif

#define Q_rint(x) ((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5)) //johnfitz -- from joequake

#define vadd(n,dst,a,b) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = (a)[i] + (b)[i]; \
} while(0)

#define vsub(n,dst,a,b) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = (a)[i] - (b)[i]; \
} while(0)

#define vfma(n,dst,a,b,c) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = (a)[i] * (b)[i] + (c)[i]; \
} while(0)

#define vsma(n,a,s,b,dst) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = (a)[i] + s * (b)[i]; \
} while(0)

#define vset(dst,...) \
do { \
const size_t n = countargs(__VA_ARGS__); \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = (__typeof__(__VA_ARGS__)[]){ __VA_ARGS__ }[i]; \
} while (0)

#define vfloor(n,dst,src) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = (typeof((dst)[0]))floor((double)((src)[i])); \
} while(0)

#define vclamp(n,dst,minval,maxval) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < n; i++) \
	(dst)[i] = ((dst)[i] < (minval)) ? (minval) : ((dst)[i] > (maxval)) ? (maxval) : (dst)[i]; \
} while(0)

#define vcross3(dst,a,b) \
do { \
_Pragma("omp simd") \
for(size_t i = 0; i < 3; i++) \
	(dst)[i] = (a)[(1+i) % 3] * (b)[(2+i) % 3] - (a)[(2+i) % 3] * (b)[(1+i) % 3]; \
} while(0)

#define vdot2(x,y)      ((x)[0]*(y)[0]+(x)[1]*(y)[1])
#define vdot3(x,y)	((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define vdot4(x,y)	((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2]+(x)[3]*(y)[3])
#define DoublePrecisionDotProduct(x,y)	((double)(x)[0]*(y)[0]+(double)(x)[1]*(y)[1]+(double)(x)[2]*(y)[2])

#define VectorCopy(src,dst)				do {(dst)[0]=(src)[0];(dst)[1]=(src)[1];(dst)[2]=(src)[2];} while (0)

#define vlensqr(v)   vdot3(v,v)
#define vlen(v) sqrt(vdot3(v,v))

//johnfitz -- courtesy of lordhavoc
// QuakeSpasm: To avoid strict aliasing violations, use a float/int union instead of type punning.
#define VectorNormalizeFast(_v)\
do\
{\
	union { float f; int i; } _y, _number;\
	_number.f = vdot3((_v), (_v));\
	if (_number.f != 0.0)\
	{\
		_y.i = 0x5f3759df - (_number.i >> 1);\
		_y.f = _y.f * (1.5f - (_number.f * 0.5f * _y.f * _y.f));\
		VectorScale((_v), _y.f, (_v));\
	}\
} while (0)

static FUNC_INLINE float anglemod(float a)
{
#if 0
	if (a >= 0)
		a -= 360*(int)(a/360);
	else
		a += 360*( 1 + (int)(-a/360) );
#endif
	a = (360.0/65536) * ((int)(a*(65536/360.0)) & 65535);
	return a;
}

/*
==================
NormalizeAngle

Returns a value between -180 and 180
==================
*/
static FUNC_INLINE float NormalizeAngle (float degrees)
{
	degrees += 180.f;
	// Note: can't use fmod because of the way it handles negative values
	degrees -= floor (degrees * (1.f/360.f)) * 360.f;
	degrees -= 180.f;
	return degrees;
}

/*
==================
AngleDifference

Returns a value between -180 and 180
==================
*/
static FUNC_INLINE float AngleDifference (float dega, float degb)
{
	return NormalizeAngle (dega - degb);
}

/*
==================
LerpAngle

Returns a value between -180 and 180
==================
*/
static FUNC_INLINE float LerpAngle (float degfrom, float degto, float frac)
{
	return NormalizeAngle (degfrom + AngleDifference (degto, degfrom) * frac);
}


//johnfitz -- the opposite of AngleVectors.  this takes forward and generates pitch yaw roll
//TODO: take right and up vectors to properly set yaw and roll
static FUNC_INLINE void VectorAngles (const vec3_t forward, vec3_t angles)
{
	vec3_t temp;

	temp[0] = forward[0];
	temp[1] = forward[1];
	temp[2] = 0;
	angles[PITCH] = -atan2(forward[2], vlen(temp)) / M_PI_DIV_180;
	angles[YAW] = atan2(forward[1], forward[0]) / M_PI_DIV_180;
	angles[ROLL] = 0;
}

static FUNC_INLINE void AngleVectors (vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	angle = angles[YAW] * (M_PI*2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI*2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI*2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;
	right[0] = (-1*sr*sp*cy+-1*cr*-sy);
	right[1] = (-1*sr*sp*sy+-1*cr*cy);
	right[2] = -1*sr*cp;
	up[0] = (cr*sp*cy+-sr*-sy);
	up[1] = (cr*sp*sy+-sr*cy);
	up[2] = cr*cp;
}

static FUNC_INLINE int VectorCompare (const vec3_t v1, const vec3_t v2)
{
	int		i;

	for (i=0 ; i<3 ; i++)
		if (v1[i] != v2[i])
			return 0;

	return 1;
}

static FUNC_INLINE void VectorLerp (const vec3_t veca, const vec3_t vecb, float frac, vec3_t dst)
{
	dst[0] = LERP (veca[0], vecb[0], frac);
	dst[1] = LERP (veca[1], vecb[1], frac);
	dst[2] = LERP (veca[2], vecb[2], frac);
}

static FUNC_INLINE float VectorNormalize (vec3_t v)
{
	float	length, ilength;

	length = sqrt(vdot3(v,v));

	if (length)
	{
		ilength = 1/length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
}

static FUNC_INLINE float DistanceSquared (const vec3_t a, const vec3_t b)
{
	vec3_t ab;
	vsub(3,ab,b,a);
	return vlensqr (ab);
}

static FUNC_INLINE float Distance (const vec3_t a, const vec3_t b)
{
	return sqrt (DistanceSquared (a, b));
}

static FUNC_INLINE void VectorInverse (vec3_t v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

static FUNC_INLINE void VectorScale (const vec3_t in, vec_t scale, vec3_t out)
{
	out[0] = in[0]*scale;
	out[1] = in[1]*scale;
	out[2] = in[2]*scale;
}

int Q_log2(int val);
int Q_nextPow2(int val);

float GetFraction (float val, float minval, float maxval);
float GetClampedFraction (float val, float minval, float maxval);

float Log2f (float val);
float Exp2f (float val);
float GetLogFraction (float val, float minval, float maxval);
float GetClampedLogFraction (float val, float minval, float maxval);
float LogLerp (float minval, float maxval, float t);

float EaseInOut (float t);

uint32_t Interleave0 (uint16_t x);
uint32_t Interleave (uint16_t even, uint16_t odd);
uint16_t DeinterleaveEven (uint32_t x);
void DecodeMortonIndex (uint16_t index, int *x, int *y);

void R_ConcatRotations (float in1[3][3], float in2[3][3], float out[3][3]);
void R_ConcatTransforms (float in1[3][4], float in2[3][4], float out[3][4]);

void Matrix3x4_RM_Transform4 (const float *matrix, const float *vector, float *product);
void Matrix3x4_RM_Transform3 (const float *matrix, const float *vector, float *product);

void FloorDivMod (double numer, double denom, int *quotient,
		int *rem);
fixed16_t Invert24To16(fixed16_t val);
int GreatestCommonDivisor (int i1, int i2);

void AngleVectors (vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, struct mplane_s *plane);

void MatrixMultiply(float left[16], float right[16]);
void RotationMatrix(float matrix[16], float angle, int axis);
void TranslationMatrix(float matrix[16], float x, float y, float z);
void ScaleMatrix(float matrix[16], float x, float y, float z);
void IdentityMatrix(float matrix[16]);
void ApplyScale(float matrix[16], float x, float y, float z);
void ApplyTranslation(float matrix[16], float x, float y, float z);
void MatrixTranspose4x3(const float src[16], float dst[12]);
void ProjectVector(const vec3_t src, const float matrix[16], vec3_t dst);

qboolean RayVsBox (const vec3_t org, const vec3_t rcpdelta, const vec3_t mins, const vec3_t maxs, float *frac);

#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
	(((p)->type < 3)?						\
	(										\
		((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))

/*==========================================================================*/

/* SIMD */
#if (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))) || (defined(__GNUC__) && defined(__SSE__) && defined(__SSE2__))
	#define USE_SIMD
	#define USE_SSE2
	#include <emmintrin.h>
#endif

/*==========================================================================*/

#endif	/* __MATHLIB_H */

