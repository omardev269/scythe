// BismIllahIRRahmaanIRRaheem
/* freefly camera implementation */

#include <graphics/freeflycam.h>
#include <runtime/rtdebug.h>
#include <hlsl++.h>

namespace scythe {
	void FreeflyCamera::Reset() {
		m_Position = { 0,0,0 };
		m_PositionalDelta = { 0,0,0 };
		m_Rotation = { 0,0,0 };
		m_RotationalDelta = { 0,0,0 };
		m_Mat = hlslpp::float4x4::identity();
		m_Speed = 1.0f;
	}

	void FreeflyCamera::GetPosition(float3& oPos)
	{
		float32 m_Values[3];
		hlslpp::store(m_Values, -m_Position);
		oPos = { m_Values[0], m_Values[1], m_Values[2] };
	}

	void FreeflyCamera::GetRotation(float3& oRot)
	{
		float32 m_Values[3];
		hlslpp::store(m_Values, m_Rotation);
		oRot = { m_Values[0], m_Values[1], m_Values[2] };
	}

	void FreeflyCamera::Teleport(float3 aPos) {
		m_Position.x = -aPos.x;
		m_Position.y = -aPos.y;
		m_Position.z = -aPos.z;
	}

	void FreeflyCamera::SetRotation(float3 aRot) {
		m_Rotation.x = aRot.x;
		m_Rotation.y = aRot.y;
		m_Rotation.z = aRot.z;
	}

	void FreeflyCamera::Move(float3 aDelta) {
		m_PositionalDelta.x -= aDelta.x;
		m_PositionalDelta.y -= aDelta.y;
		m_PositionalDelta.z -= aDelta.z;
	}

	void FreeflyCamera::Rotate(float3 aDelta) {
		m_RotationalDelta.x += aDelta.x;
		m_RotationalDelta.y += aDelta.y;
		m_RotationalDelta.z += aDelta.z;
	}

	void FreeflyCamera::SetSpeed(float32 aSpeed) {
		m_Speed = aSpeed;
	}
	// hlslpp conversions are via AI - original version was gb_math
	void FreeflyCamera::Update(float32 aDeltaTime, int32 aFOV, float32 aAspectRatio) {
		// value adjustment
		// - position
		m_PositionalDelta *= aDeltaTime;
		SC_SMART_CHECK(m_Speed >= 0.0f, "FreeflyCamera speed is negative");
		m_PositionalDelta *= m_Speed;
		m_Position += m_PositionalDelta;
		m_PositionalDelta = hlslpp::float3(0.0f, 0.0f, 0.0f);
		// - rotation
		m_RotationalDelta *= m_Speed;
		m_Rotation += m_RotationalDelta;
		m_RotationalDelta = hlslpp::float3(0.0f, 0.0f, 0.0f);
		// -
		const hlslpp::frustum m_Frustum = hlslpp::frustum::field_of_view_y(hlslpp::radians(static_cast<float32>(aFOV)), aAspectRatio, 0.05f, 500.0f);
		hlslpp::float4x4 m_Projection = hlslpp::float4x4::perspective(hlslpp::projection(m_Frustum, hlslpp::zclip::minus_one));
		const hlslpp::float3 m_Angles = hlslpp::radians(m_Rotation);
#if HLSLPP_COORDINATES == HLSLPP_COORDINATES_RIGHT_HANDED
		const hlslpp::float3 m_TransformAngles = -m_Angles;
#else
		const hlslpp::float3 m_TransformAngles = m_Angles;
		const hlslpp::float4x4 m_FlipZ = hlslpp::float4x4::scale(1.0f, 1.0f, -1.0f);
#if HLSLPP_LOGICAL_LAYOUT == HLSLPP_LOGICAL_LAYOUT_ROW_MAJOR
		m_Projection = hlslpp::mul(m_FlipZ, m_Projection);
#else
		m_Projection = hlslpp::mul(m_Projection, m_FlipZ);
#endif
#endif
		const hlslpp::float4x4 m_Rotate0 = hlslpp::float4x4::rotation_x(m_TransformAngles.x);
		const hlslpp::float4x4 m_Rotate1 = hlslpp::float4x4::rotation_y(m_TransformAngles.y);
		const hlslpp::float4x4 m_Rotate2 = hlslpp::float4x4::rotation_z(m_TransformAngles.z);
		const hlslpp::float4x4 m_Translate = hlslpp::float4x4::translation(m_Position);
#if HLSLPP_LOGICAL_LAYOUT == HLSLPP_LOGICAL_LAYOUT_ROW_MAJOR
		m_Mat = hlslpp::mul(hlslpp::mul(hlslpp::mul(hlslpp::mul(m_Translate, m_Rotate2), m_Rotate1), m_Rotate0), m_Projection);
#else
		m_Mat = hlslpp::mul(hlslpp::mul(hlslpp::mul(hlslpp::mul(m_Projection, m_Rotate0), m_Rotate1), m_Rotate2), m_Translate);
#endif
	}
	void FreeflyCamera::GetMatrix(matrix4x4& oMatrix)
	{
#if HLSLPP_LOGICAL_LAYOUT == HLSLPP_LOGICAL_LAYOUT_ROW_MAJOR
		hlslpp::store(oMatrix.m, m_Mat);
#else
		hlslpp::store_transposed(oMatrix.m, m_Mat);
#endif
	}
}