// BismIllahIRRahmaanIRRaheem
/* core engine definitions */

#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H
#include <runtime/engineCore_C.h>
#define SC_IFNOTNULLDISMISS(ptr) { if (ptr) { ptr->Dismiss(); ptr = NULL; } }
#include <math.h>
#include <emmintrin.h>
#include <xmmintrin.h>
struct float2 {
	float32 x, y;
	SC_INLINEFUNC void operator++() {
		x++;
		y++;
	}
	SC_INLINEFUNC void operator--() {
		x--;
		y--;
	}
	SC_INLINEFUNC void operator+=(const float2& aOther) {
		x += aOther.x;
		y += aOther.y;
	}
	SC_INLINEFUNC void operator-=(const float2& aOther) {
		x -= aOther.x;
		y -= aOther.y;
	}
	SC_INLINEFUNC void operator/=(const float2& aOther) {
		x /= aOther.x;
		y /= aOther.y;
	}
	SC_INLINEFUNC void operator*=(const float2& aOther) {
		x *= aOther.x;
		y *= aOther.y;
	}
	SC_INLINEFUNC float2 operator+(const float2& aOther) const {
		return { x + aOther.x, y + aOther.y };
	}
	SC_INLINEFUNC float2 operator-(const float2& aOther) const {
		return { x - aOther.x, y - aOther.y };
	}
	SC_INLINEFUNC float2 operator*(const float2& aOther) const {
		return { x * aOther.x, y * aOther.y };
	}
	SC_INLINEFUNC float2 operator/(const float2& aOther) const {
		return { x / aOther.x, y / aOther.y };
	}
	SC_INLINEFUNC float2 normalize() const {
		// checks for zeroes to not divide by zero
		float32 m_Length = sqrtf(x * x + y * y);
		return { x / m_Length, y / m_Length };
	}
	SC_INLINEFUNC float32 magnitude() const {
		// checks for zeroes to not divide by zero
		return sqrtf(x * x + y * y);
	}
};
struct float3 {
	float32 x, y, z;
	SC_INLINEFUNC void operator++() {
		x++;
		y++;
		z++;
	}
	SC_INLINEFUNC void operator--() {
		x--;
		y--;
		z--;
	}
	SC_INLINEFUNC void operator+=(const float3& aOther) {
		x += aOther.x;
		y += aOther.y;
		z += aOther.z;
	}
	SC_INLINEFUNC void operator-=(const float3& aOther) {
		x -= aOther.x;
		y -= aOther.y;
		z -= aOther.z;
	}
	SC_INLINEFUNC void operator/=(const float3& aOther) {
		x /= aOther.x;
		y /= aOther.y;
		z /= aOther.z;
	}
	SC_INLINEFUNC void operator*=(const float3& aOther) {
		x *= aOther.x;
		y *= aOther.y;
		z *= aOther.z;
	}
	SC_INLINEFUNC float3 operator+(const float3& aOther) const {
		return { x + aOther.x, y + aOther.y, z + aOther.z };
	}
	SC_INLINEFUNC float3 operator-(const float3& aOther) const {
		return { x - aOther.x, y - aOther.y, z - aOther.z };
	}
	SC_INLINEFUNC float3 operator*(const float3& aOther) const {
		return { x * aOther.x, y * aOther.y, z * aOther.z };
	}
	SC_INLINEFUNC float3 operator/(const float3& aOther) const {
		return { x / aOther.x, y / aOther.y, z / aOther.z };
	}
	SC_INLINEFUNC float3 normalize() const {
		// checks for zeroes to not divide by zero
		if (x == 0.0f && y == 0.0f && z == 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}
		float32 m_Length = sqrtf(x * x + y * y + z * z);
		return { x / m_Length, y / m_Length, z / m_Length };
	}
	SC_INLINEFUNC float32 magnitude() const {
		// checks for zeroes to not divide by zero
		if (x == 0.0f && y == 0.0f && z == 0.0f) {
			return 0.0f;
		}
		return sqrtf(x * x + y * y + z * z);
	}
};
struct alignas(16) float4 {
	float32 x, y, z, w;
#ifndef SC_X64
	SC_INLINEFUNC void operator++() {
		x++;
		y++;
		z++;
		w++;
	}
	SC_INLINEFUNC void operator--() {
		x--;
		y--;
		z--;
		w--;
	}
	SC_INLINEFUNC void operator+=(const float4& aOther) {
		x += aOther.x;
		y += aOther.y;
		z += aOther.z;
		w += aOther.w;
	}
	SC_INLINEFUNC void operator-=(const float4& aOther) {
		x -= aOther.x;
		y -= aOther.y;
		z -= aOther.z;
		w -= aOther.w;
	}
	SC_INLINEFUNC void operator/=(const float4& aOther) {
		x /= aOther.x;
		y /= aOther.y;
		z /= aOther.z;
		w /= aOther.w;
	}
	SC_INLINEFUNC void operator*=(const float4& aOther) {
		x *= aOther.x;
		y *= aOther.y;
		z *= aOther.z;
		w *= aOther.w;
	}
	SC_INLINEFUNC float4 operator+(const float4& aOther) const {
		return { x + aOther.x, y + aOther.y, z + aOther.z, w + aOther.w };
	}
	SC_INLINEFUNC float4 operator-(const float4& aOther) const {
		return { x - aOther.x, y - aOther.y, z - aOther.z, w - aOther.w };
	}
	SC_INLINEFUNC float4 operator*(const float4& aOther) const {
		return { x * aOther.x, y * aOther.y, z * aOther.z, w * aOther.w };
	}
	SC_INLINEFUNC float4 operator/(const float4& aOther) const {
		return { x / aOther.x, y / aOther.y, z / aOther.z, w / aOther.w };
	}
	SC_INLINEFUNC float4 normalize() const {
		// checks for zeroes to not divide by zero
		if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) {
			return { 0.0f, 0.0f, 0.0f, 0.0f };
		}
		float32 m_Length = sqrtf(x * x + y * y + z * z + w * w);
		return { x / m_Length, y / m_Length, z / m_Length, w / m_Length };
	}
	SC_INLINEFUNC float32 magnitude() const {
		// checks for zeroes to not divide by zero
		if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) {
			return 0.0f;
		}
		return sqrtf(x * x + y * y + z * z + w * w);
	}
