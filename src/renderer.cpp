#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/DirectLightingIntegrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::DIRECT_LIGHTING: _integrator = new DirectLightingIntegrator(); break;
		case IntegratorType::RAY_CAST:
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();

		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				/* p_texture.setPixel(
					i,
					j,
					Vec3f( intAsFloat( i ) / intAsFloat( width ), intAsFloat( j ) / intAsFloat( height ), 0 ) );*/


				/*Ray r	   = p_camera->generateRay( ( i + 0.5f ) / width, ( j + 0.5f ) / height );
				Vec3f couleur = _integrator->Li( p_scene, r, 0.f, 100.f );
				
				//p_texture.setPixel( i, j, ( r.getDirection() + 1.f ) * 0.5f );
				p_texture.setPixel( i, j, couleur );*/

				Vec3f moy = VEC3F_ZERO;

				for (int k = 0; k < _nbPixelSamples; k++) {
					float randomx = rand() % _nbPixelSamples;
					float randomy = rand() % _nbPixelSamples;
					Ray r = p_camera->generateRay( ( i + randomx / _nbPixelSamples ) / width, ( j + randomy / _nbPixelSamples ) / height );
					Vec3f couleur = _integrator->Li( p_scene, r, 0.f, 100.f );
					moy += couleur;
				}

				moy /= (float) _nbPixelSamples;
				p_texture.setPixel( i, j, glm::clamp( moy, VEC3F_ZERO, Vec3f( 1.f ) ) );
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
