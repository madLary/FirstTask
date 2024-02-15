#include "main.h"

float math_func(float num, char index, QList<float> buffer);
void processing(QVector<float> &data, QString math_Functions, QVector<int> &lineStatus, int input_type);
bool intVerification(QString x);
bool intVerification(float x);
bool isNumVerification(QString x);
void writeFile(QVector<float> data, QFile &fileOut, QVector<int> lineStatus, int input_type);
void outDataConverter(int output_type, QVector<float> &data, QVector<int> &lineStatus);
void fileRead(int input_type, QVector<float> &data, QVector<int> &lineStatus, QFile &fileInput);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSettings config("./Config.ini", QSettings::Format::IniFormat);

    //создание .ini файла
//    config.setValue("input_type", 0);
//    config.setValue("math_Functions", 0123);
//    config.setValue("output_type", 1);

//    qDebug() << config.value("input_type", 0).toInt();
//    qDebug() << config.value("math_Functions", 0123).toString();//обратить внимание как удобнее забирать значение
//    qDebug() << config.value("output_type", 1).toInt();

    int input_type = config.value("input_type", 0).toInt();
    QString math_Functions = config.value("math_Functions", 0123).toString();//попробовать переделать в QStringList и забирать значение через пробелы для удобства
    int output_type = config.value("output_type", 1).toInt();
    QVector<float> data;
    QVector<int> lineStatus;

    if(input_type !=0 && input_type != 1)
    {
        qDebug() << "Unknown input type.";
        return a.exec();
    }
    if(output_type !=0 && output_type != 1)
    {
        qDebug() << "Unknown output type.";
        return a.exec();
    }

    qDebug() << "Settings:";
    qDebug() << input_type;
    qDebug() << math_Functions;
    qDebug() << output_type;
    qDebug() << "";
    qDebug() << "Program:";


    //QFile fileInput("/home/ilya/Qt/TaskOneConsole/Data.txt");
    QFile fileInput("./Data.txt");
    if(!fileInput.open(QIODevice::ReadOnly))
    {
        qDebug() <<"Error open file";
        return 0;
    }
    qDebug() << "Size of input file: "<< fileInput.size();


    QFile fileOut("./fileOut.txt");
    if(!fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"Error write file";
    }

    //1 - считывание
    fileRead(input_type, data, lineStatus, fileInput);

    //2 - обработка
    processing(data, math_Functions, lineStatus, input_type);
    outDataConverter(output_type, data, lineStatus);
    //qDebug() << "Output vector: " << data;

    //3 - запись вектора в файл
    writeFile(data, fileOut, lineStatus, input_type);



    fileInput.close();
    fileOut.close();
    return 0;
    //return a.exec();
};

void fileRead(int input_type, QVector<float> &data, QVector<int> &lineStatus, QFile &fileInput)
{
    if(input_type == 0)
    {
        qDebug() << "Input data is int";
        qDebug() << "";

        // запись числа из файла и проверка
        while(!fileInput.atEnd())
        {
            QString num = fileInput.readLine();
            qDebug() << num;
            qDebug() << isNumVerification(num);
            qDebug() << intVerification(num);

            //запоминает статусы строк. 0 - все хорошо, 1 - вводные данные не являются числом или слишком длинные, 2 - не были целым числом
            if (!isNumVerification(num))
                lineStatus.push_back(1);
            else if (!intVerification(num))
                lineStatus.push_back(2);
            else
                lineStatus.push_back(0);

            data.push_back(num.toInt());

        }
    }
        else
        {
            qDebug() << "Input data is float";
            qDebug() << "";

            // запись запись числа из файла и проверка
            while(!fileInput.atEnd())
            {
                QString num = fileInput.readLine();
                qDebug() << num;
                qDebug() << isNumVerification(num);

                //запоминает статусы строк. 0 - все хорошо, 1 - вводные данные не были числом
                if (!isNumVerification(num))
                    lineStatus.push_back(1);
                else
                    lineStatus.push_back(0);

                data.push_back(num.toFloat());
            }
        }
        qDebug() <<"";
}

