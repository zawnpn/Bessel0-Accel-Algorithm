#include "bess.h"
#include <QString>
#include <fstream>
double fun(double ax,double y){
   return BESSI0(ax)-y;
}
QString* get_accel_result(QString data_file,int compute_cnt){
    QString *result = new QString[3];
    double *rst = new double[compute_cnt];
    double a,b,t,l,s,r,m,y;
    const double eps=1e-6;
    int cnt=0;
    int k=compute_cnt;
    int start, end;
    double Time=0,onceTime;
    QByteArray tmp = data_file.toUtf8();
    const char *file_name = tmp.data();
    FILE *f;
    start = clock();
    f = fopen(file_name,"r+");
    while(cnt<k && fscanf(f,"%lf%lf%lf%lf",&a,&b,&t,&l)==4){

        y=l*exp(a*b*t/4); //避免多次重复计算
        s=0.001; r=100;
        s=a*s; r=a*r; // 将a整合进入 ，以免多次计算乘积
        while(r-s>eps){
            m=(s+r)/2;
            if(BESSI0(m)>y) r=m;
            else s=m;
        }
        if(fun(a*s,y)>-0.01&&fun(a*s,y)<0.01) qDebug()<<"yes";
        rst[cnt++] = s;
    }
    end = clock();
    Time = (end - start);
    Time=(Time+0.0)/(CLOCKS_PER_SEC);
    onceTime = Time/cnt;
    result[0] = QString::number(Time);
    result[1] = QString::number(cnt);
    result[2] = QString::number(onceTime);
    std::ofstream outFile("/home/zwp/Test/bes_test/accel_result.txt");
    for (int i=0;i<k;++i){
        outFile<<rst[i]<<std::endl;
    }
    return result;
}
