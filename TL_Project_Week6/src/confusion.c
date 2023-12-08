#include <zephyr/kernel.h>
#include <math.h>
#include "confusion.h"
#include "adc.h"
#include "kmeans.h"
#include "neuroverkonKertoimet.h"
/* 
  K-means algorithm should provide 6 center points with
  3 values x,y,z. Let's test measurement system with known
  center points. I.e. x,y,z are supposed to have only values
  1 = down and 2 = up
  
  CP matrix is thus the 6 center points got from K-means algoritm
  teaching process. This should actually come from include file like
  #include "KmeansCenterPoints.h"
  
  And measurements matrix is just fake matrix for testing purpose
  actual measurements are taken from ADC when accelerator is connected.
*/ 

// tämä indeksoi k-means pisteet järjestykseen
int index_key[] = {5,0,1,3,4,2};


/*
int CP[6][3]={
	                     {1,0,0},
						 {2,0,0},
						 {0,1,0},
						 {0,2,0},
						 {0,0,1},
						 {0,0,2}
};
*/

int measurements[6][3]={
	                     {1,0,0},
						 {2,0,0},
						 {0,1,0},
						 {0,2,0},
						 {0,0,1},
						 {0,0,2}
};

int CM[6][6]= {0};
int CM_neuro[6][6] = {0};

float calculateDistance(int x1, int y1, int z1,
                        int x2, int y2, int z2){
   //kahden 3D-avaruuden pisteen välinen etäisyys on:
   //etäisyys = sqrt((x2-x1)**2+(y2-y1)**2+(z2-z1)**2)
   float x_delta = (float)x2 - (float)x1;
   float y_delta = (float)y2 - (float)y1;
   float z_delta = (float)z2 - (float)z1;

   float len = sqrt(pow(x_delta,2)+pow(y_delta,2)+pow(z_delta,2));

   return len;

}



void printConfusionMatrix(void)
{
	printk("Confusion matrix = \n");
	printk("   cp1 cp2 cp3 cp4 cp5 cp6\n");
	for(int i = 0;i<6;i++)
	{
		printk("cp%d %d   %d   %d   %d   %d   %d\n",i+1,CM[i][0],CM[i][1],CM[i][2],CM[i][3],CM[i][4],CM[i][5]);
	}

   printk("NEUROVERKON Confusion matrix = \n");
	printk("   cp1 cp2 cp3 cp4 cp5 cp6\n");
	for(int i = 0;i<6;i++)
	{
		printk("cp%d %d   %d   %d   %d   %d   %d\n",i+1,CM_neuro[i][0],CM_neuro[i][1],CM_neuro[i][2],CM_neuro[i][3],CM_neuro[i][4],CM_neuro[i][5]);
	}
}

void makeHundredFakeClassifications(void)
{
   /*******************************************
   Jos ja toivottavasti kun teet toteutuksen paloissa eli varmistat ensin,
   että etäisyyden laskenta 6 keskipisteeseen toimii ja osaat valita 6 etäisyydestä
   voittajaksi sen lyhyimmän etäisyyden, niin silloin voit käyttää tätä aliohjelmaa
   varmistaaksesi, että etäisuuden laskenta ja luokittelu toimii varmasti tunnetulla
   itse keksimälläsi sensoridatalla ja itse keksimilläsi keskipisteillä.
   *******************************************/

   //looppi joka tekee 100 kertaa tyhmän datapisteen suunnalla, arvaa tyhmän datapisteen winnerin
   //ja lopuksi confusion matrixiin koordinaattiin suunta,arvaussuunta lisätään yksi piste
   for (int i = 0; i<=99; i++){
      //arvotaan data
      int suunta = rand()%6;
      int x = rand()%2;
      int y = rand()%2;
      int z = rand()%2;

      makeOneClassificationAndUpdateConfusionMatrix(suunta,x,y,z);
   }


}

void makeOneClassificationAndUpdateConfusionMatrix(int direction,int x,int y,int z)
{
   /**************************************
   Tee toteutus tälle ja voit tietysti muuttaa tämän aliohjelman sellaiseksi,
   että se tekee esim 100 kpl mittauksia tai sitten niin, että tätä funktiota
   kutsutaan 100 kertaa yhden mittauksen ja sen luokittelun tekemiseksi.
   **************************************/

   int winner = calculateDistanceToAllCentrePointsAndSelectWinner(x,y,z);
   CM[direction][winner]++;

   
}

int calculateDistanceToAllCentrePointsAndSelectWinner(int x,int y,int z)
{
   /***************************************
   Tämän aliohjelma ottaa yhden kiihtyvyysanturin mittauksen x,y,z,
   laskee etäisyyden kaikkiin 6 K-means keskipisteisiin ja valitsee
   sen keskipisteen, jonka etäisyys mittaustulokseen on lyhyin.
   ***************************************/
  
   float minDistance = calculateDistance(x,y,z,CP[0][0],CP[0][1],CP[0][2]);
   float distance;
   int winner = 0;

   for (int i=0;i<6;i++){
      distance = calculateDistance(x,y,z,CP[index_key[i]][0],CP[index_key[i]][1],CP[index_key[i]][2]);
      if(minDistance >= distance){
         minDistance = distance;
         winner = i;
      }

   }
   return winner;
}

void resetConfusionMatrix(void)
{
	for(int i=0;i<6;i++)
	{ 
		for(int j = 0;j<6;j++)
		{
			CM[i][j]=0;
         CM_neuro[i][j]=0;
		}
	}
}

