from miio import Device
import time  # time 모듈을 임포트



# 기기의 IP 주소와 토큰을 설정
global ip_address
global token
ip_address = "192.168.111.36"
token = "fbeacba7c7ddde121f79ab56fb5be30c"

# Device 클래스를 사용하여 기기에 연결
device = Device(ip_address, token)

def dispense(n):
    # Device 클래스를 사용하여 장치에 연결
    device = Device(ip_address, token)

    for i in range(n):
        time.sleep(10)

        # 사료를 배출하는 명령을 전송
        result = device.send("action", {
            "did": "action",
            "siid": 2,  # pet-feeder 서비스의 SIID
            "aiid": 1,  # pet-food-out 액션의 AIID
        })
        print(f"Action result: {result}")

# Device Fault 프로퍼티를 확인하는 명령을 전송
fault = device.send("get_prop", ["fault"])
print(f"Device Fault: {fault}")
dispense(2)
