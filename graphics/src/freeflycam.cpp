// BismIllahIRRahmaanIRRaheem
/* freefly camera implementation */

#include <graphics/freeflycam.h>
#include <runtime/rtdebug.h>
#include <gb.h>
#include <gb_math.h>

namespace scythe {
	void FreeflyCamera::Reset() {
		SC_ZMEM(this, sizeof(*this));
		gb_mat4_identity((gbMat4*)&m_Mat);
		m_Speed = 1.0f;
	}

	void FreeflyCamera::GetPosition(float3& oPos)
	{
		oPos = m_Position;
	}

	void FreeflyCamera::GetRotation(float3& oRot)
	{
		oRot = m_Rotation;
	}

	void FreeflyCamera::Teleport(float3 aPos) {
		m_Position = aPos;
	}

	void FreeflyCamera::SetRotation(float3 aRot) {
		m_Rotation = aRot;
	}

	void FreeflyCamera::Move(float3 aDelta) {
		m_PositionalDelta.x += aDelta.x;
		m_PositionalDelta.y += aDelta.y;
		m_PositionalDelta.z += aDelta.z;
	}

	void FreeflyCamera::Rotate(float3 aDelta) {
		m_RotationalDelta.x += aDelta.x;
		m_RotationalDelta.y += aDelta.y;
		m_RotationalDelta.z += aDelta.z;
	}

	void FreeflyCamera::SetSpeed(float32 aSpeed) {
		m_Speed = aSpeed;
	}

	void FreeflyCamera::Update(float32 aDeltaTime, int32 aFOV, float32 aAspectRatio) {
		// value adjustment
		// - position
		m_PositionalDelta.x *= aDeltaTime;
		m_PositionalDelta.y *= aDeltaTime;
		m_PositionalDelta.z *= aDeltaTime;
		SC_SMART_CHECK(m_Speed >= 0.0f, "FreeflyCamera speed is negative");
		m_PositionalDelta.x *= m_Speed;
		m_PositionalDelta.y *= m_Speed;
		m_PositionalDelta.z *= m_Speed;
		m_Position.x += m_PositionalDelta.x;
		m_Position.y += m_PositionalDelta.y;
		m_Position.z += m_PositionalDelta.z;
		SC_ZMEM(&m_PositionalDelta, sizeof(m_PositionalDelta));
		// - rotation
		m_RotationalDelta.x *= m_Speed;
		m_RotationalDelta.y *= m_Speed;
		m_RotationalDelta.z *= m_Speed;
		m_Rotation.x += m_RotationalDelta.x;
		m_Rotation.y += m_RotationalDelta.y;
		m_Rotation.z += m_RotationalDelta.z;
		SC_ZMEM(&m_RotationalDelta.x, sizeof(m_RotationalDelta));
		// -
		gbMat4 m_Projection;
		gbMat4 m_Rotate0;
		gbMat4 m_Rotate1;
		gbMat4 m_Rotate01;
		gbMat4 m_Rotate2;
		gbMat4 m_Rotate;
		gbMat4 m_Translate;
		// projection
		gb_mat4_perspective(&m_Projection, gb_to_radians((float32)aFOV), aAspectRatio, 0.1f, 1000.0f);
		// rotation
		gb_mat4_rotate(&m_Rotate0, { 1, 0, 0 }, gb_to_radians(m_Rotation.x));
		gb_mat4_rotate(&m_Rotate1, { 0, 1, 0 }, gb_to_radians(m_Rotation.y));
		gb_mat4_rotate(&m_Rotate2, { 0, 0, 1 }, gb_to_radians(m_Rotation.z));
		gb_mat4_mul(&m_Rotate01, &m_Rotate0, &m_Rotate1);
		gb_mat4_mul(&m_Rotate, &m_Rotate01, &m_Rotate2);
		// translation
		gb_mat4_translate(&m_Translate, { m_Position.x, m_Position.y, m_Position.z });
		// multiplication
		gb_mat4_mul((gbMat4*)&m_Mat, &m_Projection, &m_Rotate);
		gb_mat4_mul((gbMat4*)&m_Mat, (gbMat4*)&m_Mat, &m_Translate);
	}
	void FreeflyCamera::GetMatrix(matrix4x4& oMatrix)
	{
		oMatrix = m_Mat;
	}
}