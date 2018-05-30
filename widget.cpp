#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include "bess_accel.cpp"
#include "bess_matlab.cpp"
#include "gen_data.cpp"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    matlab_loc = "/usr/local/MATLAB/R2017b/bin/matlab";
    compute_cnt = 100;
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_accel_clicked()
{
    QString *accel_result;
    accel_result = get_accel_result(this->data_loc, this->compute_cnt);
    ui->label_accel_time->setText(accel_result[0]);
    ui->label_accel_counts->setText(accel_result[1]);
    ui->label_accel_once->setText(accel_result[2]);
}

void Widget::on_pushButton_matlab_clicked()
{
    QMessageBox::information(this,tr("Matlab Bessel 算法"), tr("正在调用Matlab运行Bessel算法，请稍等..."));
    QString *matlab_result = get_matlab_result(this->matlab_loc, this->matlab_script, this->data_loc, this->compute_cnt);
    QMessageBox::information(this,tr("Matlab Bessel 算法"), tr("运行完毕，请检查结果！"));
    ui->label_matlab_time->setText(matlab_result[0]);
    ui->label_matlab_counts->setText(matlab_result[1]);
    ui->label_matlab_once->setText(matlab_result[2]);
}

void Widget::on_pushButton_matlab_loc_clicked()
{
    this->matlab_loc = QFileDialog::getOpenFileName(this,tr("选择Matlab程序"),"/usr/local/MATLAB/R2017b/bin/","");
    qDebug()<<this->matlab_loc;
}

void Widget::on_pushButton_matlab_script_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this,tr("选择Matlab算法脚本"),"/home/zwp/Test/bes_test/",tr("Matlab(*.m)"));
    this->matlab_script = tmp;
    qDebug()<<this->matlab_script;
}

void Widget::on_pushButton_report_clicked()
{
    float accel_time = ui->label_accel_once->text().toFloat();
    float matlab_time = ui->label_matlab_once->text().toFloat();
    float ratio = matlab_time/accel_time;
    ui->label_result_ratio->setText(QString::number(ratio));
    ui->label_result_1->setText(tr("1e-6"));
    ui->label_result_2->setText(tr("1e-4"));
}

void Widget::on_comboBox_currentTextChanged(const QString &arg1)
{
    this->compute_cnt = arg1.toInt();
    qDebug()<<arg1;
}
void Widget::on_pushButton_data_loc_clicked()
{
    this->data_loc = QFileDialog::getOpenFileName(this,tr("选择data数据"),"/home/zwp/Test/bes_test/",tr("Data(*.txt)"));
    qDebug()<<this->data_loc;
}

void Widget::on_pushButton_clicked()
{
    gen_data(this->data_loc,this->compute_cnt);
}
