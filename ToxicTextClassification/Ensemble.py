import numpy as np, pandas as pd
df_logistic=pd.read_csv('C:/Users/ketka/Desktop/kaggle/submission.csv')
df_xgb=pd.read_csv('C:/Users/ketka/Desktop/kaggle/submission1.csv')
labels = ['toxic', 'severe_toxic', 'obscene', 'threat', 'insult', 'identity_hate']
df_ensemble=df_xgb.copy()
df_ensemble[labels]=df_logistic[labels]*0.6 + df_xgb[labels]*0.4
df_ensemble.to_csv('C:/Users/ketka/Desktop/kaggle/output.csv',index=False)
