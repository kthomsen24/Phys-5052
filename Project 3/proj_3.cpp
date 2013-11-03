/*
  Project 3
  Phys 5052
  A. Kafle, J. Primus, K. Thomsen
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

typedef double F(double,double);        // defining a type allowing the program
                                        // to more closely parallel the book's
                                        // notation, namely renaming
                                        // v'(t,v) = f(t,v)

vector<vector<double> > results;        // vector of vectors containing the
                                        // results of the calculations
                                        // row 0: t values
                                        // row 1: modified Euler results
                                        // row 2: RK4 results

vector< vector<double> > err;           // vector of vectors containing the
                                        // actual errors in the calculations
                                        // row 0: t values
                                        // row 1: modified Euler errors
                                        // row 2: RK4 errors

  double h;                             // step size

// constant declarations
  double g = 9.80665;                   // standard Earth gravity in m * s^(-2)
  double m = 1e-2;                      // mass of the projectile in kg
  double k = 1e-4;                      // drag coefficient for spherical
                                        // projectile in kg * m^(-1)

  double v0 = 0;                        // initial velocity in m * s^(-1)
  double a0 = g - k / m * pow(v0,2);    // initial acceleration in m * s^(-2)
  double low = 0, high = 10;            // lower and upper limits of integration in s

// test case 1
//   double v0 = 0;
//   double a0 = 0;
//   double low = 0, high = 1;

// test case 2
//   double v0 = 1;
//   double a0 = 0;
//   double low = 0, high = 3.14159;

// calculating v(t) via the modified Euler method
void mod_euler(F f, double h)
{
    double v = v0;
    double counter = 0;
    for (double t = 0; t < (high-low); t += h)
    {
      results[0][counter] = t;
      results[1][counter] = v;
      v += h * f(t+h/2, v+h/2);
      counter++;
    }
}

// calculating v(t) via RK4
void rk4(F f, double h)
{
    double v = v0;
    double counter = 0;
    double f0, f1, f2, f3;
    for (double t = low; t < (high-low); t += h)
    {
      f0 = h * f(t, v);
      f1 = h * f(t + h / 2, v + f0 / 2);
      f2 = h * f(t + h / 2, v + f1 / 2);
      f3 = h * f(t + h, v + f2);
      results[2][counter] = v;
      v += (f0 + 2 * f1 + 2 * f2 + f3) / 6;
      counter++;}
}

// the DE expressed as an equation for the first derivative
double v_prime(double t, double v)
{
  return g - k / m * pow(v,2);

// test case 1
//   return pow(t,2) + 1;

// test case 2
//   return -sin(t);
}

// the analytic solution to the DE
double v_actual(double t)
{
  return sqrt((m*g)/k) * tanh(sqrt((k*g)/m)*t);
  
// test case 1
//   return tan(t);
  
// test case 2
//   return cos(t);
}

// print results to the terminal
void print_results()
{
  int counter, counter2;
    cout << "result tables for h = " << setprecision(2) << h << endl;
    cout << "    x    \t|\t modEuler\t|\t   RK4    \t|\n";
  for (counter = 0; counter < 10/h+1; counter++)
    {  for (counter2 = 0; counter2 <= 2; counter2++)
      {
        cout << fixed << setprecision(8) <<
        results[counter2][counter] << "\t" <<  "|" <<"\t";
      }
      cout << endl;
    }
   cout << "------------------------------\n";
}

// calculate errors
void errors()
{
  err.resize(3);
  for(int counter = 0; counter < 3; counter++)
    {err[counter].resize(10/h+1);}
    
  for (int counter = 0; counter < 10/h+1; counter++)
    { err[0][counter] = results[0][counter];
      for (int counter2 = 1; counter2 <= 2; counter2++)
        {
          if (counter == 0) {err[counter2][counter] = 0;} else {
          err[counter2][counter] = fabs(v_actual(results[0][counter])
                - results[counter2][counter]);}
        }
    }
}

// print errors to the terminal
void print_errors()
{
  int counter, counter2;
    cout << "errors for h = " << setprecision(2) << h << endl;
    cout << "    x    \t|\t  Euler \t|\t modEuler\t|\t impEuler\t|\t   RK4    \t|\n";
  for (counter = 0; counter < 10/h+1; counter++)
    {  for (counter2 = 0; counter2 < 3; counter2++)
      {
        cout << fixed << setprecision(8) <<
        err[counter2][counter] << "\t" <<  "|" <<"\t";
      }
      cout << endl;
    }
   cout << "------------------------------\n";
}

// create output files for making graphs with gnuplot
void print_gnuplot()
{
// setting up velocity output files
  fstream f_modeuler_vel;
  f_modeuler_vel.open("mod_euler_vel.dat", ios::out);
  fstream f_rk4_vel;
  f_rk4_vel.open("rk4_vel.dat", ios::out);

// setting up velocity error output files
  fstream f_modeuler_vel_err;
  f_modeuler_vel_err.open("mod_euler_vel_err.dat", ios::out);
  fstream f_rk4_vel_err;
  f_rk4_vel_err.open("rk4_vel_err.dat", ios::out);

  // setting up position output files
  fstream f_modeuler_pos;
  f_modeuler_pos.open("mod_euler_pos.dat", ios::out);
  fstream f_rk4_pos;
  f_rk4_pos.open("rk4_pos.dat", ios::out);

  int counter, counter2;                // counter variables

// outputting modified Euler results to files
  for (counter = low; counter < high/h+1; counter++)
    {
      f_modeuler_vel << results[0][counter] << "" << 
      "," <<" " << results[1][counter] << endl;
      f_modeuler_vel_err << results[0][counter] << "" << 
      "," <<" " << err[1][counter] << endl;
      f_modeuler_pos << results[0][counter] << "" << 
      "," <<" " << results[1][counter]*results[0][counter] << endl;
    }

// outputting RK4 results to files
  for (counter = low; counter < high/h+1; counter++)
    {
      f_rk4_vel << results[0][counter] << "" <<  ","
      <<" " << results[2][counter] << endl;
      f_rk4_vel_err << results[0][counter] << "" << 
      "," <<" " << err[2][counter] << endl;
      f_rk4_pos << results[0][counter] << "" <<  ","
      <<" " << results[2][counter]*results[0][counter] << endl;
    }

  f_modeuler_vel.close();
  f_rk4_vel.close();
  f_modeuler_vel_err.close();
  f_rk4_vel_err.close();
  f_modeuler_pos.close();
  f_rk4_pos.close();
}

void computations()
{
    for (int counter = 0; counter <= 2; counter++)
      {results[counter].resize((high-low)/h+1);}
    mod_euler(v_prime, h);
    rk4(v_prime, h);
    errors();
    print_gnuplot();
}

int main()
{
  results.resize(3);
//   h = 2;
//   computations();
  h = .1;
  computations();
//   h = .5;
//   computations();
}