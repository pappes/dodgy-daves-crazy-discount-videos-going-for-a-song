from PiicoDev_VEML6030 import PiicoDev_VEML6030
from PiicoDev_TMP117 import PiicoDev_TMP117
from PiicoDev_MPU6050 import PiicoDev_MPU6050
from PiicoDev_Unified import sleep_ms # cross-platform compatible sleep function


from time import sleep, time
from dataclasses import dataclass

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


def get_temp_sample(sample):
    sample.temp_c = tempSensor.readTempC()
    sample.alt_temp_c = motion.read_temperature()

def get_light_sample(sample):
    sample.light_lux = light.read()

def get_accel_sample(sample):
    # Accelerometer data
    accel = motion.read_accel_data() # read the accelerometer [ms^-2]
    sample.accel_x_meterpersec = accel["x"]
    sample.accel_y_meterpersec = accel["y"]
    sample.accel_z_meterpersec = accel["z"]

def get_gyro_sample(sample):
    # Accelerometer data
    gyro = motion.read_gyro_data() # read the accelerometer [ms^-2]
    sample.gyro_x_degreepersec = gyro["x"]
    sample.gyro_y_degreepersec = gyro["y"]
    sample.gyro_z_degreepersec = gyro["z"]

def scale_light(light_lux):
    ml = light_lux/1000
    return ml*light_scale_multiplier + light_scale_addition

def scale_temp(temp_c):
    ml = temp_c/40
    return ml*temp_scale_multiplier + temp_scale_addition

def classify_sample(sample):
    # Use Tensorflow to catgorise door as open or closed
    slice = [[float32(scale_light(sample.light_lux)),float32(scale_temp(sample.temp_c))]]
    print(slice)

    interpreter = tflite.Interpreter(model_path="tensorflow/model.tflite")  
    interpreter.allocate_tensors()
    input_details = interpreter.get_input_details()    
    output_details = interpreter.get_output_details()

    interpreter.set_tensor(input_details[0]['index'], slice)
    start_time = time()
    interpreter.invoke()
    stop_time = time()

    output_data = interpreter.get_tensor(output_details[0]['index'])
    print(output_data)
    print('time: {:.3f}ms'.format((stop_time - start_time) * 1000))



# Initialise Sensor
light = PiicoDev_VEML6030()
tempSensor = PiicoDev_TMP117() 
motion = PiicoDev_MPU6050()


while True:
    sample = DataSample()
    get_light_sample(sample)
    get_temp_sample(sample)
    get_accel_sample(sample)
    get_gyro_sample(sample)
    print(str(sample.light_lux) + " lux", end =" ")
    print(str(sample.temp_c) + " °C", end =" ")
    print(str(sample.alt_temp_c) + " _C", end = " ")
    print(str(sample.accel_x_meterpersec) + " xmps " + str(sample.accel_y_meterpersec) + " ymps " + str(sample.accel_z_meterpersec) + "zmps", end =" ")
    print(str(sample.gyro_x_degreepersec) + " xdps " + str(sample.gyro_y_degreepersec) + " ydps " + str(sample.gyro_z_degreepersec) + "zdps")

    #Use Tensorflow to catgorise door as open or closed
    classify_sample(sample)

    sleep(3)
