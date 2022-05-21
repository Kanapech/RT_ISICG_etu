#include "DirectLightingIntegrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			//float cosTheta = glm::max( glm::dot( -p_ray.getDirection(), hitRecord._normal ), 0.f );
			
			Vec3f finalLight = VEC3F_ZERO;
			for ( BaseLight * light : p_scene.getLights() )
			{
				LightSample ls = light->sample( hitRecord._point );
				Ray			shadow = Ray( hitRecord._point, -ls._direction );
				shadow.offset( hitRecord._normal );
				if ( !p_scene.intersect(shadow, p_tMin, p_tMax, hitRecord)) {
					finalLight += hitRecord._object->getMaterial()->getFlatColor() * _directLighting( ls, hitRecord );
				}
			}

			return finalLight;			
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f DirectLightingIntegrator::_directLighting( const LightSample & ls, HitRecord & hitRecord ) const
	{
		float cosTheta = glm::max( glm::dot( -ls._direction, hitRecord._normal ), 0.f );
		return hitRecord._object->getMaterial()->getFlatColor() * ls._radiance * cosTheta;
	}
} // namespace RT_ISICG