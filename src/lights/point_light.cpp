#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point ) const
	{
		float _distance	 = glm::distance( _position, p_point );
		Vec3f _radiance	 = ( _color * _power ) / ( _distance * _distance );
		Vec3f _direction = glm::normalize( p_point - _position );

		return LightSample( _direction, _distance, _radiance, 1.f );
	}
} // namespace RT_ISICG