from PiicoDev_VEML6030 import PiicoDev_VEML6030
from PiicoDev_TMP117 import PiicoDev_TMP117
from PiicoDev_MPU6050 import PiicoDev_MPU6050
from PiicoDev_Unified import sleep_ms # cross-platform compatible sleep function


from time import sleep, time
from dataclasses import dataclass
from collections import namedtuple

import tflite_runtime.interpreter as tflite
from numpy import float32

light_scale_multiplier = 103.34
light_scale_addition = 0.0
temp_scale_multiplier = 18.48
temp_scale_addition = -5.31


@dataclass
class DataSample:
    light_lux: float = 0.0
    temp_c: float = 0.0
    alt_temp_c: float = 0.0
    accel_x_meterpersec: float = 0.0
    accel_y_meterpersec: float = 0.0
    accel_z_meterpersec: float = 0.0
    gyro_x_degreepersec: float = 0.0
    gyro_y_degreepersec: float = 0.0
    gyro_z_degreepersec: float = 0.0

def print_sample(sample):
    print(str(sample.light_lux) + " lux", end =" ")
    print(str(sample.temp_c) + " °C", end =" ")
    print(str(sample.alt_temp_c) + " _C", end = " ")
    print(str(sample.accel_x_meterpersec) + " xmps " + str(sample.accel_y_meterpersec) + " ymps " + str(sample.accel_z_meterpersec) + "zmps", end =" ")
    print(str(sample.gyro_x_degreepersec) + " xdps " + str(sample.gyro_y_degreepersec) + " ydps " + str(sample.gyro_z_degreepersec) + "zdps")

def init_sensors():
    # Initialise Sensor
    light = PiicoDev_VEML6030()
    tempSensor = PiicoDev_TMP117() 
    motion = PiicoDev_MPU6050()
    sensor_collection = namedtuple('Sensors', ['light', 'temp', 'motion'])
    return sensor_collection(light, tempSensor, motion)

def get_accel_sample(sensors, sample):
    # Accelerometer data
    accel = sensors.motion.read_accel_data() # read the accelerometer [ms^-2]
    sample.accel_x_meterpersec = accel["x"]
    sample.accel_y_meterpersec = accel["y"]
    sample.accel_z_meterpersec = accel["z"]

def get_gyro_sample(sensors, sample):
    # Accelerometer data
    gyro = sensors.motion.read_gyro_data() # read the accelerometer [ms^-2]
    sample.gyro_x_degreepersec = gyro["x"]
    sample.gyro_y_degreepersec = gyro["y"]
    sample.gyro_z_degreepersec = gyro["z"]



def get_temp_sample(sensors, sample):
    sample.temp_c = sensors.temp.readTempC()
    sample.alt_temp_c = sensors.motion.read_temperature()

def get_light_sample(sensors, sample):
    sample.light_lux = sensors.light.read()

def scale_light(light_lux):
    ml = light_lux/1000
    return ml*light_scale_multiplier + light_scale_addition

def scale_temp(temp_c):
    ml = temp_c/40
    return ml*temp_scale_multiplier + temp_scale_addition

def scale_sample(sample):
    # Apply scaling factor to prepare sample for classification
    scaled_light = float32(scale_light(sample.light_lux))
    scaled_temp = float32(scale_temp(sample.temp_c))
    return [[scaled_light,scaled_temp]]


def collect_sample(sensors):
    sample = DataSample()
    get_light_sample(sensors, sample)
    get_temp_sample(sensors, sample)
    get_accel_sample(sensors, sample)
    get_gyro_sample(sensors, sample)
    return sample



def init_ml_model():
    # Initialise tflite model and allocate tensors
    ml_model = namedtuple('Model', ['interpreter', 'input', 'output'])
    ml_model.interpreter = tflite.Interpreter(model_path="tensorflow/model.tflite")  
    ml_model.interpreter.allocate_tensors()
    ml_model.input_details = ml_model.interpreter.get_input_details()    
    ml_model.output_details = ml_model.interpreter.get_output_details()
    return ml_model

def classify_sample(ml_model, scaled_sample):
    # Use Tensorflow to catgorise door as open or closed
    ml_model.interpreter.set_tensor(ml_model.input_details[0]['index'], scaled_sample)
    ml_model.interpreter.invoke()
    output_data = ml_model.interpreter.get_tensor(ml_model.output_details[0]['index'])

    return output_data[0][0]


sensors = init_sensors()
ml_model = init_ml_model()

while True:
    sample = collect_sample(sensors)                                    #;print_sample(sample)
    scaled_sample = scale_sample(sample)                                #;print(scaled_sample)
    start_time = time()

    #Use Tensorflow to catgorise door as open or closed
    door_open_probability = classify_sample(ml_model, scaled_sample)    #;print(door_open_probability)
    
    execution_time_ms = (time()-start_time) * 1000
    print('tensorflow execution time: {:.3f}ms'.format(execution_time_ms), end = " ")
    if door_open_probability > 0.5:
        print(f"probability that the door is OPEN is {door_open_probability*100}%")
    else:
        print(f"probability that the door is closed is {100 - door_open_probability*100}%")

    sleep(1)
