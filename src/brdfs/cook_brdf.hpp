#pragma once__RT_ISICG_BRDF_COOK__
#ifndef __RT_ISICG_BRDF_COOK__
#define __RT_ISICG_BRDF_COOK__

#include "defines.hpp"
#include "ray.hpp"
#include "hit_record.hpp"
#include "lights/light_sample.hpp"

namespace RT_ISICG
{
	class CookBRDF
	{
	  public:
		CookBRDF( const Vec3f & p_kd ) : _kd( p_kd ) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( Vec3f p_wo, Vec3f p_wi, Vec3f _normal, float rug, Vec3f fo ) const { 
			Vec3f h = ( p_wo + p_wi ) / glm::length( p_wo + p_wi );
			float alpha = glm::pow( rug, 2 );
			float alpha2 = glm::pow( alpha, 2 );
			float D = alpha2 / ( PIf * glm::pow( ( ( glm::pow( glm::dot( _normal, h ), 2 ) ) * ( alpha2 - 1 ) + 1 ), 2 ) );

			
			float G1 = funcG( glm::dot( _normal, p_wo ), rug );
			float G2 = funcG( glm::dot( _normal, p_wi ), rug );
			float G	 = G1 * G2;

			Vec3f F = fo + ( 1.f - fo ) * glm::pow( ( 1 - glm::dot( h, p_wo ) ), 5.f );

			return ( D * F * G ) / ( 4.f * glm::dot( p_wo, _normal ) * glm::dot( p_wi, _normal ) );
		}


		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;

		float funcG( float x, float rug ) const
		{
			float k = glm::pow( rug + 1, 2 ) / 8;

			return x / ( x * ( 1 - k ) + k );
		}
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_COOK__
