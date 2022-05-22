#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, float & p_u, float & p_v ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		float epsilon = 0.0000001f;
		Vec3f edge1, edge2, h, s, q;
		float a, f;
		edge1 = v1 - v0;
		edge2 = v2 - v0;

		h = glm::cross( d, edge2 );
		a = glm::dot( edge1, h );

		if ( a > -epsilon && a < epsilon ) return false; // Le rayon est parallèle au triangle.

		f = 1.f / a;
		s = o - v0;
		p_u = f * ( glm::dot( s, h ) );

		if ( p_u < 0.f || p_u > 1.f ) return false;

		q = glm::cross( s, edge1 );
		p_v = f * glm::dot( d, q );
		if ( p_v < 0.f || p_u + p_v > 1.f ) return false;

		// On calcule t pour savoir ou le point d'intersection se situe sur la ligne.
		float t = f * glm::dot( edge2, q );
		if ( t > epsilon ) // Intersection avec le rayon
		{
			Vec3f outIntersection = o + d * t;
			p_t					  = glm::distance( outIntersection, o );
			return true;
		}
		else // On a bien une intersection de droite, mais pas de rayon.
			return false;



		return false;
	}

	const Vec3f & TriangleMeshGeometry::getNormalInterpol( const float u, const float v ) const
	{
		return ( 1 - u - v ) * _refMesh->_normals[ _v0 ] + u * _refMesh->_normals[ _v1 ]
			   + v * _refMesh->_normals[ _v2 ];
	}

	void TriangleMeshGeometry::getVertices( Vec3f & a, Vec3f & b, Vec3f & c ) const
	{
		a = _refMesh->_vertices[ _v0 ];
		b = _refMesh->_vertices[ _v1 ];
		c = _refMesh->_vertices[ _v2 ];
	}

} // namespace RT_ISICG
