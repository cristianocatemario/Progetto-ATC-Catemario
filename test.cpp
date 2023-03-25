
#include "gtest/gtest.h"

//#include "Position.h"
#include "GPS.H"


class Driver : public GPSobserver
{
 public:

 Position* position_array;
 double* direction_array;
 double* speed_array;
 int n;
 int i;

 Driver(void)
 {
  i = 0;
  n = 60;
  position_array = new Position[n];
  direction_array = new double[n];
  speed_array = new double[n];
 }

 Driver(int array_dim)
 {
  i = 0;
  n = array_dim;
  position_array = new Position[n];
  direction_array = new double[n];
  speed_array = new double[n];
 }

 ~Driver(void)
 {
  delete position_array;
  delete direction_array;
  delete speed_array;
 }

 virtual void GPSnotify(Position position, double direction, double speed)
 {
  position_array[i] = position;
  direction_array[i] = direction;
  speed_array[i] = speed;
  i = (i+1)%n;
 }
};



TEST(GPS, UpdateAndNotify)
{
 Driver driver(10);
 GPS* gps = GPS::getGPS(1);
 int i;
 
 gps->subscribe(&driver);
 for (i=0; i<10; i++)
    gps->update();
 gps->unsubscribe();
 
 for (i=0; i<10; i++)
    {
  ASSERT_EQ((int)driver.position_array[i].getLatitude().getDegree(), 43);
  ASSERT_EQ((int)driver.position_array[i].getLatitude().getMinute(), 29);
  ASSERT_EQ((int)driver.position_array[i].getLatitude().getSecond(), 57);
  ASSERT_EQ((int)driver.position_array[i].getLongitude().getDegree(), 10);
  ASSERT_EQ((int)driver.position_array[i].getLongitude().getMinute(), 19);
  ASSERT_EQ((int)driver.position_array[i].getLongitude().getSecond(), 26);
  ASSERT_EQ((int)driver.position_array[i].getTime().getHour(), 9);
  ASSERT_EQ((int)driver.position_array[i].getTime().getMinute(), 0);
  ASSERT_EQ((int)driver.position_array[i].getTime().getSecond(), 0);
  ASSERT_EQ((int)driver.position_array[i].getDate().getDay(), 6);
  ASSERT_EQ((int)driver.position_array[i].getDate().getMonth(), 9);
  ASSERT_EQ((int)driver.position_array[i].getDate().getYear(), 2011);
	 ASSERT_FLOAT_EQ(driver.direction_array[i], 0.0);
	 ASSERT_FLOAT_EQ(driver.speed_array[i], 0.0);
    }
}

TEST(Position, DefaultConstructor)
{
 Position p;
 
 ASSERT_EQ(0, (int)p.getTime().getHour());
 ASSERT_EQ(0, (int)p.getTime().getMinute());
 ASSERT_EQ(0, (int)p.getTime().getSecond());
 ASSERT_EQ(1, (int)p.getDate().getDay());
 ASSERT_EQ(1, (int)p.getDate().getMonth());
 ASSERT_EQ(2000, (int)p.getDate().getYear());
 ASSERT_DOUBLE_EQ(0.0, p.getLatitude().getLatitude());
 ASSERT_DOUBLE_EQ(0.0, p.getLongitude().getLongitude());
}

TEST(Position, StandardConstructor)
{
 Time t0;
 Date d0;
 Latitude lat0;
 Longitude lon0;
 Position pos0(d0, t0, lat0, lon0);;
 
 ASSERT_EQ(0, (int)pos0.getTime().getHour());
 ASSERT_EQ(0, (int)pos0.getTime().getMinute());
 ASSERT_EQ(0, (int)pos0.getTime().getSecond());
 ASSERT_EQ(1, (int)pos0.getDate().getDay());
 ASSERT_EQ(1, (int)pos0.getDate().getMonth());
 ASSERT_EQ(2000, (int)pos0.getDate().getYear());
 ASSERT_DOUBLE_EQ(0.0, pos0.getLatitude().getLatitude());
 ASSERT_DOUBLE_EQ(0.0, pos0.getLongitude().getLongitude());

 Time t1(12,59,59);
 Date d1(25,12,2011);
 Latitude lat1(43,33,03,NORD); // Piazza Grande - Livorno
 Longitude lon1(10,18,33,EST); // Piazza Grande - Livorno
 Position pos1(d1, t1, lat1, lon1);

 ASSERT_EQ(12, (int)pos1.getTime().getHour());
 ASSERT_EQ(59, (int)pos1.getTime().getMinute());
 ASSERT_EQ(59, (int)pos1.getTime().getSecond());
 ASSERT_EQ(25, (int)pos1.getDate().getDay());
 ASSERT_EQ(12, (int)pos1.getDate().getMonth());
 ASSERT_EQ(2011, (int)pos1.getDate().getYear());
 ASSERT_NEAR(43.550729, pos1.getLatitude().getLatitude(), 0.0003); // 0.0003 = 1/3600
 ASSERT_NEAR(10.309113, pos1.getLongitude().getLongitude(), 0.0003); // 0.0003 = 1/3600
}

