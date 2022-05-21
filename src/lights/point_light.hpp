#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
	  public:

		PointLight( const std::string & p_name, const Vec3f & p_position, const Vec3f & p_color, const float p_power = 1.f )
			: _position( p_position ), BaseLight( p_name, p_color, p_power )
		{
		}

		virtual LightSample sample( const Vec3f & p_point ) const;
		Vec3f _position = VEC3F_ZERO;

	};

} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__