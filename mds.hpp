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
  double** get_potential_field();

  //variable changing functions
  void set_pos(unsigned int x, unsigned int y, unsigned int z );
  void set_velo(int dx, int dy, int dz );
  void set_accel(int ax, int ay, int az );

  //data saving functions
  void save_potential_csv( const char* filename );

  //kinematic functions
  void apply_velocity();
  void apply_acceleration();
  void calc_potential_grid();
};


void print_plane( int** matr, unsigned int cols, unsigned int rows );
double calc_potential( double q, int r, unsigned int *pos,
                      unsigned int x, unsigned int y, unsigned int height );
double calc_elec( double q, int r, unsigned int *pos,
                  unsigned int x, unsigned int y, unsigned int height );
void sleep( unsigned int ms );

double mat_max( double **mat, unsigned int rows, unsigned int cols );

double mat_min( double **mat, unsigned int rows, unsigned int cols );

unsigned int **normalize( double **mat, unsigned int rows, unsigned int cols );

void print_potential( double** field, unsigned int cols, unsigned int rows );