class PositionTest : public testing::Test
{
 public:

 Time *t0, *t1, *t2; 
 Date *d0, *d1, *d2; 
 Latitude *lat0, *lat1, *lat2; 
 Longitude *lon0, *lon1, *lon2; 
 Position *pos0, *pos1, *pos2;  
 
 virtual void SetUp(void)
 {
  d0 = new Date();
  d1 = new Date(1,1,2011); // 1/1/2011
  d2 = new Date(31,12,2011); // 31/12/2011
  t0 = new Time();
  t1 = new Time(12,0,0); // 12:00:00
  t2 = new Time(23,59,59); // 23:59:59
  lat0 = new Latitude();
  lat1 = new Latitude(43,33,03,NORD); // Piazza Grande - Livorno
  lat2 = new Latitude(43,43,23,NORD); // Piazza del duomo - Pisa
  lon0 = new Longitude();
  lon1 = new Longitude(10,18,33,EST); // Piazza Grande - Livorno
  lon2 = new Longitude(10,23,47,EST); // Piazza del duomo - Pisa
  pos0 = new Position();
  pos1 = new Position(*d1, *t0, *lat1, *lon1);
  pos2 = new Position(*d2, *t2, *lat2, *lon2);
 }

 virtual void TearDown(void)
 {
  delete d0;
  delete d1;
  delete d2;
  delete t0;
  delete t1;
  delete t2;
  delete lat0;
  delete lat1;
  delete lat2;
  delete lon0;
  delete lon1;
  delete lon2;
  delete pos0;
  delete pos1;
  delete pos2;
 }
};

TEST_F(PositionTest, ComputeUTM)
{
 ASSERT_NEAR(4822808.0, (*pos1).getNorth(), 50.0);
 ASSERT_NEAR(605746.0, (*pos1).getEast(), 50.0);

 ASSERT_NEAR(4842053.0, (*pos2).getNorth(), 50.0);
 ASSERT_NEAR(612467.0, (*pos2).getEast(), 50.0);
}

TEST_F(PositionTest, ComputeDistance)
{
 ASSERT_NEAR(20421.0, (*pos1).distance(*pos2), 100.0);
 ASSERT_NEAR(20421.0, (*pos2).distance(*pos1), 100.0);
}

TEST_F(PositionTest, ComputePeriod)
{
 ASSERT_NEAR((double)(365*24*60*60-1), (*pos1).elapsedTime(*pos2), 0.0003); // 0.0003 = 1/3600
 ASSERT_NEAR(-(double)(365*24*60*60-1), (*pos2).elapsedTime(*pos1), 0.0003); // 0.0003 = 1/3600
}


class TimeTest : public testing::Test
{
 public:
 
 Time *t0, *t1, *t2, *t3, *t4, *t5;

 virtual void SetUp(void)
 {
  t0 = new Time();
  t1 = new Time(24,0,0);
  t2 = new Time(0,60,0);
  t3 = new Time(12,0,60);
  t4 = new Time(12,0,0);
  t5 = new Time(23,59,59);
 }

 virtual void TearDown(void)
 {
  delete t0;
  delete t1;
  delete t2;
  delete t3;
  delete t4;
 }
};

TEST_F(TimeTest, ConstructorsAndGetters)
{
 ASSERT_TRUE((*t0).getHour()==0 && (*t0).getMinute()==0 && (*t0).getSecond()==0);
 ASSERT_TRUE((*t1).getHour()==0 && (*t1).getMinute()==0 && (*t1).getSecond()==0);
 ASSERT_TRUE((*t2).getHour()==0 && (*t2).getMinute()==0 && (*t2).getSecond()==0);
 ASSERT_TRUE((*t3).getHour()==0 && (*t3).getMinute()==0 && (*t3).getSecond()==0);
 ASSERT_TRUE((*t4).getHour()==12 && (*t4).getMinute()==0 && (*t4).getSecond()==0);
 ASSERT_TRUE((*t5).getHour()==23 && (*t5).getMinute()==59 && (*t5).getSecond()==59);
}

