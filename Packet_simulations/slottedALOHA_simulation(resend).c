#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define slotTotal 100000
#define stationTotal 100
#define resendRate 0.2
#define debugMode 0 //if 1 then on.
double lambda;
int dataResendProbabilityThreshold = resendRate * 100;
int resendFlag = 0;
int sendCancel = 1;
int MayCanceledStation, MayCanceledStation2;

typedef struct {
    int packetBuffer[stationTotal];
} SLOT;

typedef struct {
    int probability;
    int interval;
    int resendMode;
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
void stationInit(){
    for(int k=0; k<stationTotal; k++){
        station[k].probability = 0;
        station[k].interval = 0;
        station[k].resendMode = 0;
    }
}

int alreadySentOrNot(int slotIndex, int stationIndexNow){
    int temp = 0;
    for(int stationIndex=0; stationIndex<stationIndexNow; stationIndex++){
        temp += slot[slotIndex].packetBuffer[stationIndex];
    }
    if(temp == 0){
        return 0;
    }else if(temp == 1){
        return 1;
    }
}

int send_packet(STATION* station, int stationIndex, int slotIndex, int dataSendProbabilityThreshold, int* delayTotal){ //λを満たすかつinterval空ければ現在のslotの，現在のstationに対応するpacketBufferに書き込む．
    //if(debugMode == 1) printf("The probability = %d and the SPTh = %d\n", station[stationIndex].probability, dataSendProbabilityThreshold);
    // ↓通常送信
    int keyToLock = 0;
    if(station[stationIndex].resendMode == 0 && station[stationIndex].probability <= dataSendProbabilityThreshold && station[stationIndex].interval == 0 && alreadySentOrNot(slotIndex, stationIndex) == 0){
        slot[slotIndex].packetBuffer[stationIndex] = 1;
        //randomNumberZeroToTenthousandを用いて整数のインターバル値を入れる．
        station[stationIndex].interval = (int) (- (1 / (lambda) * log10(rand() / (double)RAND_MAX))) + 1;
        MayCanceledStation = stationIndex;
        if(debugMode == 1) printf("\tA datum was sent(Interval: %d).\n",station[stationIndex].interval);
        if(debugMode == 1) printf("\tslot[%d].packetBuffer[%d] = %d\n", slotIndex, stationIndex, slot[slotIndex].packetBuffer[stationIndex]);
    }
    // ↓通常モードから再送モードへ移行
    if(station[stationIndex].resendMode == 0 && station[stationIndex].probability <= dataSendProbabilityThreshold && station[stationIndex].interval == 0  && alreadySentOrNot(slotIndex, stationIndex) == 1){
        station[stationIndex].resendMode = 1;
        if(debugMode == 1) printf("\tstation[%d]'s changed its mode to ResendMode.\n", stationIndex); // failure to send.
        keyToLock = 1;
        *delayTotal += 1;
        resendFlag = 1;
    }
    // ↓再送
    if(keyToLock == 0 && station[stationIndex].resendMode == 1 && station[stationIndex].probability <= dataSendProbabilityThreshold && station[stationIndex].interval == 0 && alreadySentOrNot(slotIndex, stationIndex) == 0){
        slot[slotIndex].packetBuffer[stationIndex] = 1;
        station[stationIndex].interval = (int) (- (1 / (resendRate) * log10(rand() / (double)RAND_MAX))) + 1;
        station[stationIndex].resendMode = 0;
        MayCanceledStation2 = stationIndex;
        *delayTotal += 1;
        if(debugMode == 1) printf("\t// A datum was resent(Interval: %d).\n",station[stationIndex].interval);
        if(debugMode == 1) printf("\t// slot[%d].packetBuffer[%d] = %d\n", slotIndex, stationIndex, slot[slotIndex].packetBuffer[stationIndex]);
        if(debugMode == 1) printf("\t// station[%d]'s changed its mode to NormalMode.\n", stationIndex); // success to send.
    }
    // ↓衝突時に再送も取り消す
    if(keyToLock == 0 && station[stationIndex].resendMode == 1 && station[stationIndex].probability <= dataSendProbabilityThreshold && station[stationIndex].interval == 0 && alreadySentOrNot(slotIndex, stationIndex) == 1){
        resendFlag = 1;
    }
}

// ↓通常送信時
int sendProbability(STATION* station, int stationIndex, int slotIndex, int dataSendProbabilityThreshold, int* delayTotal){ // データ発生率λに従いデータ転送．
    int randomNumberZeroToTenthousand;
    randomNumberZeroToTenthousand = (rand() % 10000) + 1; // some changes will be needed. This num should be 0.0005 to 0.03 though the num ranges 0 to 10000;
    station[stationIndex].probability = randomNumberZeroToTenthousand;
    send_packet(station, stationIndex, slotIndex, dataSendProbabilityThreshold, delayTotal);
}

// ↓再送時
int sendProbability2(STATION* station, int stationIndex, int slotIndex, int dataSendProbabilityThreshold, int* delayTotal){
    int randomNumberZeroToOnehundred;
    randomNumberZeroToOnehundred = (rand() % 100) + 1;
    station[stationIndex].probability = randomNumberZeroToOnehundred;
    send_packet(station, stationIndex, slotIndex, dataResendProbabilityThreshold, delayTotal);
}

int main(void){
    if(debugMode == 1) printf("The simulation's started.\n");
    //double dataSendProbabilityThreshold = atof(argv[1]);
    //lambda = dataSendProbabilityThreshold;
    //dataSendProbabilityThreshold *= 10000;

    for(int k=5; k <= 300; k += 5){
        // Initialization
        slotInit(); //各slot中の，各stationに対応するpacketBufferを0に初期化
        stationInit();
        double dataSendProbabilityThreshold = (double) k / 10000;
        lambda = dataSendProbabilityThreshold;
        dataSendProbabilityThreshold *= 10000;
        int a;
        int* delayTotal;
        delayTotal = &a;
        *delayTotal = 0;

        // Simulation
        for(int slotIndex=0; slotIndex<slotTotal; slotIndex++){
            resendFlag = 0;
            for(int stationIndex=0; stationIndex<stationTotal; stationIndex++){
                if(station[stationIndex].resendMode == 0) sendProbability(station, stationIndex, slotIndex, dataSendProbabilityThreshold, delayTotal);
                if(station[stationIndex].resendMode == 1) sendProbability2(station, stationIndex, slotIndex, dataSendProbabilityThreshold, delayTotal);
                if(station[stationIndex].interval > 0){
                    station[stationIndex].interval--; //slotを経るごとにデータ発生間隔値を減らす．
                }else if(station[stationIndex].interval == 0 && station[stationIndex].resendMode == 1){
                    if(debugMode == 1) printf("\tstation[%d]'s been ready for resending.\n", stationIndex);
                }
            }
            // Collision
            if(resendFlag == 1){
                slot[slotIndex].packetBuffer[MayCanceledStation] = 0;   // やっぱり0にする
                slot[slotIndex].packetBuffer[MayCanceledStation2] = 0;  // 再送もやっぱり0にする
                station[MayCanceledStation].resendMode = 1;     // 衝突したら再送モードに
                station[MayCanceledStation].interval = 0; // 再送間隔もリセット
                if(debugMode == 1) printf("\tslot[%d]'spacketBuffer[%d] was canceled.\n", slotIndex, MayCanceledStation);
            }
            if(debugMode == 1) printf("[@ %.4f]Next slot ->\n", dataSendProbabilityThreshold / 10000);
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

        printf("\n[@ %.4f]The throughPut is\t\t%d\t(DelayTotal:\t%d\t).\n", dataSendProbabilityThreshold / 10000, throughPut, *delayTotal);
    }

    return 0;
}
