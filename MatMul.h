#include<vector>
#include<thread>
#include<limits>
#include<iomanip>

unsigned maxThreadsSupported = std::thread::hardware_concurrency();
#define PER_THREAD_THRESHOLD 100

template<class T>
void PrintMatrix(std::vector<std::vector<T>> m){
    for(int i=0;i<m.size();i++){
        for(int j=0;j<m[0].size();j++)
            std::cout<<std::setprecision(std::numeric_limits<double>::digits10+5)<<m[i][j]<<" ";
        std::cout<<std::endl;
    }
}

template<class T>
void MatrixTranspose(std::vector<T> &m, std::vector<T> &res){
    //Check whether inputs are valid
    if(m.empty()||m[0].empty())
        throw std::invalid_argument("Matrix has empty row or empty column!\n");
    if(res.empty()||res[0].empty())
        throw std::invalid_argument("Result container has empty row or empty column!\n");
    if(res.size()!=m[0].size()||res[0].size()!=m.size())
        throw std::invalid_argument("Size of result container doesn't match the expect size!\n");
    
    //Straightforward exchange
    for(int i=0;i<m.size();++i){
        for(int j=0;j<m[0].size();++j){
            res[j][i]=m[i][j];
        }
    }
}

//Function that computed by each thread 
template<class T>
static void multiThreadingMul(std::vector<std::vector<T>> &m1, std::vector<std::vector<T>> &m2_T, std::vector<std::vector<T>>&res,int threadID){
    int m1_r=m1.size();
    int m1_c=m1[0].size();
    int m2_c=m2_T.size();
    int m2_r=m2_T[0].size();
    int res_r=res.size();
    int res_c=res[0].size();

    int elePerThread=(m1_r*m2_c)/maxThreadsSupported;       //How many elements in the result will each thread computes
    int eleRemainder=(m1_r*m2_c)%maxThreadsSupported;       //If there is some elements left, assign them to thread 0

    //Use thread ID to determine the range that each thread will compute
    int startPos, endPos;
    if(threadID==0){
        startPos=0;                             //Inclusive
        endPos=elePerThread+eleRemainder;       //Exclusive
    }else{
        startPos=threadID*elePerThread+eleRemainder;
        endPos=(threadID+1)*elePerThread+eleRemainder;
    }

    for(int pos=startPos;pos<endPos;++pos){
        //Convert flat range into 2 dimension i,j indices
        int i=pos/res[0].size();
        int j=pos%res[0].size();
        for(int k=0;k<m2_r;++k)
            res[i][j]+=m1[i][k]*m2_T[j][k];
    }
}


template<class T>
void MatrixMul(std::vector<std::vector<T>> &m1, std::vector<std::vector<T>> &m2, std::vector<std::vector<T>>&res){
    //Check whether inputs are valid
    if(m1.empty()||m1[0].empty())
        throw std::invalid_argument("Matrix 1 has empty row or empty column!\n");
    if(m2.empty()||m2[0].empty())
        throw std::invalid_argument("Matrix 2 has empty row or empty column!\n");
    if(res.empty()||res[0].empty())
        throw std::invalid_argument("Result container has empty row or empty column!\n");

    //Transpose m2 to improve the cache performance
    std::vector<std::vector<T>> m2_T(m2[0].size(),std::vector<T> (m2.size(),0));
    MatrixTranspose(m2,m2_T);

    int m1_r=m1.size();
    int m1_c=m1[0].size();
    int m2_r=m2.size();
    int m2_c=m2[0].size();
    int res_r=res.size();
    int res_c=res[0].size();

    if(m1_c!=m2_r){
        throw std::invalid_argument("Size of two matrices don't match!\n");
    }else if(res_r!=m1_r||res_c!=m2_c){
        throw std::invalid_argument("Size of result container doesn't match the expect size!\n");
    }

    //If failed to detect multiple HW threads or size of matrix is small, then only use one thread to avoid the overhead of multithreading
    if(maxThreadsSupported==0 || m1_r*m1_c*m2_c<maxThreadsSupported*int(PER_THREAD_THRESHOLD)){
        for(int i=0;i<m1_r;i++){
            for(int j=0;j<m2_c;j++){
                for(int k=0;k<m1_c;k++)
                    res[i][j]+=m1[i][k]*m2_T[j][k];
            }
        }
    }else{      //Else use multi threads to compute in parallel
        std::thread* threadPool=new std::thread[maxThreadsSupported];
        for(int i=0;i<maxThreadsSupported;++i){     //Assign task to each thread
            threadPool[i]=std::thread(multiThreadingMul<T>,std::ref(m1),std::ref(m2_T),std::ref(res),i);
        }
        for(int i=0;i<maxThreadsSupported;++i){     //Wait for all threads to complete
            threadPool[i].join();
        }
        delete[] threadPool;       
    }
}