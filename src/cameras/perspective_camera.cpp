#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		/// TODO ! _u ? _v ? _w ?

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float h	   = glm::tan( glm::radians( _fovy ) / 2.f ) * _focalDistance;
		_viewportU = 2 * ( h * _aspectRatio ) * _u;
		_viewportV = 2 * h * _v;
		_viewportTopLeftCorner = - _w * _focalDistance - _viewportU / 2.f + _viewportV / 2.f;
	}

} // namespace RT_ISICG
