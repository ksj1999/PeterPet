# NeedKcal

# db에서 받아와야 하는 변수 (임의 지정)
weight = 3
dogid = 'dung'
neuter = True
bcs = 'Obesity'
acttime = '2023-11-06 10:30:00'
now_kcal = 50
frequency = 3 #
feed = 3785 #


class CalorieCalculator:
    def __init__(self, acttime, dogid, weight, bcs):
        self.time = acttime
        self.id = dogid
        self.rer = round(70 * weight**(0.75), 2)
        self.bcs = bcs

    def calculate_der(self, now_kcal):
        
        if now_kcal < self.rer/5:
            act_level = 0
        elif self.rer/5 <= now_kcal < self.rer/3:
            act_level = 1
        elif now_kcal >= self.rer/3:
            act_level = 2

        if self.bcs == 'Obesity':
            der = 1.4 * self.rer
        elif self.bcs == 'Underweight':
            der = 2 * self.rer
        elif self.bcs == 'Normal':
            if act_level == 0:
                der = 1.8 * self.rer # 활동 적음
            elif act_level == 1:
                der = 2.0 * self.rer # 보통 (활동적)
            elif act_level == 2:
                der = 3.5 * self.rer # 강도 높은 활동

        return [der, act_level]
    
class FeedManager:
    def __init__(self, acttime, dogid, weight, bcs, now_kcal):
        
        self.calorie_cal = CalorieCalculator(acttime, dogid, weight, bcs)
        self.cal_der = self.calorie_cal.calculate_der(now_kcal)[0]

    def daily_amount(self, feed):
        amount_feed = self.cal_der / feed * 1000

        return amount_feed
        
      

calorie_calculator = CalorieCalculator(acttime, dogid, weight, bcs)
feed_manager = FeedManager(acttime, dogid, weight, bcs, now_kcal)

need_kcal = round(calorie_calculator.calculate_der(now_kcal)[0], 2)
act_level = calorie_calculator.calculate_der(now_kcal)[1]
amount = round(feed_manager.daily_amount(feed),2)
eat_kcal = round(amount/frequency,2)

print(need_kcal, act_level, amount, eat_kcal)


# db 저장은 뺌