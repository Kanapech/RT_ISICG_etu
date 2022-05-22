#include "WhittedIntegrator.hpp"

namespace RT_ISICG
{

	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax) const
	{
		return trace( p_scene, p_ray, p_tMin, p_tMax, 0, false );
	}
	Vec3f WhittedIntegrator::trace( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax, int bounce, bool insideObject ) const
	{	
		if ( bounce >= _nbBounces ) return VEC3F_ZERO;

		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			// float cosTheta = glm::max( glm::dot( -p_ray.getDirection(), hitRecord._normal ), 0.f );
			if ( hitRecord._object->getMaterial()->isMirror() ) //Materiau mirroir
			{
				//std::cout << "mirror" << std::endl;
				Vec3f dir =  glm::reflect( p_ray.getDirection(), hitRecord._normal );
				Ray	  reflectRay( hitRecord._point, dir );
				reflectRay.offset( hitRecord._normal );
				return trace( p_scene, reflectRay, p_tMin, p_tMax, bounce++, false );
			}
			else if ( hitRecord._object->getMaterial()->isTransparent() ) //Materiau transparent
			{
				float iorIN;
				float iorOUT;

				//Gère l'entrée et la sortie du materiau
				if ( insideObject ) { 
					iorIN = hitRecord._object->getMaterial()->getIOR();
					iorOUT = 1.f; //Indice de réfraction dans le vide
				}
				else
				{
					iorIN  = 1.f;
					iorOUT = hitRecord._object->getMaterial()->getIOR();
				}

				Vec3f dir = glm::reflect( p_ray.getDirection(), hitRecord._normal );
				Ray	  reflectRay( hitRecord._point, dir );
				reflectRay.offset( hitRecord._normal );


				//Voir Wikipédia Fresnel (équations dans l'ordre)
				dir = glm::refract(
					p_ray.getDirection(), hitRecord._normal, iorIN/iorOUT );
				Ray refractRay( hitRecord._point, dir );
				refractRay.offset( -hitRecord._normal );

				float cosThetaI = glm::dot( hitRecord._normal, -p_ray.getDirection() );
				float cosThetaT = glm::dot( -hitRecord._normal, refractRay.getDirection() );

				float RS = ( iorOUT * cosThetaI - iorIN * cosThetaT ) / ( iorOUT * cosThetaI + iorIN * cosThetaT );
				float RP = ( iorOUT * cosThetaT - iorIN * cosThetaI ) / ( iorOUT * cosThetaT + iorIN * cosThetaI );

				float kr = ( RS*RS + RP*RP ) / 2.f;

				if ( kr < 1 )
					return ( 1 - kr ) * trace( p_scene, refractRay, p_tMin, p_tMax, bounce++, !insideObject )
						   + kr * trace( p_scene, refractRay, p_tMin, p_tMax, bounce++, insideObject );
				else
					return kr * trace( p_scene, reflectRay, p_tMin, p_tMax, bounce++, insideObject );
			}
			else //Eclairage direct
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