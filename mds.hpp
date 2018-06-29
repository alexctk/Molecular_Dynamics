#include <stdint.h>

class atom
{
private:
  int charge;
  unsigned int radius;
  int *vel;
  int *accel;
  double** potential_field;
  double** e_field;
  unsigned int cols;
  unsigned int rows;
  unsigned int height;
public:
  unsigned int *pos;
  atom(double c, unsigned int r,
            unsigned int x, unsigned int y, unsigned int z,
            int dx, int dy, int dz,
            int ax, int ay, int az,
            unsigned int rows, unsigned int cols, unsigned int height );
  ~atom();

  //status functions
  void print_properties();
  void print_pos();
  void print_velo();
  void print_accel();
  void print_potential();
  unsigned int give_radius();

  //variable changing functions
  void set_pos(unsigned int x, unsigned int y, unsigned int z );
  void set_velo(int dx, int dy, int dz );
  void set_accel(int ax, int ay, int az );

  //data saving functions
  void save_potential_csv( const char* filename );

  //kinematic functions
  void apply_velocity();
  void apply_acceleration();
};

class RGB_COLOR {
private:
  uint8_t r, g, b;
public:
  RGB_COLOR(uint8_t red, uint8_t green, uint8_t blue){
    r = red;
    g = green;
    b = blue;
  }
  ~RGB_COLOR(){}
};

void print_plane( int** matr, unsigned int cols, unsigned int rows );
double calc_potential( double q, int r, unsigned int *pos,
                      unsigned int x, unsigned int y, unsigned int height );
double calc_elec( double q, int r, unsigned int *pos,
                  unsigned int x, unsigned int y, unsigned int height );
void sleep( unsigned int ms );

double mat_max( double **mat );

double mat_min( double **mat );

unsigned int **normalize( double **mat, unsigned int rows, unsigned int cols );

//take a matrix of normalized greyscale values between 0 and 255_normalize
//convert into array of RGB_COLOR structs which tell the RBG value at each point
RGB_COLOR** color_graph( unsigned int **mat, unsigned int rows, unsigned int cols );
