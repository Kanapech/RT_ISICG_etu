#pragma once
#pragma once
#ifndef __RT_ISICG_MF_MATERIAL__
#define __RT_ISICG_MF_MATERIAL__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/cook_brdf.hpp"

namespace RT_ISICG
{
	class MFMaterial : public BaseMaterial
	{
	  public:
		MFMaterial( const std::string & p_name,
					const Vec3f &		p_diffuse,
					const Vec3f &		p_specular,
					const float &		p_metalness,
					const float &		p_rug,
					const Vec3f			p_fo )
			: BaseMaterial( p_name ), _lambert_brdf( p_diffuse ), _cook_brdf( p_specular ), _metalness( p_metalness ),
			  _rug( p_rug ), _fo( p_fo )
		{
		}

		virtual ~MFMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return ( 1.f - _metalness ) * _lambert_brdf.evaluate()
				   + _metalness
						 * _cook_brdf.evaluate( -p_ray.getDirection(),
												p_lightSample._direction,
												p_hitRecord._normal,
												_rug,
												_fo );
		}

		inline const Vec3f & getFlatColor() const override { return _lambert_brdf.getKd() + _cook_brdf.getKd(); }

	  protected:
		LambertBRDF _lambert_brdf;
		CookBRDF	_cook_brdf;
		float		_metalness = 1.f;
		float		_rug = 1.f;
		Vec3f		_fo;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MF_MATERIAL__
