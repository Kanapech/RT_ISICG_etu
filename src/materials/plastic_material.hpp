#pragma once
#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float _s )
			: BaseMaterial( p_name ), _lambert_brdf( p_diffuse ), _phong_brdf( p_specular, _s )
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return _lambert_brdf.evaluate() + _phong_brdf.evaluate( p_ray, p_hitRecord, p_lightSample );
		}

		inline const Vec3f & getFlatColor() const override { return _lambert_brdf.getKd() + _phong_brdf.getKs(); }

	  protected:
		LambertBRDF _lambert_brdf;
		PhongBRDF _phong_brdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__
