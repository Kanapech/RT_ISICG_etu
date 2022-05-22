#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		Vec3f invDir = 1.f / p_ray.getDirection();
		Vec3f orig	 = p_ray.getOrigin();
		
		float tmin = ( _min.x - orig.x ) * invDir.x;
		float tmax = ( _max.x - orig.x ) * invDir.x;

		if ( tmin > tmax ) std::swap( tmin, tmax );
		
		float tymin = ( _min.y - orig.y ) * invDir.y;
		float tymax = ( _max.y - orig.y ) * invDir.y;

		if ( tymin > tymax ) std::swap( tymin, tymax );

		if ( ( tmin > tymax ) || ( tymin > tmax ) ) return false;

		if ( tymin > tmin ) tmin = tymin;

		if ( tymax < tmax ) tmax = tymax;

		float tzmin = ( _min.z - orig.z ) * invDir.z;
		float tzmax = ( _max.z - orig.z ) * invDir.z;

		if ( tzmin > tzmax ) std::swap( tzmin, tzmax );

		if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return false;

		if ( tzmin > tmin ) tmin = tzmin;

		if ( tzmax < tmax ) tmax = tzmax;

		return true;
	}
} // namespace RT_ISICG
