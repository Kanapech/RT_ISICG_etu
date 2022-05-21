#ifndef __RT_ISICG_MIRROR__
#define __RT_ISICG_MIRROR__

#include "base_material.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
	#pragma once
	class MirrorMaterial : public BaseMaterial
	{
	  public:
		MirrorMaterial( const std::string & p_name) : BaseMaterial( p_name )
		{
		}

		virtual ~MirrorMaterial() = default;

		Vec3f shade( const Ray & p_ray, const HitRecord & p_hit, const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		inline const Vec3f & getFlatColor() const override { return BLACK; }

		bool isMirror() override { return true; }

	  protected:
		Vec3f _color = Vec3f( 1.f, 1.f, 1.f );
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MIRROR__
