#include "quad_light.hpp"
#include <utils/random.hpp>

namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f randPos = Vec3f( _position + _u * randomFloat() + _v * randomFloat() );
		float _distance = glm::distance( randPos, p_point );
		Vec3f _direction = glm::normalize( p_point - randPos );
		float _pdf		 = ( ( 1 / _area ) * ( _distance * _distance ) / glm::dot( _normal, -_direction ) );
		Vec3f _radiance	 = ( _color * _power ) / _pdf;

		return LightSample( _direction, _distance, _radiance, _pdf );
	}
} // namespace RT_ISICG