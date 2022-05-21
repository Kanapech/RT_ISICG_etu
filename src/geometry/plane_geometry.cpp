#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t1 ) const
	{

		/* float d = glm::dot( _normal, p_ray.getDirection() );
		if ( d >= 0.f ) {
			return false;
		}

		Vec3f po = _point - p_ray.getOrigin();
		p_t1	 = glm::dot( _normal, po ) / d;
		return true;*/

		p_t1 = glm::dot( _point - p_ray.getOrigin(), _normal ) / glm::dot( p_ray.getDirection(), _normal );
		if ( p_t1 > 0 ) { return true; }
		return false;
	}

} // namespace RT_ISICG
