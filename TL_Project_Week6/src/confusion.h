#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H


void printConfusionMatrix(void);
void makeHundredFakeClassifications(void);
void makeOneClassificationAndUpdateConfusionMatrix(int,int,int,int);
int calculateDistanceToAllCentrePointsAndSelectWinner(int,int,int);
void resetConfusionMatrix(void);
float calculateDistance(int,int,int,int,int,int);
void softmax(float n2[6]);
void relu(float n1[10]);
void arraysum(float neuronit[], int r,float bias[r][1]);
void testi();
void arraytesti(float arr[2][2]);

float dotProduct(float,float,float,float,float,float);

void matmul_l1(float w1[3][10], float a0[3], float tulos[10]);
void matmul_l2(float w2[10][6], float a1[10], float tulos[6]);

void neuroverkko(int,int,int,int);
#endif