TEST_F(TimeTest, Difference)
{
 ASSERT_NEAR(23.99972, (*t0).hourDifference(*t5), .00001);
 ASSERT_NEAR(12.000000, (*t0).hourDifference(*t4), .00001);
 ASSERT_NEAR(-12.000000, (*t4).hourDifference(*t0), .00001);
 ASSERT_NEAR(11.999722, (*t4).hourDifference(*t5), 0.0001);
}

 
TEST(Date, DefaultConstructor)
{
 Date d;

 EXPECT_TRUE(d.getDay()==1 && d.getMonth()==1 && d.getYear()==2000);
}

TEST(Date, ConstructorFromWrongParameters)
{
 Date d0(31,2,2011);
 Date d1(1,0,2011);
 Date d2(29,2,2011);

 EXPECT_TRUE(d0.getDay()==1 && d0.getMonth()==1 && d0.getYear()==2000);
 EXPECT_TRUE(d1.getDay()==1 && d1.getMonth()==1 && d1.getYear()==2000);
 EXPECT_TRUE(d2.getDay()==1 && d2.getMonth()==1 && d2.getYear()==2000);
}

TEST(Date, ConstructorFromRightParameters)
{
 Date d0(29,2,2012);
 Date d1(23,3,1965);
 
 EXPECT_TRUE(d0.getDay()==29 && d0.getMonth()==2 && d0.getYear()==2012);
 EXPECT_TRUE(d1.getDay()==23 && d1.getMonth()==3 && d1.getYear()==1965);
}

TEST(Date, DifferenceInDay)
{
 Date d0;
 Date d1(23,3,1965);

 EXPECT_EQ(12702, d1.dayDifference(d0));
}

TEST(Latitude, DefaultConstructor)
{
 Latitude l;

 EXPECT_TRUE(l.getDegree()==0 && l.getMinute()==0 && l.getSecond()==0 && l.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l.getLatitude());
}

TEST(Latitude, ConstructorFromWrongParameters)
{
 Latitude l0(100,0,0,1);
 Latitude l1(10,100,0,-1);
 Latitude l2(10,0,100,2);

 EXPECT_TRUE(l0.getDegree()==0 && l0.getMinute()==0 && l0.getSecond()==0 && l0.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l0.getLatitude());
 EXPECT_TRUE(l1.getDegree()==0 && l1.getMinute()==0 && l1.getSecond()==0 && l1.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l1.getLatitude());
 EXPECT_TRUE(l2.getDegree()==0 && l2.getMinute()==0 && l2.getSecond()==0 && l2.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l2.getLatitude());
}

TEST(Latitude, ConstructorFromRightParameters)
{
 Latitude l0(43,29,56,NORD);
 Latitude l1(13,9,50,SUD);

 EXPECT_TRUE(l0.getDegree()==43 && l0.getMinute()==29 && l0.getSecond()==56 && l0.getOrientation()==1);
 ASSERT_NEAR(43.4988888, l0.getLatitude(), 0.000001);
 EXPECT_TRUE(l1.getDegree()==13 && l1.getMinute()==9 && l1.getSecond()==50 && l1.getOrientation()==-1);
 ASSERT_NEAR(-13.1638888, l1.getLatitude(), 0.000001);
}

TEST(Longitude, DefaultConstructor)
{
 Longitude l;

 EXPECT_TRUE(l.getDegree()==0 && l.getMinute()==0 && l.getSecond()==0 && l.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l.getLongitude());
}

TEST(Longitude, ConstructorFromWrongParameters)
{
 Longitude l0(200,0,0,1);
 Longitude l1(10,100,0,-1);
 Longitude l2(10,0,100,2);

 EXPECT_TRUE(l0.getDegree()==0 && l0.getMinute()==0 && l0.getSecond()==0 && l0.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l0.getLongitude());
 EXPECT_TRUE(l1.getDegree()==0 && l1.getMinute()==0 && l1.getSecond()==0 && l1.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l1.getLongitude());
 EXPECT_TRUE(l2.getDegree()==0 && l2.getMinute()==0 && l2.getSecond()==0 && l2.getOrientation()==1);
 EXPECT_DOUBLE_EQ(0.0, l2.getLongitude());
}

TEST(Longitude, ConstructorFromRightParameters)
{
 Longitude l0(10,19,26,EST);
 Longitude l1(72,32,45,WEST);

 EXPECT_TRUE(l0.getDegree()==10 && l0.getMinute()==19 && l0.getSecond()==26 && l0.getOrientation()==1);
 ASSERT_NEAR(10.32388888, l0.getLongitude(), 0.000001);
 EXPECT_TRUE(l1.getDegree()==72 && l1.getMinute()==32 && l1.getSecond()==45 && l1.getOrientation()==-1);
 ASSERT_NEAR(-72.5458333, l1.getLongitude(), 0.000001);
}




int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
