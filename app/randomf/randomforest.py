import sys
from joblib import load
import pandas as pd

# Corrected indexing for reading arguments from command line
time = sys.argv[2]
SensorId = sys.argv[3]
ax = float(sys.argv[4])
ay = float(sys.argv[5])
az = float(sys.argv[6])
gx = float(sys.argv[7])
gy = float(sys.argv[8])
gz = float(sys.argv[9])

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