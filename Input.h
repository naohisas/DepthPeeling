#pragma once
#include <kvs/CommandLine>
#include <kvs/RGBColor>


namespace local
{

struct Input
{
private:
    kvs::CommandLine m_commandline;

public:
    int nrepeats;
    int npeels;
    int npolygons;
    float opacity;
    kvs::RGBColor color;
    kvs::RGBColor background;

public:
    Input( int argc, char** argv ):
        nrepeats( 1 ),
        npeels( 1 ),
        npolygons( 10 ),
        opacity( 0.5 ),
        color( kvs::RGBColor::Black() ),
        background( kvs::RGBColor::White() )
    {
        m_commandline = kvs::CommandLine( argc, argv );
        m_commandline.addHelpOption();
        m_commandline.addOption( "nrepeats", "Number of repetitions for IPBR. (defulat: 1)", 1, false );
        m_commandline.addOption( "npeels", "Number of peels for DP. (defulat: 1)", 1, false );
        m_commandline.addOption( "npolygons", "Number of polygons. (defulat: 10)", 1, false );
        m_commandline.addOption( "opacity", "Opacity value for polygon object. (default: 0.5)", 1, false );
        m_commandline.addOption( "color", "Color value for polygon object. (default: 0, 0, 0)", 3, false );
        m_commandline.addOption( "background", "Background color. (default: 255, 255, 255)", 3, false );
    }

    bool parse()
    {
        if ( !m_commandline.parse() ) { return false; }

        if ( m_commandline.hasOption( "nrepeats" ) )
        {
            nrepeats = m_commandline.optionValue<int>( "nrepeats" );
        }

        if ( m_commandline.hasOption( "npeels" ) )
        {
            npeels = m_commandline.optionValue<int>( "npeels" );
        }

        if ( m_commandline.hasOption( "npolygons" ) )
        {
            npolygons = m_commandline.optionValue<int>( "npolygons" );
        }

        if ( m_commandline.hasOption( "opacity" ) )
        {
            opacity = m_commandline.optionValue<float>( "opacity" );
        }

        if ( m_commandline.hasOption( "color" ) )
        {
            const int r = m_commandline.optionValue<int>( "color", 0 );
            const int g = m_commandline.optionValue<int>( "color", 1 );
            const int b = m_commandline.optionValue<int>( "color", 2 );
            color = kvs::RGBColor( r, g, b );
        }

        if ( m_commandline.hasOption( "background" ) )
        {
            const int r = m_commandline.optionValue<int>( "background", 0 );
            const int g = m_commandline.optionValue<int>( "background", 1 );
            const int b = m_commandline.optionValue<int>( "background", 2 );
            background = kvs::RGBColor( r, g, b );
        }

        return true;
    }
};

} // end of namespace local