void outDataConverter(int output_type, QVector<float> &data, QVector<int> &lineStatus)
{
    if(output_type == 0)
    {
        qDebug() << "Output data is int";
        for(int i = 0; i < data.size(); ++i)
        {
            if(!intVerification(data[i]) && lineStatus[i] == 0)
            {
                //qDebug() << intVerification(data[i]);
                // статус строки 3 - предупреждает что число было сильно преобразовано перед выводом
                lineStatus[i] = 3;

            }
            data[i]=(int)data[i];
        }
    }
    else
        qDebug() << "Output data is float";
};

void writeFile(QVector<float> data, QFile &fileOut, QVector<int> lineStatus, int input_type)
{
    QTextStream writeStream(&fileOut);
    writeStream.setRealNumberPrecision(14);
    qDebug() << "Output vector: ";

    for(int i=0; i < data.size(); ++i)
    {
        writeStream << data[i];
        cout << to_string(data[i]) << endl;



//        if(lineStatus[i] == 1)
//        {
//            writeStream << "ERROR code:1 - input data is not a number or too long";
//        }
//        else if (lineStatus[i] == 2 && input_type == 0)
//        {
//            writeStream << "ERROR code:2 - input data is not an int";
//        }
//        else if (lineStatus[i] == 3)
//        {
//            writeStream << data[i];
//            writeStream << " WARN! The output data type has been changed by more than 0.03 while converting from float to int";
//        }
//        else
//            writeStream << data[i];


        //        for(size_t i = 0; i < sizeof data[i]; ++i)
        //        {
        //            cout << ' ' << bitset<8>(reinterpret_cast<unsigned char *>(&data[i])[i]);
        //        }

        writeStream << "\n";
    }





};

bool isNumVerification(QString x)
{
    bool status;
    x.toFloat(&status);
    return status;
};

bool intVerification(float x)
{

    int intX = (int)x;
//    qDebug() << intX;
//    qDebug() << abs(x - intX);
    if(0.03 >= abs(x - intX) )
        return true;
    else
        return false;
};


bool intVerification(QString x)
{
    int intX = x.toInt();
    float floatX = x.toFloat();
    if(intX - floatX == 0)
        return true;
    else
        return false;
};

void processing(QVector<float> &data, QString math_Functions, QVector<int> &lineStatus, int input_type)
{
    QList<float> buffer;
    for(int i = 0; i < data.size(); ++i)
    {
        qDebug() << "Line #" << i;
        if(lineStatus[i]==1)
        {
            qDebug() << "ERROR: Input data is not a number, or too long";
            qDebug() << "";
            continue;
        }
        else if((lineStatus[i]==2) && (input_type == 0))
        {
            qDebug() << "ERROR: Input data is not an int";
            qDebug() << "";
            continue;
        }

        qDebug() << "Line is OK";
        qDebug() << "Input number:" << data[i];
        for(int j = 0; j < math_Functions.size(); ++j)
        {
            char index = QString(math_Functions).toStdString()[j];
            qDebug() << "Math function index:" << index;
            data[i] = math_func(data[i], index, buffer);
            qDebug() << "After:" <<data[i];
            buffer << data[i];
            if (buffer.size() > 10)
                buffer.removeFirst();
            //qDebug() << buffer;
        }
        qDebug() <<"";
    }
};

float math_func(float num, char index, QList<float> buffer)
{
    int x=2; //здесь тоже нужна проверка, не забыть

    switch (index)
    {
    case '0':
        //cin >> x;
        return num*x;
        break;

    case '1':
        //cin >> x;
        if(num>0)
        {
            return log(num)/log(x);
        }
        else
        {
            qDebug() << "Not a positive number. Log was not calculated";
            return num;
        }
        break;

    case '2':
        //cin >> x;
        return pow(num, x);
        break;

    case '3':
        if(buffer.size() < 10)
        {
            qDebug() << "There are not 10 counts. Avg was not calculated";
            return num;
        }
        float sum = 0;
        for(int i = 0; i < 10; ++i)
        {
            sum += buffer[i];
        }
        return sum/10;
        break;
    }
    qDebug() << "Unknown math fuction index";
    return num;
};
