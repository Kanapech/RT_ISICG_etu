#include "WhittedIntegrator.hpp"

namespace RT_ISICG
{

	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax) const
	{
		return trace( p_scene, p_ray, p_tMin, p_tMax, 0 );
	}
	Vec3f WhittedIntegrator::trace( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax, int bounce ) const
	{	
		if ( bounce >= _nbBounces ) return VEC3F_ZERO;

		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			// float cosTheta = glm::max( glm::dot( -p_ray.getDirection(), hitRecord._normal ), 0.f );
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				//std::cout << "mirror" << std::endl;
				Vec3f dir =  glm::reflect( p_ray.getDirection(), hitRecord._normal );
				Ray	  reflectRay( hitRecord._point, dir );
				reflectRay.offset( hitRecord._normal );
				return trace( p_scene, reflectRay, p_tMin, p_tMax, bounce++ );
			}
			else
			{
				Vec3f finalLight = VEC3F_ZERO;
				for ( BaseLight * light : p_scene.getLights() )
				{
					if ( light->isSurface() )
					{
						Vec3f finalShadow = VEC3F_ZERO;
						for ( int i = 0; i < _nbLightSample; i++ )
						{
							LightSample ls	   = light->sample( hitRecord._point );
							Ray			shadow = Ray( hitRecord._point, ls._direction );
							shadow.offset( hitRecord._normal );
							if ( !p_scene.intersectAny( shadow, p_tMin, ls._distance ) )
							{
								finalShadow += _directLighting( ls, p_ray, hitRecord );
							}
						}

						finalShadow /= _nbLightSample;
						finalLight += finalShadow;
						// std::cout << finalLight.x << finalLight.y << finalLight.z << std::endl;
					}
					else
					{
						LightSample ls	   = light->sample( hitRecord._point );
						Ray			shadow = Ray( hitRecord._point, ls._direction );
						shadow.offset( hitRecord._normal );
						if ( !p_scene.intersectAny( shadow, p_tMin, ls._distance ) )
						{
							finalLight += _directLighting( ls, p_ray, hitRecord );
						}
					}
				}

				return finalLight;
			}
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f WhittedIntegrator::_directLighting( const LightSample & ls,
													 const Ray &		 p_ray,
													 HitRecord &		 hitRecord ) const
	{
		float cosTheta = glm::max( glm::dot( ls._direction, hitRecord._normal ), 0.f );
		return hitRecord._object->getMaterial()->shade( p_ray, hitRecord, ls ) * ls._radiance * cosTheta;
	}
} // namespace RT_ISICG