/* void softmax(float n2[6]){

   float sum = 0;

   for(int i=0;i<6;i++){
      sum += exp(n2[i]); //tässä ylivuodon eston kikkailu jota en ymmärrä
   }
   
   for(int i=0;i<6;i++){
      n2[i] = exp(n2[i])/ sum;
      //printk("%f\n", n2[i][0]);
   }
} */

void softmax(float n2[6]){
   //tämä on chatgpt koodia, jossa aiemman softmaxin yli-/alivuotobugi korjattu

   // Calculate softmax using log-sum-exp trick
   float maxVal = n2[0];
   for(int i = 1; i < 6; i++){
      if(n2[i] > maxVal) {
         maxVal = n2[i];
      }
   }

   float logSumExp = 0.0;
   for(int i = 0; i < 6; i++){
      logSumExp += exp(n2[i] - maxVal); //tässä kriittinen operaatio, - maxVal
   }

   float logSoftmax[6];
   for(int i = 0; i < 6; i++){
      logSoftmax[i] = n2[i] - maxVal - log(logSumExp);
   }

   // Exponentiate to get softmax probabilities
   for(int i = 0; i < 6; i++){
      n2[i] = exp(logSoftmax[i]);
   }
}

void relu(float n1[10]){
   

   for (int i=0;i<10;i++){
      if(n1[i] < 0){
         n1[i] = 0;
      }
      //printk("%f\n", n1[i][0]);
   }
}
void arraysum(float neuronit[], int r,float bias[r][1]){
   for(int i=0;i<r;i++){
      neuronit[i] += bias[i][0];
   }
}
/* void testi(){
   float valu[2][2] = {{1.1,2.2},{3.3,4.4}};
    arraytesti(valu);
   printk("{%f,%f}\n",valu[0][0],valu[0][1]);
   printk("{%f,%f}\n",valu[1][0],valu[1][1]);

   float n2[6][1] ={{-0.1601722240447998},{0.3263254761695862},{-0.16412590444087982},{0.1776800900697708},{0.03297794982790947},{0.18514344096183777}};

   softmax(n2);
   for(int i=0;i<6;i++){
      printk("%f\n", n2[i][0]);
   }

   float n1[10][1] = {{-5},{-4},{-3},{-2},{-1},{0},{1},{3},{4},{5}};

   relu(n1);
   for(int i=0;i<10;i++){
      printk("%f\n", n1[i][0]);
   }
   float jotain[] = {1.1,2.2,3.3};
   float jotain2[][1] = {{1.1},{2.2},{3.3}};

   arraysum(jotain,3,jotain2);
   for(int i=0;i<3;i++){
      printk("%f\n", jotain[i]);
   }

} */
void arraytesti(float arr[2][2]){
   arr[1][0] = 5.5;
   
}
   

float dotProduct(float x1, float y1, float z1,
                  float x2, float y2, float z2)
{
   float product = x2*x1 + y2*y1 + z2*z1;
   return product;
}

void matmul_l1(float w1[3][10], float a0[3], float tulos[10]){
   //w1 on muotoa [3][10]
   //a0 on siis oltava muotoa [1][3]
   //jokainen w1[3][rivi] pistetulotetaan a0 kanssa
   for(int i=0;i<10;i++){
      //tähän yhden rivin ja sarakkeen pistetulo

      float pistetulo = a0[0]*w1[0][i] + a0[1]*w1[1][i] + a0[2]*w1[2][i];
      tulos[i] = pistetulo;

   }
}


void matmul_l2(float w2[10][6], float a1[10], float tulos[6])
{
   //matriisikertolasku 10x6 * 1x10
   float pistetulo;
   for(int i=0;i<6;i++){
      for(int j=0;j<10;j++){
         pistetulo = pistetulo + w2[j][i]*a1[j];
      }
      tulos[i] = pistetulo;
      //printk("2. krs tulostaulukon %d:s elementti: %d\n",i+1,tulos[i]);
      pistetulo = 0;
   }

}

void neuroverkko(int dir, int x, int y, int z){
   float tulos1[] = {0,0,0,0,0,0,0,0,0,0};
   float a0[] = {x,y,z};
   printk("%f, %f, %f\n",a0[0],a0[1],a0[2]);
   float tulos2[] = {0,0,0,0,0,0};

   //Tässä lasketaan neuroverkon ensimmäinen hidden layer tulos1 matriisiin
   matmul_l1(str_w1,a0,tulos1);
   arraysum(tulos1,10,str_b1);
   relu(tulos1);

   for(int i=0;i<10;i++){
      printk("l1 tulosmatriisin %d. luku on %f\n",i+1,tulos1[i]);
      k_sleep(K_MSEC(10));
      //printk("l2 tulosmatriisin %d. luku on %f\n",i+1,tulos2[i]); 
   } 

   matmul_l2(str_w2,tulos1,tulos2);
   arraysum(tulos2,6,str_b2);
      for(int i=0;i<6;i++){
      printk("l2 tulosmatriisin %d. luku on %f\n",i+1,tulos2[i]);
      k_sleep(K_MSEC(10));
   } 
   softmax(tulos2);
   for(int i=0;i<6;i++){
      printk("l2 tulosmatriisin %d. luku on %f\n",i+1,tulos2[i]);
      k_sleep(K_MSEC(10));
   }


   float max = -__FLT_MAX__;
   int index = 0;
   for(int i=0;i<6;i++){
      if(tulos2[i]>= max){
         max = tulos2[i];
         index = i;
      }
   }
   CM_neuro[dir][index]++;
}

