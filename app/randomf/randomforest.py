# model predict (활동 상태), 소비 칼로리

import mysql.connector
import time
from datetime import datetime, timedelta
import random
import numpy as np
import pandas as pd
import schedule

from joblib import load

# db에서 받아와야 하는 변수 (임의 지정)
weight = 3
dogid = 'dung'
acttime = '2023-11-06 10:30:00'

rer = round(70 * weight**(0.75), 2)
min_rer = round(rer/1440, 2)

# # MySQL 연결 설정
# mydb = mysql.connector.connect(
#     host="r",
#     user="r",
#     password="r",
#     database="r"
# )
# mycursor = mydb.cursor()

# 모델 로드
loaded_model = load('app/randomf/random_forest_model.joblib')

# 센서값 입력 함수 
def get_data_and_diff():
    ax, ay, az, gx, gy, gz =0.47, 0.61, -0.66, 203.74, 47.3, 15.81 
    new_data = pd.DataFrame({'ax': [abs(ax)],'ay': [abs(ay)],'az': [abs(az)], 'gx': [abs(gx)], 'gy': [abs(gy)], 'gz': [abs(gz)]})
    return new_data

def insert_activity(acttime, dogid, stop, walk, run, kcal):
    sql = "INSERT INTO activity (ActTime, DogId, Stop, Walk, Run, Kcal) VALUES (%s, %s, %s, %s, %s, %s)"
    val = (acttime, dogid, stop, walk, run, kcal)
    # mycursor.execute(sql, val)
    # mydb.commit()

sensor_data = get_data_and_diff()
prediction =loaded_model.predict(sensor_data)
print(prediction)

if prediction == 0:
    kcal = min_rer
    insert_activity(acttime, dogid, 1, 0, 0, kcal)
elif prediction == 1:
    kcal = min_rer*5
    insert_activity(acttime, dogid, 0, 1, 0, kcal)
elif prediction == 2:
    kcal = min_rer*10
    insert_activity(acttime, dogid, 0, 0, 1, kcal)
