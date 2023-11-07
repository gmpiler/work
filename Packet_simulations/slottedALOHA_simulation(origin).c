#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define slotTotal 100000
#define stationTotal 100
#define debugMode 0 //if 1 then on.
double lambda;

typedef struct {
    int packetBuffer[stationTotal];
} SLOT;

typedef struct {
    int probability;
    int interval;
} STATION;

SLOT slot[slotTotal];
STATION station[stationTotal];

void slotInit(){
    for(int k=0; k<slotTotal; k++){
        for(int l=0; l<stationTotal; l++){
            slot[k].packetBuffer[l] = 0;
        }
    }
}

int send_packet(STATION* station, int stationIndex, int slotIndex, int dataSendProbabilityThreshold){ //λを満たすかつinterval空ければ現在のslotの，現在のstationに対応するpacketBufferに書き込む．
    //if(debugMode == 1) printf("The probability = %d and the SPTh = %d\n", station[stationIndex].probability, dataSendProbabilityThreshold);
    if(station[stationIndex].probability <= dataSendProbabilityThreshold && station[stationIndex].interval == 0){
        slot[slotIndex].packetBuffer[stationIndex] = 1;
        //randomNumberZeroToTenthousandを用いて整数のインターバル値を入れる．
        station[stationIndex].interval = (int) (- (1 / (lambda) * log10(rand() / (double)RAND_MAX))) + 1;
        if(debugMode == 1) printf("\tA datum was sent(Interval: %d).\n",station[stationIndex].interval);
        if(debugMode == 1) printf("\tslot[%d].packetBuffer[%d] = %d\n", slotIndex, stationIndex, slot[slotIndex].packetBuffer[stationIndex]);
    }
}

int sendProbability(STATION* station, int stationIndex, int slotIndex, int dataSendProbabilityThreshold){ // データ発生率λに従いデータ転送．
    int randomNumberZeroToTenthousand;
    randomNumberZeroToTenthousand = (rand() % 10000) + 1; // some changes will be needed. This num should be 0.0005 to 0.03 though the num ranges 0 to 10000;
    station[stationIndex].probability = randomNumberZeroToTenthousand;
    send_packet(station, stationIndex, slotIndex, dataSendProbabilityThreshold);
}

int main(int argc, char* argv[]){
    printf("The simulation's started.\n");
    double dataSendProbabilityThreshold = atof(argv[1]);
    lambda = dataSendProbabilityThreshold;
    dataSendProbabilityThreshold *= 10000;

    // Initialization
    slotInit(); //各slot中の，各stationに対応するpacketBufferを0に初期化

    // Simulation
    for(int slotIndex=0; slotIndex<slotTotal; slotIndex++){
        for(int stationIndex=0; stationIndex<stationTotal; stationIndex++){
            sendProbability(station, stationIndex, slotIndex, dataSendProbabilityThreshold);
            if(station[stationIndex].interval > 0){
                station[stationIndex].interval--; //slotを経るごとにデータ発生間隔値を減らす．
            }else if(station[stationIndex].interval == 0){
                if(debugMode == 1) printf("\t\tstation[%d]'s been ready for sending.\n", stationIndex);
            }
        }
            if(debugMode == 1) printf("Next slot ->\n");
    }

    // Count throughput
    int throughPut = 0;
    for(int slotIndex=0; slotIndex<slotTotal; slotIndex++){
        int sumSlot = 0;
        for(int stationIndex=0; stationIndex<stationTotal; stationIndex++){
            sumSlot += slot[slotIndex].packetBuffer[stationIndex];
        }
        if(sumSlot == 1){
            throughPut += sumSlot;
        }
    }
    printf("\nThe throughPut is %d.\n", throughPut);

    return 0;
}
