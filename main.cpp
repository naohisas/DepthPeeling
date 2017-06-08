#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/PolygonObject>
#include <kvs/StochasticPolygonRenderer>
#include <kvs/MersenneTwister>
#include <kvs/Light>
#include "DepthPeelingRenderer.h"

kvs::PolygonObject* Polygons( const size_t npolygons, const kvs::RGBColor color, const kvs::UInt8 opacity )
{
    const size_t nvertices = npolygons * 3;
    kvs::ValueArray<kvs::Real32> coords( nvertices * 3 );
    kvs::ValueArray<kvs::Real32> normals( npolygons * 3 );
    kvs::Real32* pcoords = coords.data();
    kvs::Real32* pnormals = normals.data();

    kvs::MersenneTwister rand;
    for ( size_t i = 0; i < npolygons; i++ )
    {
        kvs::Vec3 p0( rand(), rand(), rand() );
        kvs::Vec3 p1( rand(), rand(), rand() );
        kvs::Vec3 p2( rand(), rand(), rand() );
        kvs::Vec3 n = ( p1 - p0 ).cross( p2 - p0 );

        *(pcoords++) = p0.x();
        *(pcoords++) = p0.y();
        *(pcoords++) = p0.z();
        *(pcoords++) = p1.x();
        *(pcoords++) = p1.y();
        *(pcoords++) = p1.z();
        *(pcoords++) = p2.x();
        *(pcoords++) = p2.y();
        *(pcoords++) = p2.z();
        *(pnormals++) = n.x();
        *(pnormals++) = n.y();
        *(pnormals++) = n.z();
    }

    kvs::PolygonObject* object = new kvs::PolygonObject();
    object->setPolygonTypeToTriangle();
    object->setNormalTypeToPolygon();
    object->setCoords( coords );
    object->setNormals( normals );
    object->setColor( color );
    object->setOpacity( opacity );
    return object;
}

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    const size_t npolygons = 10;
    const kvs::RGBColor color( 50, 200, 255 );
    const kvs::UInt8 opacity = 200;
    const kvs::RGBColor background_color = kvs::RGBColor::White();

    kvs::PolygonObject* polygon = Polygons( npolygons, color, opacity );

    kvs::glut::Screen screen1( &app );
    screen1.setTitle( "Stochastic Rendering" );
    screen1.setPosition( 0, 0 );
    screen1.setBackgroundColor( background_color );
    {
        kvs::PolygonObject* object = new kvs::PolygonObject();
        object->shallowCopy( *polygon );

        kvs::StochasticPolygonRenderer* renderer = new kvs::StochasticPolygonRenderer();
        renderer->setRepetitionLevel( 100 );

        screen1.registerObject( object, renderer );
    }
    screen1.show();
    kvs::Light::SetModelTwoSide( true );

    kvs::glut::Screen screen2( &app );
    screen2.setTitle( "Depth Peeling Rendering" );
    screen2.setPosition( screen1.x() + screen1.width(), screen1.y() );
    screen2.setBackgroundColor( background_color );
    {
        kvs::PolygonObject* object = new kvs::PolygonObject();
        object->shallowCopy( *polygon );

        local::DepthPeelingRenderer* renderer = new local::DepthPeelingRenderer();
        renderer->setBackgroundColor( background_color );
        renderer->setNumberOfPeels( 100 );

        screen2.registerObject( object, renderer );
    }
    screen2.show();
    kvs::Light::SetModelTwoSide( true );

    return app.run();
}
