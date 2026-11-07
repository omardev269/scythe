// BismIllahIRRahmaanIRRaheem
/* freefly camera declaration */

#ifndef SC_FREEFLYCAM_H
#define SC_FREEFLYCAM_H
#include <graphics/graphicsCore.h>
#include <hlsl++/matrix_float_type.h>
#include <hlsl++/vector_float_type.h>

namespace scythe {
	class FreeflyCamera {
		hlslpp::float3 m_Rotation;
		hlslpp::float3 m_Position;
		hlslpp::float3 m_RotationalDelta;
		hlslpp::float3 m_PositionalDelta;
		hlslpp::float4x4 m_Mat;
		float32 m_Speed;
	public:
		void Reset();
		void GetPosition(float3& oPos);
		void GetRotation(float3& oRot);
		void Teleport(float3 aPos);
		void SetRotation(float3 aRot);
		void Move(float3 aDelta);
		void Rotate(float3 aDelta);
		void SetSpeed(float32 aSpeed);
		void Update(float32 aDeltaTime, int32 aFOV, float32 aAspectRatio);
		void GetMatrix(matrix4x4& oMatrix);
	};
}

#endif // !SC_FREEFLYCAM_H