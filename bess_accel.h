#ifndef BESS_ACCEL_H
#define BESS_ACCEL_H

#include <QString>
#include <fstream>
#include <iomanip>
#include "bess.h"

QString* get_accel_result(QString data_file,int compute_cnt){
    QString *result = new QString[3];
    double *rst = new double[compute_cnt];

//    int cnt=0;
    const int k=compute_cnt;
    int start, end;
    double Time=0,onceTime;
    QByteArray tmp = data_file.toUtf8();
    const char *file_name = tmp.data();
    int p1 = data_file.lastIndexOf('/');
    QString dir_loc = data_file.left(p1);
    FILE *f;
    double a,b,t,l,s,r,m,y;
    const double eps=1e-12;
    int cnt=0;
    f = fopen(file_name,"r+");
    while(cnt<k && fscanf(f,"%lf%lf%lf%lf",&a,&b,&t,&l)==4){
        start = clock();
        y=l*exp(a*b*t/4); //避免多次重复计算
        s=0.001; r=100;
        s=a*s; r=a*r; // 将a整合进入 ，以免多次计算乘积
        while(r-s>eps){
            m=(s+r)/2;
            if(BESSI0(m)>y) r=m;
            else s=m;
        }
        if(b_check(a*s,y)>-0.01&&b_check(a*s,y)<0.01) qDebug()<<"yes";
        rst[cnt++] = s/a;
        end = clock();
        Time += (end - start);
    }
    Time=(Time+0.0)/(CLOCKS_PER_SEC);
    onceTime = Time/k;
    result[0] = QString::number(Time);
    result[1] = QString::number(k);
    result[2] = QString::number(onceTime);
    std::ofstream outFile((dir_loc+"/accel_result.txt").toUtf8().data());
    for (int i=0;i<k;++i){
        outFile<<std::setprecision(10)<<rst[i]<<std::endl;
    }
    return result;
}

#endif // BESS_ACCEL_H
