# This Python 3 environment comes with many helpful analytics libraries installed
# It is defined by the kaggle/python docker image: https://github.com/kaggle/docker-python
# For example, here's several helpful packages to load in 

import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)

# Input data files are available in the "../input/" directory.
# For example, running this (by clicking run or pressing Shift+Enter) will list the files in the input directory

import os
print(os.listdir("../input"))

# Any results you write to the current directory are saved as output.
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import cross_val_score
from sklearn.pipeline import make_union
import xgboost as xgb

classes = ['toxic', 'severe_toxic', 'obscene', 'threat', 'insult', 'identity_hate']

df_train=pd.read_csv('../input/train.csv').fillna(' ')
df_test=pd.read_csv('../input/test.csv').fillna(' ')
train_txt=df_train["comment_text"]
test_txt=df_test["comment_text"]
combined_txt=pd.concat([train_txt,test_txt])
word_vec = TfidfVectorizer(
    sublinear_tf=True,
    strip_accents='unicode',
    analyzer='word',
    token_pattern=r'\w{1,}',
    ngram_range=(1, 1),
    max_features=10000)
char_vec = TfidfVectorizer(
    sublinear_tf=True,
    strip_accents='unicode',
    analyzer='char',
    ngram_range=(2, 6),
    max_features=50000)
vectorizer = make_union(word_vec, char_vec, n_jobs=2)
vectorizer.fit(combined_txt)
train = vectorizer.transform(train_txt)
test = vectorizer.transform(test_txt)
submission = pd.DataFrame.from_dict({'id': df_test['id']})
for class_name in classes:
    train_target = df_train[class_name]
    classifier = LogisticRegression(solver='sag')
    classifier.fit(train, train_target)
    submission[class_name] = classifier.predict_proba(test)[:, 1]

submission.to_csv('submission.csv', index=False)
