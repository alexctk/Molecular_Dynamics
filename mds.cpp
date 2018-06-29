#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdint.h>

#include "mds.hpp"

atom::atom( double c, unsigned int r,
          unsigned int x, unsigned int y, unsigned int z,
          int dx, int dy, int dz,
          int ax, int ay, int az,
          unsigned int rows, unsigned int cols, unsigned int height )
  :charge(c),
  radius(r),
  cols(cols),
  rows(rows)
{
  pos = new unsigned int[3];
  vel = new int[3];
  accel = new int[3];
  printf("set_pos\n");
  set_pos( x, y, z );
  printf("set_velo\n");
  set_velo( dx, dy, dz );
  printf("set_accel\n");
  set_accel( ax, ay, az );

  potential_field = new double*[rows];
  for( int i = 0; i < cols; i++ ){
      potential_field[i] = new double[cols];
  }
  for( int j = 0; j < rows; j++ ){
    for( int  i = 0; i < cols; i++ ){
      potential_field[j][i] = calc_potential( c, r, pos, i, j, 0 );
    }
  }

}
atom::~atom()
{

}

void atom::print_properties()
{
  printf("charge = %d, radius = %u\n", charge, radius );
}

void atom::print_pos()
{
  printf("position (%u, %u, %u)\n", pos[0], pos[1], pos[2] );
}

void atom::print_velo()
{
  printf("velocity (%d, %d, %d)\n", vel[0], vel[1], vel[2] );
}

void atom::print_accel()
{
  printf("acceleration (%d, %d, %d)\n", accel[0], accel[1], accel[2] );
}

unsigned int atom::give_radius()
{
  return radius;
}

void atom::set_pos( unsigned int x, unsigned int y, unsigned int z )
{
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
}

void atom::set_velo( int dx, int dy, int dz )
{
  vel[0] = dx;
  vel[1] = dy;
  vel[2] = dz;
}

void atom::set_accel( int ax, int ay, int az )
{
  accel[0] = ax;
  accel[1] = ay;
  accel[2] = az;
}

void atom::print_potential()
{
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
      printf("%f ", potential_field[j][i] );
    }
    printf("\n");
  }
}

void atom::save_potential_csv( const char* filename )
{
  std::ofstream p;
  p.open( filename );
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
        p << potential_field[j][i] << ",";
    }
    p << "\n";
  }
  p.close();
}


//assuming one iteration of loop is equal to one second,
//add dx to x
void atom::apply_velocity()
{
  pos[0] += vel[0];
  pos[1] += vel[1];
  pos[2] += vel[2];

  //bounds checking
  while( pos[0] >= cols ){
    pos[0] = pos[0] - cols;
  }
  while( pos[1] >= rows ){
    pos[1] = pos[1] - rows;
  }
  while( pos[2] >= height ){
    pos[2] = pos[2] - height;
  }
}

//assuming one iteration of loop is equal to one second,
//add ax to dx
void atom::apply_acceleration()
{
  vel[0] += accel[0];
  vel[1] += accel[1];
  vel[2] += accel[2];
}

void print_plane( int** matr, unsigned int cols, unsigned int rows )
{
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
      printf("%d ", matr[j][i] );
    }
    printf("\n");
  }
}

double calc_potential( double q, int r, unsigned int* pos,
                      unsigned int x, unsigned int y, unsigned int z )
{
  double e = 8.854187817*pow( 10.00, -12.00 );
  double dist = sqrt( pow( (double)x-(double)pos[0], 2.00 ) + pow( (double)y-(double)pos[1], 2.00 )
                      + pow( (double)z-(double)pos[2], 2.00  ) );
  double u = (-pow(q, 2.00))/(4.00*M_PI*e*dist);
  return u;
}

double calc_elec( double q, int r, unsigned int* pos,
                      unsigned int x, unsigned int y, unsigned int z )
{
  double e = 8.854187817*pow( 10.00, -12.00 );
  double dist = sqrt( pow( (double)x-(double)pos[0], 2.00 ) + pow( (double)y-(double)pos[1], 2.00 )
                      + pow( (double)z-(double)pos[2], 2.00  ) );
  double u = (-pow(q, 2.00))/(4.00*M_PI*e*dist);
  return u;
}

void sleep( unsigned int ms )
{
  clock_t goal = ms + clock();
  while( goal > clock() );
}

//find the max double value in a matrix
//need to think about how to handle inf
double mat_max( double **mat, unsigned int rows, unsigned int cols )
{
  double max = mat[0][0];
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
      if ( mat[j][i] > max ){
        max = mat[j][i];
      }
    }
  }
  return max;
}

//find the min double value in a matrix
//need to think about how to handle inf
double mat_min( double **mat, unsigned int rows, unsigned int cols )
{
  double min = mat[0][0];
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
      if ( mat[j][i] < min ){
        min = mat[j][i];
      }
    }
  }
  return min;
}

//normalize matrix values between 0 and 1
//used for RGB heatmaps
//-inf = 0, +inf = 1
//want non inf min and max
//input takes matrix of values and outputs heatmap in new matrix
unsigned int **normalize( double **mat, unsigned int rows, unsigned int cols )
{
  double min = mat_min( mat );
  double max = mat_max( mat );
  double new_val;
  unsigned int **norm = new unsigned int*[rows];
  for( int i = 0; i < cols; i++ ){
    norm[i] = new unsigned int[cols];
  }

  //normalization
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
      if( mat[j][i] == min ){
        norm[j][i] = 0;
      }
      if( mat[j][i] == max ){
        norm[j][i] = 1;
      }
      else{
        new_val = ( mat[j][i] - min )*1 /(max - min);
        norm[j][i] = new_val;
      }
    }
  }
  //convert the normalization into colors
  //optimize for SFML's color function

  //sf::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha )
  //or sf::Color::black || white || red || green || blue || yellow || magenta
  //|| cyan || transparent

  //color ranges
  //min blue
  //(max + min)/2 green
  //max red
  //want a linear increase/decrease of the colors



  return norm;
}


//take a matrix of normalized greyscale values between 0 and 255_normalize
//convert into array of RGB_COLOR structs which tell the RBG value at each point
RGB_COLOR** color_graph( unsigned int **mat, unsigned int rows, unsigned int cols )
{
  RGB_COLOR* rbg = new RGB_COLOR[rows];
  for( int i = 0; i < cols; i++ ){
    rbg[i] = new RGB_COLOR[cols];
  }

  //calculate the rbg for each point in mat and put it into the new array
  uint8_t r;
  uint8_t g;
  uint8_t b;
  for( int j = 0; j < rows; j++ ){
    for( int i = 0; i < cols; i++ ){
      
    }
  }
}
