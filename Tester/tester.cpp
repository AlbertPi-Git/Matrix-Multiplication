#include<vector>
#include<iostream>
#include<chrono>
#include<random>
#include "../MatMul.h"

#define TYPE long

int main(){
    int m1_r=1213,m1_c=1453,m2_c=940;
    std::vector<std::vector<TYPE>> m1(m1_r,std::vector<TYPE> (m1_c,1));
    std::vector<std::vector<TYPE>> m2(m1_c,std::vector<TYPE> (m2_c,2));
    std::vector<std::vector<TYPE>> res(m1_r,std::vector<TYPE> (m2_c,0));

    std::random_device dev;
    std::default_random_engine Rndengine(dev());
    std::uniform_real_distribution<double> uniform_dist(-10000,10000);

    //Randomly assign values to each matrix element
    for(int i=0;i<m1.size();++i){
        for(int j=0;j<m1[0].size();++j){
            m1[i][j]=uniform_dist(Rndengine);
        }
    }
    for(int i=0;i<m2.size();++i){
        for(int j=0;j<m2[0].size();++j){
            m2[i][j]=uniform_dist(Rndengine);
        }
    }

    //Check how many time will this matrix multiplication spend
    auto startTime=std::chrono::high_resolution_clock::now();
    MatrixMul<TYPE> (m1,m2,res);
    auto endTime=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds> (endTime-startTime);
    std::cerr<<"Time elapsed: "<<duration.count()<<" ms"<<std::endl;

    PrintMatrix<TYPE> (m1);
    std::cout<<'\n';
    PrintMatrix<TYPE> (m2);
    std::cout<<'\n';
    PrintMatrix<TYPE> (res);
}
