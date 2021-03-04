# -*- coding: utf-8 -*-
import pandas as pd
import numpy as np
from fbprophet import Prophet
print('a')
#holiday
TF_Spring=pd.DataFrame({
    'holiday':'Spring Festival',
    'ds':pd.to_datetime(['2018-2-15','2019-2-5','2020-1-24']),
      'lower_window': -3,
  'upper_window': 4,
})
TF_holiday=pd.DataFrame({
    'holiday':'holiday1',
    'ds':pd.to_datetime(['2018-5-29','2019-4-15','2020-6-24']),
      'lower_window': -4,
  'upper_window': 4,
})
TF_yuandan=pd.DataFrame({
    'holiday':'yuandan',
    'ds':pd.to_datetime(['2018-1-1','2019-1-1','2020-1-1']),
      'lower_window': -2,
  'upper_window': 2,
})
holidays=pd.concat((TF_Spring,TF_holiday,TF_yuandan))
menus=["Enoki Mushroom1.csv","Enoki Mushroom2.csv","Enoki Mushroom3.csv",
        "Oyster Mushroom1.csv","Oyster Mushroom2.csv","Oyster Mushroom3.csv",
        "Shiitake Mushroom1.csv","Shiitake Mushroom2.csv","Shiitake Mushroom3.csv",
        "King Mushroom1.csv","King Mushroom2.csv","King Mushroom3.csv",
        "Buna Shimeiji1.csv","Buna Shimeiji2.csv","Buna Shimeiji3.csv",
        "Bamboo Shoot1.csv","Bamboo Shoot2.csv","Bamboo Shoot3.csv",
        "Spinach1.csv","Spinach2.csv","Spinach3.csv",
        "Tung Oh1.csv","Tung Oh2.csv","Tung Oh3.csv",
        "Xiang Mai1.csv","Xiang Mai2.csv","Xiang Mai3.csv",
        "Fresh Lettuce1.csv","Fresh Lettuce2.csv","Fresh Lettuce3.csv",
        "Chinese Cabbage1.csv","Chinese Cabbage2.csv","Chinese Cabbage3.csv",
        "Baby Cabbage1.csv","Baby Cabbage2.csv","Baby Cabbage3.csv"
]
for menu in menus:
    print(menu)
    df = pd.read_csv(menu,encoding='unicode_escape')
    #df['y'] = np.log(df['y'])
    print(df.head())
    m = Prophet(n_changepoints=50,changepoint_prior_scale=0.8,holidays=holidays,
                interval_width=0.90 )
    m.fit(df);

    #future = m.make_future_dataframe(periods=30)
    future = m.make_future_dataframe(periods=31)

    forecast = m.predict(future)

    m.plot(forecast)
    print(type(forecast.tail(31)['yhat']))
    forecast.tail(31)['yhat'].to_excel(menu+".xls")
    #future.tail()
