#pragma once
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Light>
#include <kvs/PolygonObject>
#include <kvs/StochasticPolygonRenderer>
#include "DepthPeelingRenderer.h"
#include "Input.h"
#include "Data.h"


namespace local
{

class Screens
{
private:
    kvs::glut::Screen m_screen_pbr;
    kvs::glut::Screen m_screen_dpr;

public:
    Screens( kvs::glut::Application& app, local::Input& input ):
        m_screen_pbr( &app ),
        m_screen_dpr( &app )
    {
        kvs::PolygonObject polygon = local::Data( input );

        m_screen_pbr.setTitle( "Particle Based Rendering" );
        m_screen_pbr.setPosition( 0, 0 );
        this->setup_pbr( input, polygon );

        m_screen_dpr.setTitle( "Depth Peeling Rendering" );
        m_screen_dpr.setPosition( m_screen_pbr.x() + m_screen_pbr.width(), m_screen_pbr.y() );
        this->setup_dpr( input, polygon );
    }

    void show()
    {
        m_screen_pbr.show();
        m_screen_dpr.show();
    }

    void hide()
    {
        m_screen_pbr.hide();
        m_screen_dpr.hide();
    }

    void capture()
    {
        m_screen_pbr.paintEvent();
        m_screen_pbr.scene()->camera()->snapshot().write( "result_PBR.bmp" );

        m_screen_dpr.paintEvent();
        m_screen_dpr.scene()->camera()->snapshot().write( "result_DPB.bmp" );
    }

private:
    void setup_pbr( local::Input& input, kvs::PolygonObject& polygon )
    {
        typedef kvs::PolygonObject Object;
        typedef kvs::StochasticPolygonRenderer Renderer;

        m_screen_pbr.setSize( input.width, input.height );
        m_screen_pbr.setBackgroundColor( input.background );

        Object* object = new Object();
        object->shallowCopy( polygon );

        Renderer* renderer = new Renderer();
        renderer->setRepetitionLevel( input.nrepeats );

        m_screen_pbr.registerObject( object, renderer );
        m_screen_pbr.create();
        kvs::Light::SetModelTwoSide( true );
    }

    void setup_dpr( local::Input& input, kvs::PolygonObject& polygon )
    {
        typedef kvs::PolygonObject Object;
        typedef local::DepthPeelingRenderer Renderer;

        m_screen_dpr.setSize( input.width, input.height );
        m_screen_dpr.setBackgroundColor( input.background );

        Object* object = new Object();
        object->shallowCopy( polygon );

        Renderer* renderer = new Renderer();
        renderer->setBackgroundColor( input.background );
        renderer->setNumberOfPeels( input.npeels );

        m_screen_dpr.registerObject( object, renderer );
        m_screen_dpr.create();
        kvs::Light::SetModelTwoSide( true );
    }
};

} // end of namespace local
