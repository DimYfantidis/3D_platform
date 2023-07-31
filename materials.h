#pragma once

#include <array>
#include "typedefs.h"


namespace materials
{
	class material
	{
	public:
		float shininess;
		vector4f ambient;
		vector4f specular;
		vector4f diffuse;

		constexpr material(float shine, 
			const std::array<float, 4>& a,
			const std::array<float, 4>& s,
			const std::array<float, 4>& d)
			:
			shininess(shine),
			ambient{ a[0], a[1], a[2], a[3] },
			specular{ s[0], s[1], s[2], s[3] },
			diffuse{ d[0], d[1], d[2], d[3] }
		{}
	};

	constexpr material gold(0.6f,
		{ 0.02150f, 0.174500f, 0.02150f, 1.0f },
		{ 0.07568f, 0.614240f, 0.07568f, 1.0f },
		{ 0.63300f, 0.727811f, 0.63300f, 1.0f }
	);
	constexpr material jade(0.1f,
		{ 0.135000f,	0.222500f,	0.157500f, 1.0f },
		{ 0.540000f,	0.890000f,	0.630000f, 1.0f },
		{ 0.316228f,	0.316228f,	0.316228f, 1.0f }
	);
	constexpr material obsidian(0.3f,
		{ 0.05375f,	0.050000f,	0.066250f, 1.0f },
		{ 0.182750f,	0.170000f,	0.225250f, 1.0f },
		{ 0.332741f,	0.328634f,	0.346435f, 1.0f }
	);
	constexpr material pearl(0.088f,
		{ 0.250000f, 0.207250f,	0.207250f, 1.0f },
		{ 1.000000f, 0.829000f,	0.829000f, 1.0f },
		{ 0.296648f, 0.296648f,	0.296648f, 1.0f }
	);

	const vector4f ground_amb = { 0.05f, 0.2f, 0.05f, 1.0f };
	const vector4f ground_spec = { 0.0f, 0.0f, 0.0f, 1.0f };
	const vector4f ground_diff = { 0.3f, 1.0f, 0.3f, 1.0f };

	const vector4f cuboid_amb = { 0.3f, 0.3f, 0.3f, 1.0f };
	const vector4f cuboid_spec = { 0.0f, 0.0f, 0.0f, 1.0f };
	const vector4f cuboid_diff = { 0.3f, 0.3f, 0.3f, 1.0f };

	const vector4f sphere_amb = { 0.3f, 0.3f, 0.3f, 1.0f };
	const vector4f sphere_spec = { 0.1f, 0.1f, 0.1f, 1.0f };
	const vector4f sphere_diff = { 0.3f, 0.3f, 0.3f, 1.0f };

	const vector4f ball_amb = { 0.3f, 0.3f, 0.3f, 1.0f };
	const vector4f ball_spec = { 0.0f, 0.0f, 0.0f, 1.0f };
	const vector4f ball_diff = { 0.3f, 0.3f, 0.3f, 1.0f };

	const vector4f lamp_emsn = { 0.2f, 0.2f, 0.0f, 1.0f };
	const vector4f lamp_diff = { 1.0f, 1.0f, 0.0f, 1.0f };

	const vector4f light_spec = { 0.5f, 0.5f, 0.5f, 1.0f };
	const vector4f light_diff = { 0.5f, 0.5f, 0.5f, 1.0f };
}