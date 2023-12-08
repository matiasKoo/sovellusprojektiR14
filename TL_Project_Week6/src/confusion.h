#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H


void printConfusionMatrix(void);
void makeHundredFakeClassifications(void);
void makeOneClassificationAndUpdateConfusionMatrix(int,int,int,int);
int calculateDistanceToAllCentrePointsAndSelectWinner(int,int,int);
void resetConfusionMatrix(void);
float calculateDistance(int,int,int,int,int,int);
void softmax(float n2[6][1]);
void relu(float n1[10][1]);
void arraysum(float neuronit[], int r,float bias[r][1]);
void testi();
void arraytesti(float arr[2][2]);

#endif
