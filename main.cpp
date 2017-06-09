#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/StochasticPolygonRenderer>
#include <kvs/Light>
#include "DepthPeelingRenderer.h"
#include "Input.h"
#include "Data.h"


void ParticleBasedRendering( kvs::glut::Screen& screen, local::Input& input, kvs::PolygonObject& polygon )
{
    screen.setSize( input.width, input.height );
    screen.setBackgroundColor( input.background );

    kvs::PolygonObject* object = new kvs::PolygonObject();
    object->shallowCopy( polygon );

    kvs::StochasticPolygonRenderer* renderer = new kvs::StochasticPolygonRenderer();
    renderer->setRepetitionLevel( input.nrepeats );

    screen.registerObject( object, renderer );

    screen.create();
    kvs::Light::SetModelTwoSide( true );
}

void DepthPeelingRendering( kvs::glut::Screen& screen, local::Input& input, kvs::PolygonObject& polygon )
{
    screen.setSize( input.width, input.height );
    screen.setBackgroundColor( input.background );

    kvs::PolygonObject* object = new kvs::PolygonObject();
    object->shallowCopy( polygon );

    local::DepthPeelingRenderer* renderer = new local::DepthPeelingRenderer();
    renderer->setBackgroundColor( input.background );
    renderer->setNumberOfPeels( input.npeels );

    screen.registerObject( object, renderer );

    screen.create();
    kvs::Light::SetModelTwoSide( true );
}

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    local::Input input( argc, argv );
    if ( !input.parse() ) { return false; }

    kvs::PolygonObject* polygon = local::Data( input );

    kvs::glut::Screen screen1( &app );
    screen1.setTitle( "Particle Based Rendering" );
    screen1.setPosition( 0, 0 );
    ParticleBasedRendering( screen1, input, *polygon );

    kvs::glut::Screen screen2( &app );
    screen2.setTitle( "Depth Peeling Rendering" );
    screen2.setPosition( screen1.x() + screen1.width(), screen1.y() );
    DepthPeelingRendering( screen2, input, *polygon );

    if ( input.offscreen )
    {
        screen1.hide();
        screen2.hide();

        screen1.paintEvent();
        screen1.scene()->camera()->snapshot().write( "result_PBR.bmp" );

        screen2.paintEvent();
        screen2.scene()->camera()->snapshot().write( "result_DPB.bmp" );

        app.quit();
        return true;
    }
    else
    {
        screen1.show();
        screen2.show();
        return app.run();
    }
}
