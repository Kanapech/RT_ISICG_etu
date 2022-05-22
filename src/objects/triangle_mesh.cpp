#include "triangle_mesh.hpp"
//#define BVH
#define AABB

namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
	#ifdef AABB
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;
	#endif

	#ifdef BVH
		bool inter = _bvh.intersect( p_ray, p_tMin, p_tMax, p_hitRecord );
		if ( inter ) p_hitRecord._object = this;
		return inter;
	#endif

		float  tClosest = p_tMax;			 // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		float  u		= 0.f;
		float  v		= 0.f;
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t, tempU, tempV;
			if ( _triangles[ i ].intersect( p_ray, t, tempU, tempV ) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					tClosest = t;
					hitTri	 = i;
					u		 = tempU;
					v		 = tempV;
					
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			p_hitRecord._point	= p_ray.pointAtT( tClosest );
			p_hitRecord._normal = _triangles[ hitTri ].getFaceNormal();
			//p_hitRecord._normal = _triangles[ hitTri ].getNormalInterpol( u, v );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		#ifdef AABB
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;
		#endif

		#ifdef BVH
		return _bvh.intersectAny( p_ray, p_tMin, p_tMax );
		#endif
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t, u, v;
			if ( _triangles[ i ].intersect( p_ray, t, u, v ) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
} // namespace RT_ISICG
