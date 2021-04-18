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
