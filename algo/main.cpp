#include <iostream>
#include <vector>
#include<fstream>
#include <sstream>
#include "summit.h"

using namespace std;
int main() 
{

    // 示例输入序列
    vector<double> sequence;
    ifstream file("D:\\VSprojects\\summit\\summit\\data.txt"); // 文件路径和名称

    cout << "Opening file..."<<endl;

    //读入数据
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
        cout << "无法打开文件." << endl;
    
    // 寻找极大值点
    cout << "calculating..." << endl;
    vector<int> localMaxima = DPDIV(sequence);

    // 输出结果
    string filepath = "D:\\VSprojects\\summit\\summit\\output.txt";
    ofstream outputFile(filepath); // 打开输出文件
    if (outputFile.is_open()) {
        outputFile << localMaxima[0];
        for (int i = 1; i < localMaxima.size(); i++) {
            outputFile << "," << localMaxima[i];
        }

        outputFile.close(); // 关闭输出文件
        cout << "Output has been written to " << filepath << std::endl;
    }
    else 
        cout << "Failed to open output file." << std::endl;
    

    return 0;
}
