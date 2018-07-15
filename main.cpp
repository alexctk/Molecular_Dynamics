#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

#include "mds.hpp"

// add the values from the atom's potential grid to the global grid
void update_global_grid( double** atom_grid, double** global_grid, unsigned int cols, unsigned int rows )
{
  for( int i = 0; i < cols; i++){
    for( int j = 0; j < rows; j++){
      global_grid[j][i] += atom_grid[j][i];
    }
  }
  return;
}

// do a rough approximation using
// (f(x+h)-f(x-h))/2h
// (f(x_atom+x_grid) - f(x_atom-x_grid))/2(x_grid)
// where f is the potential function
// 2D gradient for now
double* calc_gradient( unsigned int* atom_pos,
		      double** atom_grid, double** potential_grid,
		      unsigned int cols, unsigned int rows )
{ 
  double* grad = new double[2];
  unsigned int atom_x = atom_pos[0];
  unsigned int atom_y = atom_pos[1];
  //cout << "if statements in calc_grad" << endl;
  if( atom_x+1 < cols && (int)atom_x-1 >= 0){
    //cout << atom_x+1 << "," << (int)atom_x-1 << endl;
    grad[0] = (potential_grid[atom_y][atom_x+1] - potential_grid[atom_y][atom_x-1])/2*1;
  }
  if( atom_y+1 < rows && (int)atom_y-1 >= 0){
    grad[1] = (potential_grid[atom_y+1][atom_x] - potential_grid[atom_y-1][atom_x])/2*1;
  }

  return grad;

}



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

  //adjustable charge
  double echarge = pow(99, -5);
  unsigned int cols = 500;
  unsigned int rows = 500;
  unsigned int height = 300;
  atom a( echarge, 2,
          0, 0, 0,
          1, 1, 0,
          0, 0, 0,
          rows, cols, height );
  atom b( echarge, 2,
	  cols-1, rows-1, 0,
	  -1, -1, 0,
	  0, 0, 0,
	  rows, cols, height );
  
  //initialize global potential grid
  double** potential = new double*[rows];
  for( int i = 0; i < cols; i++ ){
      potential[i] = new double[cols];
  }
  for( int j = 0; j < rows; j++ ){
    for( int  i = 0; i < cols; i++ ){
      potential[j][i] = 0;
    }
  }

  //apply atoms potentials to the global grid
  update_global_grid(a.get_potential_field(), potential, cols, rows);
  update_global_grid(b.get_potential_field(), potential, cols, rows);
  
  //create render window
  sf::RenderWindow window( sf::VideoMode( cols, rows), "My window" );
  sf::RenderWindow heatmap( sf::VideoMode( cols, rows), "potential");

  //intialize shapes
  sf::CircleShape circ(a.give_radius());
  circ.setFillColor( sf::Color::Green );
  circ.setPosition( a.pos[0], a.pos[1] );
  
  sf::CircleShape circ2(b.give_radius());
  circ2.setFillColor( sf::Color::Blue );
  circ2.setPosition( b.pos[0], b.pos[1] );
  

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
    circ2.setPosition(b.pos[0], b.pos[1] );
    window.draw(circ);
    window.draw(circ2);




    //end current frame
    window.display();

    //status printing
    //cout << "print pos" << endl;
    //a.print_pos();
    //cout << "calc potential" << endl;
    //a.calc_potential_grid();
    // a.print_potential();

    //apply calculations for next iteration
    //cout << "before calc grad" << endl;
    double* force = calc_gradient( a.pos,
			      a.get_potential_field(), potential,
			      cols, rows );
    double* forceb = calc_gradient( b.pos,
				    b.get_potential_field(), potential,
				    cols, rows );
    //cout << "after calc grad" << endl;
    cout << force[0] << endl;
    a.set_accel( force[0], force[1], 0 );
    a.apply_velocity();
    a.apply_acceleration();

    b.set_accel( force[0], force[1], 0 );
    b.apply_velocity();
    b.apply_acceleration();

    cout << "a's acceleration: ";
    a.print_accel();

    cout << "b's acceleration: ";
    b.print_accel();
    
    sleep( delay_ms );
  }
}
