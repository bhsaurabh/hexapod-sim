#include <stdio.h>
#include <stdlib.h>

extern "C" {
	#include "extApi.h"
}

int main(int argc, char *argv[]) {
	int port = 19997;
	int clientId = simxStart((simxChar*)"127.0.0.1", port, true, true, 2000, 5);
	if (clientId == -1) {
		printf("Error: Could not create connection.\n");
		return -1;
	}
	// lets start simulation
	simxStartSimulation(clientId, simx_opmode_oneshot);
	int first = -1;	
	int numOfIters = 1000;
	while((simxGetConnectionId(clientId) != -1) && (numOfIters-- >= 1)) {
		// getting a couple of Joint Handles
		simxInt joint11;	
	
		simxInt result = simxGetObjectHandle(clientId, "hexa_joint2_2", &joint11, simx_opmode_oneshot_wait);
		if (result != simx_error_noerror) {
			printf("An error occured in getting Joint Handle\n");
			break;
		}
		// lets get joint position
		simxFloat position[12];
		if (first == -1) {
			first = 0;
			result = simxGetJointMatrix(clientId, joint11, position, simx_opmode_streaming);
		}
		else {
			result = simxGetJointMatrix(clientId, joint11, position, simx_opmode_buffer);
		}
		extApi_sleepMs(10);

		if (result == simx_error_remote_error_flag)
			printf("Check handle\n");
		printf("The position is: %f\n", (float)(position[0]));
		extApi_sleepMs(5);	
	}	
	// stop the simulation
	simxStopSimulation(clientId, simx_opmode_oneshot);	

	simxFinish(clientId);	
	return 0;
}
 

