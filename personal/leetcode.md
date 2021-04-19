# 26. 删除有序数组中的重复项

## 给你一个有序数组 nums ，请你 原地 删除重复出现的元素，使每个元素 只出现一次 ，返回删除后数组的新长度。
## 不要使用额外的数组空间，你必须在 原地 修改输入数组 并在使用 O(1) 额外空间的条件下完成。

### 有序数组，如果可以使用额外数组，则可以使用unodered_map<int>记录出现次数，遍历一遍，将重复出现的于数组右测元素交换
### 题目要求原地修改，使用双指针，left和right  
    left左侧为已检验过的元素，right用于遍历和交换数组  
    由于是有序数组，所以重复元素是连续出现的（122234444566），当right指针和left指针元素相等，则一直向右移动右指针。当不相等，则进入了下一个区间，将左指针右移1，交换左右指针的值  
    即将重复的元素放到后面去，未重复的元素放到前面。（左右指针中间的数是重复的数）

## C++实现  
```
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {  
        //排序
       // sort(nums.begin(),nums.end());
       if(nums.empty())return {};
        int left=0,right=0;
        while(right<nums.size()){
            if(nums[right]!=nums[left]){
                left++;
                int tmp=nums[left];
                nums[left]=nums[right];
                nums[right]=tmp;
                
            }
            right++;
        }
        return left+1;
    }
};
```

# 309. 最佳买卖股票时机含冷冻期
## 股票买卖题一般都是动态规划，找到状态转移方程是关建  
    假设f是今天结束后的收益  
	f[i][0]是继续持有股票的收益
	f[i][1]是卖出股票的收益   
	状态转移方程则为
	f[i][0]=f[i-1][0]
	f[i][1]=f[i-1][0]+price[i]
	初始状态
	f[0][0]=-price[0]
	f[0][1]=0
	加了约束条件的话就会多增加几个状态最后返回几种状态中的最大
## C++实现
```
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        vector<vector<int>> f(prices.size(),vector<int>(3,0));
        f[0][0]=-prices[0];
        f[0][1]=0;
        f[0][2]=0;
        for(int i=1;i<prices.size();i++){
        //三种情况 持有 未持有-冷冻期  未持有-非冷冻期
            f[i][0]=max(f[i-1][0],f[i-1][2]-prices[i]);
            f[i][1]=f[i-1][0]+prices[i];
            f[i][2]=max(f[i-1][1],f[i-1][2]);
        }
        return max(max(f[prices.size()-1][0],f[prices.size()-1][1]),f[prices.size()-1][2]);
        
    }
};
```

# 27. 移除元素
## 给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素，并返回移除后数组的新长度。  
	不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并 原地 修改输入数组。
	
	原地修改数组，所以不可以用新的数组存储符合条件要求，可用双指针（和26思路一样），把符合要求的元素的放到left指针前面，right指针用于遍历数组
	

# C++实现
```
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int left=0;
        int right=0;
        while(right<nums.size()){
            if(nums[right]!=val){
                int tmp=nums[left];
                nums[left]=nums[right];
                nums[right]=tmp;
                left++;
            }
            right++;
        }
        return left;
    }
};
```
# 75. 颜色分类
	给定一个包含红色、白色和蓝色，一共 n 个元素的数组，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。
	此题中，我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。

	可以用单指针遍历两次，找0和2分别放到首部和尾部
	也可以用双指针遍历一次
	1、找0和1，用两个指针p0和p1来存0和1的位置，从左到右遍历数组，找到1则和p1交换，p1加1；找到0则和p0交换，但此时p0和p1都要加1（如果p1>p0，此时交换出来的nums[i]是1，还要再赋给p1才能给p1加1）
#C++实现
```
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int p0=0;
        int p1=0;
        for(int i=0;i<nums.size();i++){
            if(nums[i]==0){
                int tmp=nums[p0];
                nums[p0]=nums[i];
                nums[i]=tmp;
                if(nums[i]==1){
                    int tmp=nums[p1];
                    nums[p1]=nums[i];
                    nums[i]=tmp;
                    
                }
                p0++;
                p1++;
                
            }
            else if(nums[i]==1){
                int tmp=nums[p1];
                nums[p1]=nums[i];
                nums[i]=tmp;
                p1++;
            }
        }
    }
};
```
	2、找0和2，0往头部扔，2往尾部扔。同样从左往右遍历数组，但此时和p2交换出来的元素可能还是2，但下标已经到了i+1，所以需要循环交换，直到p2不为2为止
		注意要先用while判断2，这样剩下的nums[i]只能是0和1，就不需要用循环来交换
# C++
```
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int  left=0;
        int right=nums.size()-1;
        int i=0;
        while(i<=right){
            
            while(i<=right&&nums[i]==2){
                swap(nums[i],nums[right]);
                right--;
            }
                if(nums[i]==0){
                swap(nums[i],nums[left]);
                left++;
            }
            i++;
            }
        }
    
};
```