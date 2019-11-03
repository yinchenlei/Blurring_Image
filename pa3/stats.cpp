
#include "stats.h"

stats::stats(PNG & im){

hist.resize(im.width(), vector<vector<int>> (im.height(), vector<int>(36)));

sumHueX.resize(im.width(),vector<double>(im.height()));
sumHueY.resize(im.width(),vector<double>(im.height()));
sumSat.resize(im.width(),vector<double>(im.height()));
sumLum.resize(im.width(),vector<double>(im.height()));
double histogram;
double tempH;
double tempHueX;
double tempHueY;
double tempSat;
double tempLum;
double x;
double y;
double s;
double l;
for(int i = 0; i< im.width(); i++)
{
  for(int j = 0; j<im.height(); j++)
{

  HSLAPixel* p = im.getPixel(i,j);
  tempH = p->h;
  //calculate bin
  int bin = tempH/10;
  tempSat = p->s;
  tempLum = p->l;
  tempHueX = cos(PI/180*tempH );
  tempHueY = sin(PI/180*tempH );


    if(i>0){

        x = sumHueX[i-1][j];
        y = sumHueY[i-1][j];
        s = sumSat[i-1][j];
        l = sumLum[i-1][j];
      }

      for(int k = 0; k<36 ; k++)
      {
        if(i>0)  histogram = hist[i-1][j][k];
        if(j>0 && i>0)
        {
          hist[i][j][k] = histogram+hist[i][j-1][k]-hist[i-1][j-1][k];
        }else if(j>0)
        {
          hist[i][j][k] = histogram+hist[0][j-1][k];
        }else
        {
            hist[i][j][k] = histogram;
        }
      if(k == bin)
      {
        hist[i][j][bin]++;
      }
    }
    if(j>0 && i>0)
    {

      sumHueX[i][j]=x+tempHueX+sumHueX[i][j-1]-sumHueX[i-1][j-1];
      sumHueY[i][j]=y+tempHueY+sumHueY[i][j-1]-sumHueY[i-1][j-1];
      sumSat[i][j]=s+tempSat+sumSat[i][j-1]-sumSat[i-1][j-1];
      sumLum[i][j]=l+tempLum+sumLum[i][j-1]-sumLum[i-1][j-1];
    }else if(j>0)
    {

      sumHueX[i][j]=x+tempHueX+sumHueX[0][j-1];
      sumHueY[i][j]=y+tempHueY+sumHueY[0][j-1];
      sumSat[i][j]=s+tempSat+sumSat[0][j-1];
      sumLum[i][j]=l+tempLum+sumLum[0][j-1];

    }else
    {

      sumHueX[i][j]=x+tempHueX;
      sumHueY[i][j]=(y+tempHueY);
      sumSat[i][j]=(s+tempSat);
      sumLum[i][j]=(l+tempLum);
    }
  }
}
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

  /* your code here */
  long area = (long)(lr.first-ul.first+1) * (lr.second - ul.second+1);
  return area;

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

/* your code here */
double hueX;
double hueY;
double s;
double l;
if(ul.first>0  && ul.second> 0) {
 hueX = sumHueX[ul.first-1][ul.second-1] +sumHueX[lr.first][lr.second]
                -sumHueX[lr.first][ul.second-1] -sumHueX[ul.first-1][lr.second];
 hueY = sumHueY[ul.first-1][ul.second-1] + sumHueY[lr.first][lr.second ]
                -sumHueY[lr.first][ul.second-1 ]-sumHueY[ul.first-1][lr.second];
 s = sumSat[ul.first-1][ul.second-1] + sumSat[lr.first][lr.second ]
                -sumSat[lr.first][ul.second-1 ]-sumSat[ul.first-1][lr.second];
 l = sumLum[ul.first-1][ul.second-1] + sumLum[lr.first][lr.second ]
                -sumLum[lr.first][ul.second-1]-sumLum[ul.first-1][lr.second];
}else if(ul.first>0)
{
 hueX = sumHueX[lr.first][lr.second]-sumHueX[ul.first-1][lr.second];
 hueY = sumHueY[lr.first][lr.second ]-sumHueY[ul.first-1][lr.second];
s = sumSat[lr.first][lr.second ]-sumSat[ul.first-1][lr.second];
l = sumLum[lr.first][lr.second ]-sumLum[ul.first-1][lr.second];
}else if(ul.second >0)
  {
    hueX =  sumHueX[lr.first][lr.second]
          -sumHueX[lr.first][ul.second-1];
    hueY = sumHueY[lr.first][lr.second ]
          -sumHueY[lr.first][ul.second-1];
     s =  sumSat[lr.first][lr.second ]
        -sumSat[lr.first][ul.second-1];
     l =  sumLum[lr.first][lr.second ]-
        sumLum[lr.first][ul.second-1];
  }else
  {
     hueX = sumHueX[lr.first][lr.second];
     hueY = sumHueY[lr.first][lr.second ];
     s =  sumSat[lr.first][lr.second ];
     l = sumLum[lr.first][lr.second ];
  }
/**  hueX = sumHueX[ul.first][ul.second] +sumHueX[lr.first][lr.second]
                 -sumHueX[lr.first][ul.second] -sumHueX[ul.first][lr.second];
  hueY = sumHueY[ul.first][ul.second] + sumHueY[lr.first][lr.second ]
                 -sumHueY[lr.first][ul.second ]-sumHueY[ul.first][lr.second];
  s = sumSat[ul.first][ul.second] + sumSat[lr.first][lr.second ]
                 -sumSat[lr.first][ul.second ]-sumSat[ul.first][lr.second];
  l = sumLum[ul.first][ul.second] + sumLum[lr.first][lr.second ]
                 -sumLum[lr.first][ul.second]-sumLum[ul.first][lr.second];*/
long area = rectArea(ul, lr);
hueX = hueX / area;
hueY = hueY / area;
s = s/area;
l = l/area;
double h =   atan2(hueY, hueX) *180/PI;
return HSLAPixel(h, s, l,1.0);
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

/* your code here */
   vector<int> temp(36);
  for(int i = 0; i< 36; i++)
  {
    if(ul.first> 0 && ul.second >0)
    {
      temp[i] = hist[ul.first-1][ul.second-1][i] + hist[lr.first][lr.second][i]
              -hist[lr.first][ul.second-1][i]-hist[ul.first-1][lr.second][i];
    }else if(ul.first>0)
    {
      temp[i] = hist[lr.first][lr.second][i]-
                 hist[ul.first-1][lr.second][i];
    }else if(ul.second>0)
    {
      temp[i] = hist[lr.first][lr.second][i]
                -hist[lr.first][ul.second-1][i];
    }else
    {
      temp[i] =  hist[lr.first][lr.second][i];
    }

  }

  return temp;
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0;

/* your code here */

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 )
            entropy += ((double) distn[i]/(double) area)
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1* entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

/* your code here */
     int area = rectArea(ul,lr);
     vector<int> distn = buildHist(ul,lr);
     return entropy(distn, area);
}
