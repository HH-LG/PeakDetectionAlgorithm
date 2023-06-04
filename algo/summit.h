#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

//�ҳ�����ֵ����±�
vector<int> findLocalMaxima(const vector<double>& sequence) 
{
    vector<int> localMaxima;

    // �߽��������
    if (sequence.size() == 1) {
        localMaxima.push_back(0);
        return localMaxima;
    }

    for (int i = 1; i < sequence.size() - 1; ++i) {
        if (sequence[i] > sequence[i - 1] && sequence[i] > sequence[i + 1]) {
            // ��ǰԪ�ش�����ǰ��Ԫ�أ���Ϊ����ֵ��
            localMaxima.push_back(i);
        }
    }

    // �������еĵ�һ��Ԫ��
    if (sequence[0] > sequence[1]) {
        localMaxima.push_back(0);
    }

    // �������е����һ��Ԫ��
    if (sequence[sequence.size() - 1] > sequence[sequence.size() - 2]) {
        localMaxima.push_back(sequence.size() - 1);
    }

    return localMaxima;
}

//AMPD�㷨Ѱ�Ҽ�ֵ��
vector<int> AMPD(const vector<double>& sequence) 
{
    vector<int> p_data(sequence.size(), 0);
    int count = sequence.size();
    vector<int> arr_rowsum;

    for (int k = 1; k <= count / 2; ++k)
    {
        int row_sum = 0;
        for (int i = k; i < count - k; ++i)
        {
            if (sequence[i] > sequence[i - k] && sequence[i] > sequence[i + k]) 
                row_sum += 1;
            
        }
        arr_rowsum.push_back(row_sum);
    }

    int max_index = max_element(arr_rowsum.begin(), arr_rowsum.end()) - arr_rowsum.begin();
    int max_window_length = max_index;

    for (int k = 1; k <= max_window_length; ++k) 
        for (int i = k; i < count - k; ++i) 
            if (sequence[i] > sequence[i - k] && sequence[i] > sequence[i + k]) 
                p_data[i] += 1;
        
    

    vector<int> result;
    for (int i = 0; i < count; ++i) 
        if (p_data[i] == max_window_length) 
            result.push_back(i);

    return result;
}

// ̰�ģ������м�ֵ��
// ���㼫ֵ������Ҽ�ֵ������benefit / distance
// f(leftValue, rightValue)���㼫ֵ��value����¼��ֵ����
// ��̬�滮���ȣ�������

//�����ֵ�ṹ
struct IntervalValue
{
    vector<double> sequence;
    IntervalValue(vector<double> s) { this->sequence = s; }
    double getOneSideValue(int imax, int imin,int len, double k=1) 
    { 
        int interLen = abs(imax - imin);
        double heightBenefit = sequence[imax] - sequence[imin];//�߶Ȳ�����
        double lenghtDeficit = interLen * k/( len);//���Ȳ����
        return heightBenefit - lenghtDeficit;
    }
};

struct peak
{
    //��൥�߼�ֵ,�Ҳ൥�߼�ֵ
    double leftSideValue = 0, rightSideValue = 0,totalValue = 0;
    int imax, ilmin=0, irmin;
    int leftCnt=0, rightCnt=0;
    int pre;//�ڸ�����֮��ʼǰ�������������
    void setLeft(double v,int lmin)
    {
        ilmin = lmin;
        leftSideValue = v;
    }
    void setRight(double v,int rmin)
    {
        irmin = rmin;
        rightSideValue = v;
    }
    double getIntervalValue()
    {
        //�������Ϻ���f����Ϊf(x,y)=xy;
        return leftSideValue + rightSideValue;
    }
};

bool earlyEnd(peak a, peak b)
{
    return a.irmin < b.irmin;//����ʱ���С����
}

bool moreValue(peak a, peak b)
{
    return a.totalValue > b.totalValue;//��ֵ�Ӵ�С
}
int latestNonConflictPeak(const vector<peak>& intervals, int currentInterval) {
    for (int i = currentInterval - 1; i >= 0; i--) {
        if (intervals[i].irmin <= intervals[currentInterval].ilmin)
            return i;
    }
    return -1;
}

