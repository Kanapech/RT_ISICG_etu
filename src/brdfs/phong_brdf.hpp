#pragma once
#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"
#include "ray.hpp"
#include "hit_record.hpp"
#include "lights/light_sample.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float & p_s ) : _ks( p_ks ), _s( p_s ) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Ray p_ray, HitRecord hitRecord, LightSample ls ) const { 
			Vec3f wr	   = glm::reflect( ls._direction, hitRecord._normal );
			float cosAlpha = glm::max( glm::dot( p_ray.getDirection(), wr ), 0.f );
			return _ks * glm::pow( cosAlpha, _s );
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks = WHITE;
		float _s  = 1.f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
