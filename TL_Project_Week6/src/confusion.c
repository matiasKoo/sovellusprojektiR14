#include <zephyr/kernel.h>
#include <math.h>
#include "confusion.h"
#include "adc.h"
#include "kmeans.h"

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
		}
	}
}

