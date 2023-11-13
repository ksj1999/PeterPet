import mysql.connector
import sqlite3
import time
from datetime import datetime, timedelta
import random
import numpy as np
import pandas as pd
import schedule

from joblib import dump, load

# 모델 로드
loaded_model = load('random_forest_model.joblib')

# MySQL 연결 설정
mydb = mysql.connector.connect(
    host="r",
    user="r",
    password="r",
    database="r"
)
mycursor = mydb.cursor()


# 서버에서 받아와야 하는 변수
rer = 160
min_rer = rer/1440
feed = 3785
dogid = 'dung'
neuter = 0
bcs = '비만'

current_time = datetime.now()


# 센서값 입력 함수 
def get_data_and_diff():
    ax, ay, az, gx, gy, gz =0.47, 0.61, -0.66, 203.74, 47.3, 15.81 #walk
    
    new_data = pd.DataFrame({'ax': [abs(ax)],'ay': [abs(ay)],'az': [abs(az)], 'gx': [abs(gx)], 'gy': [abs(gy)], 'gz': [abs(gz)]})
    action =loaded_model.predict(new_data)

    return action


def get_recent_kcal():
    mycursor.execute("SELECT kcal FROM activity ORDER BY ActTime DESC LIMIT 1")
    recent_kcal = mycursor.fetchone()[0]

    return recent_kcal

# db 저장 함수
def insert_activity(acttime, dogid, stop, walk, run, kcal):
    sql = "INSERT INTO activity (ActTime, DogId, Stop, Walk, Run, Kcal) VALUES (%s, %s, %s, %s, %s, %s)"
    val = (acttime, dogid, stop, walk, run, kcal)
    mycursor.execute(sql, val)
    mydb.commit()

def insert_actkcal(date, dogid, actlevel, needkcal):
    sql = "INSERT INTO actkcal (Date, DogId, ActLevel, NeedKcal) VALUES (%s, %s, %s, %s)"
    val = (date, dogid, actlevel, needkcal)
    mycursor.execute(sql, val)
    mydb.commit()

def insert_dispenser(feedtime, dogid, amount, eatkcal):
    sql = "INSERT INTO dispenser (FeedTime, DogId, Amount, EatKcal) VALUES (%s, %s, %s, %s)"
    val = (feedtime, dogid, amount, eatkcal)
    mycursor.execute(sql, val)
    mydb.commit()

def everyday(now_kcal):
    global activity_level, der, amount_feed

    # 중성화, 비만 여부 어떻게 입력되는지 몰라서 일단 형식만
    if neuter == 0:
        activity_level = 1.6
    if bcs == '비만':
        activity_level = 1.4

    if now_kcal < rer/5:
        activity_level = 1.8 #활동적이지 않음, 비만 경향
        act_level = 0
    elif rer/5 <= now_kcal < rer/3:
        activity_level = 2.0 #활동적
        act_level = 1
    elif now_kcal >= rer/3:
        activity_level = 3.5 #강도 높은 활동
        act_level = 2

    der = activity_level * rer
    amount_feed = der / feed * 1000

    insert_actkcal(current_time, dogid, act_level, der)
    insert_dispenser(current_time, dogid, amount_feed, eatkcal) # eatkcal 뭐지??
    



while True:
    # 24시에 소비 칼로리 초기화
    if current_time.hour == 0 and current_time.minute == 0 and current_time.second == 0:
        now_kcal = 0

    
    # 활동지수 결정, 사료량 결정 (매일 한 번)
    # dispenser 도 같이 하게 했는데 하루 두 번으로 빼도 됨
    schedule.every().day.at("23:59").do(everyday(now_kcal))
    

    # 1분마다 데이터->모델 결과
    action = get_data_and_diff()

    if action == 0:
        insert_activity(current_time, dogid, 1, 0, 0, min_rer)
    elif action == 1:
        insert_activity(current_time, dogid, 0, 1, 0, min_rer * 5)
    elif action == 2:
        insert_activity(current_time, dogid, 0, 0, 1, min_rer * 10)
    
    now_kcal = now_kcal + get_recent_kcal

    # 일단 시간 여기로 했는데 서버에서 관리하는 게 나을까?
    time.sleep(60) 