#else
	SC_INLINEFUNC void operator++() {
		x++;
		y++;
		z++;
		w++;
	}
	SC_INLINEFUNC void operator--() {
		x--;
		y--;
		z--;
		w--;
	}
	SC_INLINEFUNC void operator+=(const float4& aOther) {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_add_ps(m_This, m_Other);
		_mm_store_ps(&x, m_Result);
	}
	SC_INLINEFUNC void operator-=(const float4& aOther) {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_sub_ps(m_This, m_Other);
		_mm_store_ps(&x, m_Result);
	}
	SC_INLINEFUNC void operator/=(const float4& aOther) {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_div_ps(m_This, m_Other);
		_mm_store_ps(&x, m_Result);
	}
	SC_INLINEFUNC void operator*=(const float4& aOther) {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_mul_ps(m_This, m_Other);
		_mm_store_ps(&x, m_Result);
	}
	SC_INLINEFUNC float4 operator+(const float4& aOther) const {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_add_ps(m_This, m_Other);
		float4 result;
		_mm_store_ps(&result.x, m_Result);
		return result;
	}
	SC_INLINEFUNC float4 operator-(const float4& aOther) const {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_sub_ps(m_This, m_Other);
		float4 result;
		_mm_store_ps(&result.x, m_Result);
		return result;
	}
	SC_INLINEFUNC float4 operator*(const float4& aOther) const {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_mul_ps(m_This, m_Other);
		float4 result;
		_mm_store_ps(&result.x, m_Result);
		return result;
	}
	SC_INLINEFUNC float4 operator/(const float4& aOther) const {
		__m128 m_This = _mm_load_ps(&x);
		__m128 m_Other = _mm_load_ps(&aOther.x);
		__m128 m_Result = _mm_div_ps(m_This, m_Other);
		float4 result;
		_mm_store_ps(&result.x, m_Result);
		return result;
	}
	SC_INLINEFUNC float4 normalize() const {
		// checks for zeroes to not divide by zero
		if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) {
			return { 0.0f, 0.0f, 0.0f, 0.0f };
		}
		float32 m_Length = sqrtf(x * x + y * y + z * z + w * w);
		return { x / m_Length, y / m_Length, z / m_Length, w / m_Length };
	}
	SC_INLINEFUNC float32 magnitude() const {
		// checks for zeroes to not divide by zero
		if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) {
			return 0.0f;
		}
		return sqrtf(x * x + y * y + z * z + w * w);
	}
#endif // !SC_X64
};
struct int2 {
	int32 x, y;
};
struct int3 {
	int32 x, y, z;
};
struct int4 {
	int32 x, y, z, w;
};
struct uint2 {
	uint32 x, y;
};
struct uint3 {
	uint32 x, y, z;
};
struct uint4 {
	uint32 x, y, z, w;
};
#endif // !ENGINE_CORE_H