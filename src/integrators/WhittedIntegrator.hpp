
#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "base_integrator.hpp"

#pragma once
namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax) const;

	  private:
		Vec3f trace( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax, int bounce, bool insideObject ) const;
		Vec3f _directLighting( const LightSample & ls, const Ray & p_ray, HitRecord & hitRecord ) const;
		int	  _nbLightSample = 32;
		int	  _nbBounces = 5;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__
