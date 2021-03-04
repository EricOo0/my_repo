import xlrd
import csv
import pandas as pd
#打开文件
work_book = xlrd.open_workbook('Master Data (Purchase).xlsx')
#print(work_book.nsheets) #有几个表单
company_1=work_book.sheet_by_index(0)
company_2=work_book.sheet_by_index(1)
company_3=work_book.sheet_by_index(2)
purchase_list=[]
time=[]
time_tmp=[]
type_name=[];
Name="A001"
for i in range(1,company_1.nrows):
    if (company_1.row(i)[0].value == Name):
        type_name.append(company_1.row(i)[4].value) 
        time_tmp.append(company_1.row(i)[3].value)  
    
    else:
        purchase_list.append(type_name)
        type_name=[]
        time.append(time_tmp)
        time_tmp=[]
        Name=company_1.row(i)[0].value
        type_name.append(company_1.row(i)[4].value)
#f=open('data.txt','w')
#f.write(str(purchase_list))  
print(len(time[0])) 
a=pd.DataFrame(data=purchase_list[0])
a.to_csv('data.csv')
#print(purchase_list[1][0])
#print(ord(company_1.row(1)[0].value)-ord("A001"))