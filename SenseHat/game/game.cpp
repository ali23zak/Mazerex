#include <SenseHat.h>
#include "RTIMULib.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <tuple>


using namespace std;
SenseHat senseHat;

#define r 	0xF800
#define b   0x0000
#define w   0xFFFF
#define g   0x07E0

int red = 63488;
int blue = 0;
int white = 65535;
int green = 2016;

float pitch,roll,yaw;
uint16_t pixel;
uint16_t pixel1;
uint16_t pixel2;

int x = 1;
int y = 1;

int game_over = 1;

uint16_t maze[8][8] ={{r,r,r,r,r,r,r,r},{r,b,b,b,b,b,b,r},{r,r,r,b,r,r,r,r},{r,b,r,b,r,b,b,r},{r,b,b,b,b,b,b,r},{r,b,r,r,r,r,b,r},{r,b,b,r,g,b,b,r},{r,r,r,r,r,r,r,r}};
uint16_t maze_over[8][8] ={{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g},{g,g,g,g,g,g,g,g}};
uint16_t maze_end[8][8] ={{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r},{r,r,r,r,r,r,r,r}};
uint16_t maze1[8][8] ={{r,r,r,r,r,r,r,r},{r,r,b,b,b,b,r,r},{r,b,r,r,r,r,b,r},{r,b,r,b,g,r,b,r},{r,b,r,b,b,r,b,r},{r,b,b,b,r,b,b,r},{r,r,b,b,r,b,r,r},{r,r,r,r,r,r,r,r}};
uint16_t maze2[8][8] ={{b,b,b,b,b,r,b,b},{b,r,b,b,r,b,b,b},{r,b,b,r,b,b,r,b},{b,b,r,b,b,r,b,b},{b,r,b,b,r,b,b,r},{b,b,b,r,b,b,b,b},{b,r,r,b,r,b,r,b},{r,b,b,r,b,r,b,g}};
uint16_t maze3[8][8] ={{b,b,b,r,b,b,b,b},{b,r,b,r,b,r,b,b},{b,r,b,r,b,r,b,b},{b,r,b,r,b,r,b,b},{b,r,b,r,b,r,b,b},{b,r,b,r,b,r,b,b},{b,r,b,r,b,r,b,b},{b,r,b,b,b,r,b,g}};

tuple<int, int> check_wall(int x, int y, int new_x, int new_y)
{
//	std::cout << "The color value is : "<< maze[new_y][new_x] << std::endl;
//	std::cout << "The color value is : "<< maze[new_y][x] << std::endl;
//	std::cout << "The color value is : "<< maze[y][new_x] << std::endl;
//	std::cout << "The value of x in check_wall is : "<< x << std::endl;
//	std::cout << "The value of y in check_wall is : "<< y << std::endl;

	if ((maze[new_y][new_x]) != red){
	//std::cout << "returning new_x and new_y : " << std::endl;
	//std::cout << "value returning is: "<< new_x << std::endl;
	return {new_x,new_y};
	}
	else if ((maze[new_y][x]) != red){
	//std::cout << "returning x and new_y : " << std::endl;
	return  {x, new_y};
	}
	else if ((maze[y][new_x]) != red){
	//std::cout << "returning new_x and y : " << std::endl;
	return  {new_x, y};
	}
	else{
	//std::cout << "returning x and y : " << std::endl;
	return {x, y};
	}
}

tuple<int, int>move_marble(float pitch, float roll, int x, int y) 
{
	int new_x = x;
	int new_y = y;
	if (-200 < pitch && pitch < -10 && x != 7){
		//std::cout << "new_x increased : " << std::endl;
		new_x += 1;
		//std::cout << "new_x increased value is : "<< new_x << std::endl;

	}
	else if (30 > pitch && pitch > 1 && x != 0){
		//std::cout << "new_x decreased : " << std::endl;
		new_x -= 1;	
	}

	if (10 < roll && roll < 170 && y != 7){
		//std::cout << "new_y increased : " << std::endl;
		new_y += 1;	
	}
	else if (-350 < roll && roll <0 && y != 0){
		//std::cout << "new_x decreased : " << std::endl;
		new_y -= 1;	
	}
	//std::cout << "move_marble x is : "<< new_x <<" move_marble y is : "<< new_y << std::endl;
	tie(x, y) = check_wall(x, y, new_x, new_y);
	//std::cout << "check_wall x is : "<< x <<" check_wall y is : "<< y << std::endl;
	return {x, y};
}


