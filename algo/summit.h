#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

//找出极大值点的下标
vector<int> findLocalMaxima(const vector<double>& sequence) 
{
    vector<int> localMaxima;

    // 边界情况处理
    if (sequence.size() == 1) {
        localMaxima.push_back(0);
        return localMaxima;
    }

    for (int i = 1; i < sequence.size() - 1; ++i) {
        if (sequence[i] > sequence[i - 1] && sequence[i] > sequence[i + 1]) {
            // 当前元素大于其前后元素，则为极大值点
            localMaxima.push_back(i);
        }
    }

    // 处理序列的第一个元素
    if (sequence[0] > sequence[1]) {
        localMaxima.push_back(0);
    }

    // 处理序列的最后一个元素
    if (sequence[sequence.size() - 1] > sequence[sequence.size() - 2]) {
        localMaxima.push_back(sequence.size() - 1);
    }

    return localMaxima;
}

//AMPD算法寻找极值点
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

// 贪心，找所有极值点
// 计算极值点的左右价值，利用benefit / distance
// f(leftValue, rightValue)计算极值点value，记录价值最大峰
// 动态规划调度，求最大峰

//区间价值结构
struct IntervalValue
{
    vector<double> sequence;
    IntervalValue(vector<double> s) { this->sequence = s; }
    double getOneSideValue(int imax, int imin,int len, double k=1) 
    { 
        int interLen = abs(imax - imin);
        double heightBenefit = sequence[imax] - sequence[imin];//高度差收益
        double lenghtDeficit = interLen * k/( len);//长度差减损
        return heightBenefit - lenghtDeficit;
    }
};

struct peak
{
    //左侧单边价值,右侧单边价值
    double leftSideValue = 0, rightSideValue = 0,totalValue = 0;
    int imax, ilmin=0, irmin;
    int leftCnt=0, rightCnt=0;
    int pre;//在该区间之开始前结束的最近区间
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
        //收益整合函数f定义为f(x,y)=xy;
        return leftSideValue + rightSideValue;
    }
};

bool earlyEnd(peak a, peak b)
{
    return a.irmin < b.irmin;//结束时间从小到大
}

bool moreValue(peak a, peak b)
{
    return a.totalValue > b.totalValue;//价值从大到小
}
int latestNonConflictPeak(const vector<peak>& intervals, int currentInterval) {
    for (int i = currentInterval - 1; i >= 0; i--) {
        if (intervals[i].irmin <= intervals[currentInterval].ilmin)
            return i;
    }
    return -1;
}

//排好序的序列中找到小于目标值的最后一个值的下标
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

//变换至-1至1间
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

    //求出最大峰，最小峰
    vector<int> maxima = findLocalMaxima(sequence);
    vector<double> rsequence;
    for (int i = 0; i < sequence.size(); i++)
        rsequence.push_back(-sequence[i]);
    vector<int> minima = findLocalMaxima(rsequence);
    int totalLen = sequence.size();

    //对原序列进行变换变换至-1到1
    vector<double> seq(sequence);
    transformToRange(seq);

    //计算每个峰的价值
    IntervalValue calculateValue(seq);
    vector<peak> peaks;//存放所有封装好的峰
    for (auto imax : maxima)//对于平凡算法的每个峰值点
    {
        peak p;
        p.imax = imax;//峰值点下标为imax
        p.irmin = totalLen-1;
        for(auto imin:minima)//对于平凡算法的每个谷点
        {
            // sin 1
            double value = calculateValue.getOneSideValue(imax, imin, totalLen,12);
            if (imin < imax)//左侧
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
        //计算区间总价值
        p.totalValue = p.getIntervalValue();
        peaks.push_back(p);
    }

    //整个序列下标从0开始一直到len-1，进行一个区间调度
    
    //先将peak按结束时间从小到大排列
    sort(peaks.begin(), peaks.end(), earlyEnd);

    int len = peaks.size();
    vector<double> dp(len); // dp[i]表示以第i个区间结尾的最大权重和
    vector<int> prev(len); // prev[i]表示选择第i个区间时的前一个区间的索引

    dp[0] = peaks[0].totalValue;
    prev[0] = -1;
    // 动态规划求解
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

    // 回溯得到选择的峰
    int cntPeak = 0;
    vector<peak> selectedPeaks;
    int idx = len - 1;
    while (idx >= 0) 
    {
        if (prev[idx] == -1 || dp[idx] != dp[idx - 1]) //说明选择了该峰
        {
            selectedPeaks.push_back(peaks[idx]);
            cntPeak++;
        }

        idx = prev[idx];
    }

    // 按照价值大小排序
    sort(selectedPeaks.begin(), selectedPeaks.end(), moreValue);
    vector<int>selectedIndex;
    // && i <5
    for (int i=0;i<selectedPeaks.size();i++)
 //       if(selectedPeaks[i].totalValue>0.01 * allValue)
            selectedIndex.push_back(selectedPeaks[i].imax);
    
    return selectedIndex;

}