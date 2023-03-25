/*!	\file	GPS2UTM.H
*/

#include <math.h>

#define PI						3.141592653589793

#define WGS84_E2				0.006694379990197
#define WGS84_E4				WGS84_E2*WGS84_E2
#define WGS84_E6				WGS84_E4*WGS84_E2
#define WGS84_SEMI_MAJOR_AXIS	6378137.0
#define WGS84_SEMI_MINOR_AXIS	6356752.314245
#define UTM_LONGITUDE_OF_ORIGIN 3.0/180.0*PI
#define UTM_LATITUDE_OF_ORIGIN	0.0
#define UTM_FALSE_EASTING		500000.0
#define UTM_FALSE_NORTHING_N	0.0
#define UTM_FALSE_NORTHING_S	10000000.0
#define UTM_SCALE_FACTOR		0.9996


double m_calc(double latitude)
{
 return (1.0 - WGS84_E2/4.0 - 3.0*WGS84_E4/64.0 - 5.0*WGS84_E6/256.0)*latitude - (3.0*WGS84_E2/8.0 + 3.0*WGS84_E4/32.0 + 45.0*WGS84_E6/1024.0)*sin(2.0*latitude) + (15.0*WGS84_E4/256.0 + 45.0*WGS84_E6/1024.0)*sin(4.0*latitude) - (35.0*WGS84_E6/3072.0)*sin(6.0*latitude);
}

/*!	
 	\brief conversione di coordinate WGS84 (latitudine/longitudine) in coordinate UTM (metri)
 	\param latitude latitudine decimale (Nord se positiva, Sud se negativa)
 	\param longitude longitudine decimale (Est se positiva, Ovest se negativa)
 	\retval east coordinata UTM Est
 	\retval north coordinata UTM Nord
 */
void GPS2UTM(double latitude, double longitude, double* east, double* north)
{
 int int_zone;
 double M, M_origin, A, A2, e2_prim, C, T, v;
 
 int_zone = (int)(longitude/6.0);
 if (longitude < 0)
   int_zone--;
 longitude -= (double)(int_zone)*6.0;
 longitude *= PI/180.0;
 latitude *= PI/180.0;
 M = WGS84_SEMI_MAJOR_AXIS*m_calc(latitude);
 M_origin = WGS84_SEMI_MAJOR_AXIS*m_calc(UTM_LATITUDE_OF_ORIGIN);
 A = (longitude - UTM_LONGITUDE_OF_ORIGIN)*cos(latitude);
 A2 = A*A;
 e2_prim = WGS84_E2/(1.0 - WGS84_E2);
 C = e2_prim*pow(cos(latitude),2.0);
 T = tan(latitude);
 T *= T;
 v = WGS84_SEMI_MAJOR_AXIS/sqrt(1.0 - WGS84_E2*pow(sin(latitude),2.0));
 *north = UTM_SCALE_FACTOR*(M - M_origin + v*tan(latitude)*(A2/2.0 + (5.0 - T + 9.0*C + 4.0*C*C)*A2*A2/24.0 + (61.0 - 58.0*T + T*T + 600.0*C - 330.0*e2_prim)*A2*A2*A2/720.0));
 if (latitude < 0)
   *north += UTM_FALSE_NORTHING_S;
 *east = UTM_FALSE_EASTING + UTM_SCALE_FACTOR*v*(A + (1.0 - T + C)*A2*A/6.0 + (5.0 - 18.0*T + T*T + 72.0*C - 58.0*e2_prim)*A2*A2*A/120.0);
 return;
}
