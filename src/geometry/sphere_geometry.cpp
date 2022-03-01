#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		// Voir TD5 FIG pour détail
		Vec3f origcenter = p_ray.getOrigin() - _center;

		//float alpha = glm::dot(p_ray.getDirection(), p_ray.getDirection() ); // Normalisé donc on peut le remplacer par 1;
		float beta	= 2.f * ( glm::dot( p_ray.getDirection(), origcenter ) );
		float gamma = glm::dot( p_ray.getDirection(), p_ray.getDirection() ) - _radius * _radius;

		float delta = beta * beta - 4 * gamma;

		if (delta < 0) {
			p_t1 = -1.f;
			p_t2 = -1.f;

			return false;
		}
		else{ 
			
			if ( delta == 0 ) {
				p_t1 = -beta / 2.f;
				p_t2 = p_t1;
			}
			else {
				float t1 = -beta + sqrt( delta ) / 2.f;
				float t2 = -beta - sqrt( delta ) / 2.f;

				p_t1 = glm::min( t1, t2 );
				p_t2 = glm::max( t1, t2 );
			}

			return true;
		}

		return false;
	}

} // namespace RT_ISICG