//�ź�����������ҵ�С��Ŀ��ֵ�����һ��ֵ���±�
int findLastSmallerIndex(vector<int>& sortedArray, int target) {
    int left = 0;
    int right = sortedArray.size() - 1;
    int lastSmallerIndex = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (sortedArray[mid] < target) {
            lastSmallerIndex = mid;
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return lastSmallerIndex;
}

//�任��-1��1��
void transformToRange(vector<double>& sequence) {
    double minVal = *min_element(sequence.begin(), sequence.end());
    double maxVal = *max_element(sequence.begin(), sequence.end());
    double range = maxVal - minVal;

    for (double& value : sequence) 
        value = -1.0 + 2.0 * (value - minVal) / range;

}
//Dynamic Peak Detection Algorithm with IntervalValue 
vector<int> DPDIV(const vector<double>& sequence)
{

    //������壬��С��
    vector<int> maxima = findLocalMaxima(sequence);
    vector<double> rsequence;
    for (int i = 0; i < sequence.size(); i++)
        rsequence.push_back(-sequence[i]);
    vector<int> minima = findLocalMaxima(rsequence);
    int totalLen = sequence.size();

    //��ԭ���н��б任�任��-1��1
    vector<double> seq(sequence);
    transformToRange(seq);

    //����ÿ����ļ�ֵ
    IntervalValue calculateValue(seq);
    vector<peak> peaks;//������з�װ�õķ�
    for (auto imax : maxima)//����ƽ���㷨��ÿ����ֵ��
    {
        peak p;
        p.imax = imax;//��ֵ���±�Ϊimax
        p.irmin = totalLen-1;
        for(auto imin:minima)//����ƽ���㷨��ÿ���ȵ�
        {
            // sin 1
            double value = calculateValue.getOneSideValue(imax, imin, totalLen,12);
            if (imin < imax)//���
            {
                if (value > p.leftSideValue)
                    p.setLeft(value, imin);
            }
            else
            {
                if (value > p.rightSideValue)
                    p.setRight(value, imin);
            }
        }
        //���������ܼ�ֵ
        p.totalValue = p.getIntervalValue();
        peaks.push_back(p);
    }

    //���������±��0��ʼһֱ��len-1������һ���������
    
    //�Ƚ�peak������ʱ���С��������
    sort(peaks.begin(), peaks.end(), earlyEnd);

    int len = peaks.size();
    vector<double> dp(len); // dp[i]��ʾ�Ե�i�������β�����Ȩ�غ�
    vector<int> prev(len); // prev[i]��ʾѡ���i������ʱ��ǰһ�����������

    dp[0] = peaks[0].totalValue;
    prev[0] = -1;
    // ��̬�滮���
    for (int i = 1; i < len; i++) 
    {
        double includeWeight = peaks[i].totalValue;
        int pre = latestNonConflictPeak(peaks, i);
        if (pre != -1)
            includeWeight += dp[pre];
        if (includeWeight > dp[i - 1])
        {
            dp[i] = includeWeight;
            prev[i] = pre;
        }
        else
        {
            dp[i] = dp[i - 1];
            prev[i] = i - 1;
        }
    }
    double allValue = dp[len - 1];

    // ���ݵõ�ѡ��ķ�
    int cntPeak = 0;
    vector<peak> selectedPeaks;
    int idx = len - 1;
    while (idx >= 0) 
    {
        if (prev[idx] == -1 || dp[idx] != dp[idx - 1]) //˵��ѡ���˸÷�
        {
            selectedPeaks.push_back(peaks[idx]);
            cntPeak++;
        }

        idx = prev[idx];
    }

    // ���ռ�ֵ��С����
    sort(selectedPeaks.begin(), selectedPeaks.end(), moreValue);
    vector<int>selectedIndex;
    // && i <5
    for (int i=0;i<selectedPeaks.size();i++)
 //       if(selectedPeaks[i].totalValue>0.01 * allValue)
            selectedIndex.push_back(selectedPeaks[i].imax);
    
    return selectedIndex;

}