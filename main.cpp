#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "mds.hpp"

int main()
{

  //testing block
  /*
  unsigned int cols = 10;
  unsigned int rows = 10;
  atom a( 1, 1,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          rows, cols );
  a.save_potential_csv( "potential.csv");
  while( 1 ){
    printf("tick\n");
    sleep( 1000000 );
  }
  */
  //initialization block
  double echarge = 1.60217662*pow( 10, -19 );
  unsigned int cols = 600;
  unsigned int rows = 600;
  unsigned int height = 300;
  atom a( echarge, 2,
          0, 0, 0,
          1, 1, 0,
          0, 0, 0,
          rows, cols, height );
  //a.save_potential_csv( "potential.csv" );
  //create render window
  sf::RenderWindow window( sf::VideoMode( cols, rows), "My window" );
  sf::RenderWindow heatmap( sf::VideoMode( cols, rows), "potential");

  //intialize shapes
  sf::CircleShape circ(a.give_radius());
  circ.setFillColor( sf::Color::Green );
  circ.setPosition( a.pos[0], a.pos[1] );

  //global
  unsigned int delay_ms = 100000;


  //event block

  //one iteration == delay_ms
  while( 1 )
  {
    //check window's events that were triggered since the last iteration
    sf::Event event;
    while( window.pollEvent(event) ){
      //close requested event: we close the window
      if( event.type == sf::Event::Closed ){

        window.close();
      }
    }
    //clear window with black color
    window.clear(sf::Color::Black);

    //draw here
    circ.setPosition(a.pos[0], a.pos[1] );
    window.draw(circ);




    //end current frame
    window.display();

    //status printing
    a.print_pos();

    //apply calculations for next iteration
    a.apply_velocity();
    a.apply_acceleration();
    sleep( delay_ms );
  }
}
