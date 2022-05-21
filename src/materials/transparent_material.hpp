#ifndef __RT_ISICG_TRANSPARENT__
#define __RT_ISICG_TRANSPARENT__

#include "base_material.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
#pragma once
	class TransparentMaterial : public BaseMaterial
	{
	  public:
		TransparentMaterial( const std::string & p_name ) : BaseMaterial( p_name ) {}

		virtual ~TransparentMaterial() = default;

		Vec3f shade( const Ray & p_ray, const HitRecord & p_hit, const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		inline const Vec3f & getFlatColor() const override { return BLACK; }

		bool isTransparent() override { return true; }

		virtual float getIOR() override { return _ior; }

	  protected:
		Vec3f _color = Vec3f( 1.f, 1.f, 1.f );
		float _ior	 = 1.3f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_TRANSPARENT__
