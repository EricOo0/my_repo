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

# 28. 实现 strStr()
	给你两个字符串 haystack 和 needle ，请你在 haystack 字符串中找出 needle 字符串出现的第一个位置（下标从 0 开始）。如果不存在，则返回  -1 。
	字符串匹配问题，最直观的思路是暴力法遍历字符串每一作为起点的位置，判断是否匹配
# C++实现
```
class Solution {
public:
    int strStr(string haystack, string needle) {
        if(needle==""){return 0;}
        int m=haystack.size();
        int n=needle.size();
        for(int i=0;i<(m-n+1);i++){
            int j=0;
            for(;j<n;j++){
                if(haystack[i+j]!=needle[j]){
                    break;
                }
            }
            
            if(j==n){
                return i;
            }
        }
        return -1;
    }
};
```
	可使用KMP算法进行优化，KMP算法是构建了一个前缀表，记录该元素的前缀中的最长真前缀和真后缀相等的长度len，当发生不匹配时，将指针移动到模式串的len出继续匹配
	这样子减少了不必要的重复匹配，提高效率
	难点是前缀表的建立。
	
	
# 437. 路径总和 III
	给定一个二叉树，它的每个结点都存放着一个整数值。
	找出路径和等于给定数值的路径总数。
	路径不需要从根节点开始，也不需要在叶子节点结束，但是路径方向必须是向下的（只能从父节点到子节点）
	
	要考虑包含root节点和不包含root节点两种情况，传的参分别为targetsum和targetsum-rootval，所以需要两个递归
```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
   
    int pathSum(TreeNode* root, int targetSum) {
       return root? dfs_withroot(root, targetSum) + pathSum(root->left, targetSum) + pathSum(root->right, targetSum): 0;
    }
    int dfs_withroot(TreeNode* root, int targetSum){
        if(root==nullptr){return 0;}
        int res=0;
        if(root->val==targetSum){res++;}
        res+= dfs_withroot(root->left,targetSum-root->val);
        res += dfs_withroot(root->right,targetSum-root->val);
        return res;
    }
};
```

# 91. 解码方法
	题目太长就不贴了
	
	主要思路就是动态规划，当当前字符不等于0时，他自己肯定可以解码出一个符号，所以必有dp[i]=dp[i-1]（dp为前i个字符组成的字符串能够解码的种类）
	当当前字符和前一字符组成<=26并且s[i-1]不等于0（即不是01，02，这样0开头的）那么这两个数也可以组成一种字符 即dp[i]=dp[i-2]
	结合两种情况，将两种情况加起来
	
# C++实现 
```
class Solution {
public:
    int numDecodings(string s) {
        if(s[0]=='0'){return 0;}
        vector<int> dp(s.size()+1,0);
        dp[1]=1;
        dp[0]=1;
        for(int i=1;i<s.size();i++){
            if(s[i]=='0'){
                if(s[i-1]=='1'||s[i-1]=='2'){
                    dp[i+1]=dp[i-1];
                }
            }
            else if(s[i]>='1'&&s[i]<='6'){
                if(s[i-1]=='1'||s[i-1]=='2'){
                    dp[i+1]=dp[i-1];
                }
                dp[i+1]+=dp[i];
            }
            else if(s[i]>'6'){
                if(s[i-1]=='1'){
                    dp[i+1]=dp[i-1];
                }
                dp[i+1]+=dp[i];
            }

        }

        return dp[s.size()];
    }
};
```