#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:

		QuadLight( const Vec3f & p_position, const Vec3f & p_u, const Vec3f & p_v, const Vec3f & p_color, const float p_power = 1.f )
			: _position( p_position ), _u( p_u ), _v( p_v ), _normal( glm::cross( p_u, p_v ) ), _area( glm::length( p_u ) * glm::length( p_v ) ),
			  BaseLight( p_color, p_power )
		{
		}

		virtual LightSample sample( const Vec3f & p_point ) const;

		private:
			Vec3f				_position = VEC3F_ZERO;
			Vec3f				_u = VEC3F_ZERO;
			Vec3f				_v = VEC3F_ZERO;
			Vec3f				_normal	= VEC3F_ZERO;
			float				_area = 0;


	};

} // namespace RT_ISICG

#endif // __RT_ISICG_QUAD_LIGHT__