#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"
#include <algorithm>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();
		_buildRec( _root, 0, _triangles->size(), 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		if ( _root != nullptr ) return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord );
		return false;
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		if ( _root != nullptr ) return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
		return false;
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{

		

		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;

		for (int i = p_firstTriangleId; i < p_lastTriangleId; i++) {
			Vec3f A, B, C;
			( *_triangles )[ i ].getVertices( A, B, C );
			p_node->_aabb.extend( A );
			p_node->_aabb.extend( B );
			p_node->_aabb.extend( C );
		}

		if ( p_depth >= _maxDepth || ( ( p_lastTriangleId - p_firstTriangleId ) >= _maxTrianglesPerLeaf ) ) { return; }

		int	  axePartition = p_node->_aabb.largestAxis();
		Vec3f centroid	   = p_node->_aabb.centroid();
		float milieu;
		switch ( axePartition )
		{
		case 0: milieu = centroid.x; break;
		case 1: milieu = centroid.y; break;
		case 2: milieu = centroid.z; break;
		}

		auto it = std::partition( _triangles->begin() + p_firstTriangleId,
						_triangles->begin() + p_lastTriangleId,
						[ = ]( TriangleMeshGeometry & tmg )
						{
							Vec3f A, B, C;
							tmg.getVertices( A, B, C );
							switch ( axePartition )
							{
							case 0: return ( A.x < milieu && B.x < milieu && C.x < milieu ); break;
							case 1: return ( A.y < milieu && B.y < milieu && C.y < milieu ); break;
							case 2: return ( A.z < milieu && B.z < milieu && C.z < milieu ); break;
							}
						} );

		int idPartition = ( it - _triangles->begin() );
		p_node->_left	= new BVHNode();
		//p_node->_left->_firstTriangleId = p_firstTriangleId;
		//p_node->_left->_lastTriangleId	= idPartition;
		_buildRec( p_node->_left, p_firstTriangleId, idPartition, p_depth + 1 );
		p_node->_right	= new BVHNode();
		//p_node->_right->_lastTriangleId = p_lastTriangleId;
		_buildRec( p_node->_right, idPartition, p_lastTriangleId, p_depth + 1 );
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) )// On reprend le fonctionnement du MeshTriangle
			return false; 

		if (p_node->isLeaf()) {
			float  tClosest = p_tMax;			  // Hit distance.
			size_t hitTri	= _triangles->size(); // Hit triangle id.
			float  u		= 0.f;
			float  v		= 0.f;
			for ( size_t i = 0; i < _triangles->size(); i++ )
			{
				float t, tempU, tempV;
				if ( ( *_triangles )[ i ].intersect( p_ray, t, tempU, tempV ) )
				{
					if ( t >= p_tMin && t <= tClosest )
					{
						tClosest = t;
						hitTri	 = i;
						u		 = tempU;
						v		 = tempV;
					}
				}
			}
			if ( hitTri != p_node->_lastTriangleId ) // Intersection found.
			{
				p_hitRecord._point = p_ray.pointAtT( tClosest );
				// p_hitRecord._normal = _triangles[ hitTri ].getFaceNormal();
				p_hitRecord._normal = ( *_triangles )[ hitTri ].getNormalInterpol( u, v );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = tClosest;

				return true;
			}
		}
		else
		{
			HitRecord h1, h2;
			h1._distance		 = INFINITY;
			h2._distance		 = INFINITY;
			bool	  leftInter	 = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, h1 );
			bool	  rightInter = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, h2 );

			if (leftInter || rightInter) { 
				p_hitRecord = h1._distance < h2._distance ? h1 : h2;
				return true;
			}
		}
			
		
		return false;
	}

	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( !p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }

		if (p_node->isLeaf()) {
			for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
			{
				float t, u, v;
				if ( ( * _triangles )[ i ].intersect( p_ray, t, u, v ) )
					{
						if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
					}
			}
			return false;
		}
		else
		{
			bool leftInter = _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
			bool rightInter = _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );

			if ( leftInter || rightInter ) return true;
		}
		
		return false;
	}
} // namespace RT_ISICG
