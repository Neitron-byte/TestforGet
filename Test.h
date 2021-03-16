#ifndef TEST_H
#define TEST_H
#include <QtSerialPort/QSerialPort>
#include <iostream>
#include <QObject>
#include <QByteArray>
#include <iostream>
#include <random>
#include <ctime>
#include <QDebug>

class Test : public QObject
{

    Q_OBJECT

    QSerialPort* serial1 = nullptr;
    QSerialPort* serial2 = nullptr;
signals:
    void Write();

public:
    explicit Test(QObject *parent = nullptr){
        qDebug()<<"Constr";

        serial1 = new QSerialPort(this);
        serial1->setPortName("COM1");
        serial1->setBaudRate(QSerialPort::Baud9600);
        if (serial1->open(QIODevice::ReadWrite)){
            std::cout<<"Serial 1 is open"<< std::endl;
        }

        connect(serial1,SIGNAL(readyRead()),this,SLOT(Readserial1()));

        serial2 = new QSerialPort(this);
        serial2->setPortName("COM2");
        serial2->setBaudRate(QSerialPort::Baud9600);
        if (serial2->open(QIODevice::ReadWrite)){
            std::cout<<"Serial 2 is open"<< std::endl;
        }

        connect(serial2,SIGNAL(readyRead()),this,SLOT(Readserial2()));

        connect(this,SIGNAL(Write()),this,SLOT(WriteSerial2()));
    }
    ~Test(){
        qDebug()<<"Distr";
        if(serial1){
            delete serial1;
        }
        if(serial2){
            delete serial2;
        }
    }

 public slots:
    void Readserial1(){


        QByteArray arr1 = serial1->readAll();
        std::cout << "Send for calibrator " << arr1.toStdString();



    }
    void Readserial2(){

        QByteArray arr = serial2->readAll();
        std::cout << "Send command for voltmeter " << arr.toStdString()<<std::endl;
        if(arr.toStdString() == "READ?\r\n"){
            emit Write();
        }

    }


    void WriteSerial2(){
        //float L = 0;
        //float HI = 5;
            srand(time(0));
            //float val = 1.12152; //= L + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-L)));

            int rnd = rand()%31;
            QString str = QString::number(rnd);
            QByteArray data = str.toLocal8Bit(); //(reinterpret_cast<const char*>(&val), sizeof(val));

            std::cout << "ANSWER " << data.toStdString() << std::endl;
            serial2->write(data);
    }
 };
#endif // TEST_H
