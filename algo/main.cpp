#include <iostream>
#include <vector>
#include<fstream>
#include <sstream>
#include "summit.h"

using namespace std;
int main() 
{

    // ʾ����������
    vector<double> sequence;
    ifstream file("D:\\VSprojects\\summit\\summit\\data.txt"); // �ļ�·��������

    cout << "Opening file..."<<endl;

    //��������
    if (file.is_open()) 
    {
        string line;
        while (getline(file, line, ',')) 
        {
            double value;
            istringstream iss(line);
            if (iss >> value) 
                sequence.push_back(value);
        }
        file.close();
    }
    else 
        cout << "�޷����ļ�." << endl;
    
    // Ѱ�Ҽ���ֵ��
    cout << "calculating..." << endl;
    vector<int> localMaxima = DPDIV(sequence);

    // ������
    string filepath = "D:\\VSprojects\\summit\\summit\\output.txt";
    ofstream outputFile(filepath); // ������ļ�
    if (outputFile.is_open()) {
        outputFile << localMaxima[0];
        for (int i = 1; i < localMaxima.size(); i++) {
            outputFile << "," << localMaxima[i];
        }

        outputFile.close(); // �ر�����ļ�
        cout << "Output has been written to " << filepath << std::endl;
    }
    else 
        cout << "Failed to open output file." << std::endl;
    

    return 0;
}
