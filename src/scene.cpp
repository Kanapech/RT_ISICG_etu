#include "scene.hpp"
#include "materials/color_material.hpp"
#include "materials/lambert_material.hpp"
#include "materials/mf_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/mirror_material.hpp"
#include "materials/transparent_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{
		/*// Add objects.
		//_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );

		_addObject( new Sphere( "Sphere2", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plan1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		//_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		//_addMaterial( new ColorMaterial( "Red", RED ) );

		_addMaterial( new LambertMaterial( "Red", RED ) );
		//_addMaterial( new LambertMaterial( "Grey", GREY ) );

		_addMaterial( new PlasticMaterial( "GreyM", GREY*0.7f, GREY*0.3f, 64.f ) );
		_addMaterial( new MFMaterial( "RedReal", RED, RED, 1.f, 0.3, Vec3f( 1.f, 0.85f, 0.57f ) ) );

		// Link objects and materials.
		//_attachMaterialToObject( "Blue", "Sphere1" );
		
		//_attachMaterialToObject( "Grey", "Sphere2" );
		//_attachMaterialToObject( "GreyM", "Sphere2" );
		_attachMaterialToObject( "RedReal", "Sphere2" );
		_attachMaterialToObject( "Red", "Plan1" );
		

		// Add lights
		//_addLight( new PointLight( Vec3f( 1.f, 10.f, 1.f ), WHITE, 100.f) );
		//_addLight( new QuadLight( Vec3f( 1.f, 10.f, 2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 2.f ), WHITE, 40.f ) );

		_addLight( new PointLight( Vec3f( 0.f, 0.f, -2.f ), WHITE, 60.f ) );*/

		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new LambertMaterial( " WhiteMatte ", WHITE ) );
		_addMaterial( new LambertMaterial( " RedMatte ", RED ) );
		_addMaterial( new LambertMaterial( " GreenMatte ", GREEN ) );
		_addMaterial( new LambertMaterial( " BlueMatte ", BLUE ) );
		_addMaterial( new LambertMaterial( " GreyMatte ", GREY ) );
		_addMaterial( new LambertMaterial( " MagentaMatte ", MAGENTA ) );
		
		_addMaterial( new MirrorMaterial( " Mirror1 " ) );
		_addMaterial( new TransparentMaterial( " Transparent1 " ) );

		// ================================================================
		// Add objects .
		// ================================================================
		// Spheres .
		_addObject( new Sphere( " Sphere1 ", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		//_attachMaterialToObject( " WhiteMatte ", " Sphere1 " );
		_attachMaterialToObject( " Mirror1 ", " Sphere1 " );

		_addObject( new Sphere( " Sphere2 ", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		//_attachMaterialToObject( " WhiteMatte ", " Sphere2 " );
		_attachMaterialToObject( " Transparent1 ", " Sphere2 " );

		// Pseudo Cornell box made with infinite planes.
		_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		_attachMaterialToObject( " GreyMatte ", " PlaneGround " );
		_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );

		_attachMaterialToObject( " RedMatte ", " PlaneLeft " );
		_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );

		_attachMaterialToObject( " GreenMatte ", " PlaneCeiling " );
		_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );

		_attachMaterialToObject( " BlueMatte ", " PlaneRight " );
		_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );

		_attachMaterialToObject( " MagentaMatte ", " PlaneFront " );

		// ================================================================
		// Add lights .
		// ================================================================
		_addLight( new PointLight( Vec3f( 0.f, 5.f, 0.f ), WHITE, 100.f ) );
		//_addLight(new QuadLight( Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ), WHITE, 40.f ) );
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, tMax ) )
			{
				hit	 = true;
			}
		}
		return hit;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