void check_win (int x, int y)
{
	while((maze[y][x]) == green){
		senseHat.Effacer();
		sleep(0.01);
		for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            pixel1 = maze_over[i][j];
            senseHat.AllumerPixel(i,j,pixel1);
			
		}
		}
		sleep(3);

		//std::cout << "GAME OVER "<< std::endl;
		::game_over =game_over+1;
		std::cout << " GAME OVER YOU WON "<< std::endl;
		std::cout << " the value of game_over is "<< game_over << std::endl;
		sleep(0.01);
		if (game_over == 2){
		senseHat.Effacer();
		sleep(0.01);
		std::cout << " maze1 is updated "<< std::endl;
		for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            ::maze[i][j] = maze1[i][j];
        }
        } 
        ::x = 5;
		::y = 1;
		break;
		}
		else if (game_over == 3){
		senseHat.Effacer();
		sleep(0.01);
		std::cout << " maze1 is updated "<< std::endl;
		for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            ::maze[i][j] = maze2[i][j];
        }
        } 
        ::x = 0;
		::y = 0;
		break;
		}
		else if (game_over == 4){
		senseHat.Effacer();
		sleep(0.01);
		std::cout << " maze1 is updated "<< std::endl;
		for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            ::maze[i][j] = maze3[i][j];
        }
        } 
        ::x = 0;
		::y = 7;
		break;
		}
	else{
		senseHat.Effacer();
		sleep(0.01);
		break;
	}
	}
}

tuple<float, float, float>G_data()
{	int sampleCount = 0;
    int sampleRate = 0;
    uint64_t rateTimer;
    uint64_t displayTimer;
    uint64_t now;

    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

    RTIMU *imu = RTIMU::createIMU(settings);
    RTPressure *pressure = RTPressure::createPressure(settings);
    RTHumidity *humidity = RTHumidity::createHumidity(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    imu->IMUInit();
    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);

    if (pressure != NULL)
        pressure->pressureInit();

    if (humidity != NULL)
        humidity->humidityInit();

    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
    int take_data = 1;
	while (take_data == 1) {

        usleep(imu->IMUGetPollInterval() * 1000);

        while (imu->IMURead()) {
            RTIMU_DATA imuData = imu->getIMUData();

            if (pressure != NULL)
                pressure->pressureRead(imuData);

            if (humidity != NULL)
                humidity->humidityRead(imuData);

            sampleCount++;

            now = RTMath::currentUSecsSinceEpoch();


            if ((now - displayTimer) > 200000) {
                 string str= {RTMath::displayDegrees("", imuData.fusionPose)};
                imuData.fusionPose;
    			::roll = int (imuData.fusionPose.x() * RTMATH_RAD_TO_DEGREE);
    			::pitch = int (imuData.fusionPose.y() * RTMATH_RAD_TO_DEGREE);
    			::yaw = int (imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE);
    			take_data = 2;
    			return{pitch,roll,yaw};
    			
                if (pressure != NULL) {
                }
                if (humidity != NULL) {
                }
                displayTimer = now;
            }
            if ((now - rateTimer) > 1000000) {
                sampleRate = sampleCount;
                sampleCount = 0;
                rateTimer = now;
            }
        }
        
 	}
    return {pitch,roll,yaw};
}		

int main()
{
	senseHat.Effacer();
 	while (game_over < 5 ){
 		tie (pitch,roll,yaw) = G_data();
 		//std::cout << "orientation pitch : " << pitch << " roll : " << roll << " yaw : " << yaw << std::endl;
 		tie (::x, ::y) = move_marble(pitch,roll, x, y);
 		//std::cout << "new_x is : "<< x <<" new_y is : "<< y << std::endl;
		check_win(x, y);
 		::maze[y][x] = white;
 		//senseHat.AfficherMotif(maze);
 		for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
        //std::cout << "test pointer maze is : "<< maze[i][j] << std::endl;
            pixel = maze[i][j];
            senseHat.AllumerPixel(i,j,pixel);
        }
        } 		
 		sleep(0.01);
 		::maze[y][x] = blue;
 		//std::cout << "The color value of green is : "<< maze[y][x] << std::endl;
 	}
 		senseHat.Effacer();
		sleep(0.01);
 		for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
        //std::cout << "test pointer maze is : "<< maze[i][j] << std::endl;
            pixel2 = maze_end[i][j];
            senseHat.AllumerPixel(i,j,pixel2);
        }
        } 
        sleep(3);
       	senseHat.Effacer(); 
 	return 0;
}
