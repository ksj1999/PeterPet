import sys
from joblib import load
import pandas as pd

# Corrected indexing for reading arguments from command line
time = sys.argv[1]
SensorId = sys.argv[2]
ax = float(sys.argv[3])
ay = float(sys.argv[4])
az = float(sys.argv[5])
gx = float(sys.argv[6])
gy = float(sys.argv[7])
gz = float(sys.argv[8])

# Create a DataFrame with sensor data
sensor_data = pd.DataFrame({
    'ax': [abs(ax)],
    'ay': [abs(ay)],
    'az': [abs(az)],
    'gx': [abs(gx)],
    'gy': [abs(gy)],
    'gz': [abs(gz)]
})

# Load the model
loaded_model = load('randomf\\random_forest_model.joblib')

# Predict using the model
prediction = loaded_model.predict(sensor_data)
print(prediction)