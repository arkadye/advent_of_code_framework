#pragma once

#include <cstddef>

namespace utils
{
	enum class Axis : std::size_t
	{
		X = 0,
		Y = 1,
		Z = 2
	};

	template <typename T>
	class Vector3D
	{
	private:
		T m_x;
		T m_y;
		T m_z;
	public:
		// Constructors
		Vector3D() : Vector3D{ T{0} } {}
		explicit Vector3D(T init) : Vector3D{ init,init,init } {}
		Vector3D(T x, T y, T z) : m_x{ x }, m_y{ y }, m_z{ z }{}
		template <typename X_T, typename Y_T, typename Z_T>
		Vector3D(X_T x, Y_T y, Z_T z)
			: Vector3D{ static_cast<T>(x),static_cast<T>(y),static_cast<T>(z) } {}
		Vector3D(const Vector3D&) = default;
		template <typename OtherT>
		explicit Vector3D(const Vector3D<OtherT>& other)
			: Vector3D{ other.m_x, other.m_y, other.m_z } {}

	};
}