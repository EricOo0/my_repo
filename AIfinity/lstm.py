import torch
import torch.nn as nn
import xlrd
import seaborn as sns
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys
from sklearn.preprocessing import MinMaxScaler
#%matplotlib inline

#sns.get_dataset_names()
def data_collect():
    #Data processing
    work_book=xlrd.open_workbook('Master Data (Purchase).xlsx')
    #print(work_book.nsheets) #有几个表单
    company_1=work_book.sheet_by_index(0)
    company_2=work_book.sheet_by_index(1)
    company_3=work_book.sheet_by_index(2)
    purchase_list=[]
    type_name=[];
    Name="A001"
    for i in range(1,company_1.nrows):
        if (company_1.row(i)[0].value == Name):
            type_name.append(company_1.row(i)[4].value)   
    
        else:
            purchase_list.append(type_name)
            type_name=[]
            Name=company_1.row(i)[0].value
            type_name.append(company_1.row(i)[4].value)
    #flight_data.head()
    return purchase_list
data=data_collect()

# data process
test_data_size = len(data[0])-700 
train_data = data[0][:-test_data_size] 
test_data = data[0][-test_data_size:]
print(train_data)
train_data=np.array(train_data)
test_data=np.array(test_data)
#normalization
scaler = MinMaxScaler(feature_range=(-1, 1))
train_data_normalized = scaler.fit_transform(train_data.reshape(-1, 1))
train_data_normalized = torch.FloatTensor(train_data_normalized).view(-1)
train_window = 180
def create_inout_sequences(input_data, tw): 
    inout_seq = [] 
    L = len(input_data) 
    for i in range(L-tw): 
        train_seq = input_data[i:i+tw] 
        train_label = input_data[i+tw:i+tw+30] 
        inout_seq.append((train_seq ,train_label)) 
        return inout_seq
train_inout_seq = create_inout_sequences(train_data_normalized, train_window)
#LSTM Module
class LSTM(nn.Module):
    def __init__(self, input_size=1, hidden_layer_size=200, output_size=1,num_layers=3):
        super().__init__()
        self.hidden_layer_size = hidden_layer_size

        self.lstm = nn.LSTM(input_size, hidden_layer_size)
       # self.lstm = nn.LSTM(input_size, hidden_layer_size,num_layers)
        self.linear = nn.Linear(hidden_layer_size, output_size)

        self.hidden_cell = (torch.zeros(1,1,self.hidden_layer_size),
                            torch.zeros(1,1,self.hidden_layer_size))

    def forward(self, input_seq):
        lstm_out, self.hidden_cell = self.lstm(input_seq.view(len(input_seq) ,1, -1), self.hidden_cell)
        predictions = self.linear(lstm_out.view(len(input_seq), -1))
        return predictions[-1]
    
model = LSTM()
loss_function = nn.MSELoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
print(model)
# data process

#Module training
epochs = 150

for i in range(epochs):
    for seq, labels in train_inout_seq:
        optimizer.zero_grad()
        model.hidden_cell = (torch.zeros(1, 1, model.hidden_layer_size),
                        torch.zeros(1, 1, model.hidden_layer_size))

        y_pred = model(seq)

        single_loss = loss_function(y_pred, labels)
        single_loss.backward()
        optimizer.step()

    if i%25 == 1:
        print(f'epoch: {i:3} loss: {single_loss.item():10.8f}')

print(f'epoch: {i:3} loss: {single_loss.item():10.10f}')


#Predict module
fut_pred = 30

test_inputs = train_data_normalized[-train_window:].tolist()
print(test_inputs)

model.eval()

for i in range(fut_pred):
    seq = torch.FloatTensor(test_inputs[-train_window:])
    with torch.no_grad():
        model.hidden = (torch.zeros(1, 1, model.hidden_layer_size),
                        torch.zeros(1, 1, model.hidden_layer_size))
        test_inputs.append(model(seq).item())
actual_predictions = scaler.inverse_transform(np.array(test_inputs[train_window:] ).reshape(-1, 1)) 
t = scaler.inverse_transform(np.array(test_inputs ).reshape(-1, 1)) 
#print(t)
print(actual_predictions)