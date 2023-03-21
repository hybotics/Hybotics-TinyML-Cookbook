#!/usr/bin/env python3
import csv
import datetime
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import seaborn as sns
import sklearn.metrics
import tensorflow as tf

from numpy import mean
from numpy import std
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from tensorflow.keras import activations
from tensorflow.keras import layers
from wwo_hist import retrieve_hist_data

def binarize(snow, threshold):
    if snow > threshold:
        return 1
    else:
        return 0

def scaling(val, avg, std):
    return (val - avg) / std

try:
    from Secrets import Secrets
except ImportError:
    print("You must create a Secrets.py file!")
    raise

ssid = Secrets['WiFi_SSID']
psk = Secrets['WiFi_PSK']

api_key = Secrets['wwo_api_key']

frequency = 1
location_list = ['canazei']

df_weather = retrieve_hist_data(api_key,
    location_list,
    '01-JAN-2011',
    '31-DEC-2020',
    frequency,
    location_label=False,
    export_csv=False,
    store_df=True)
    
t_list = df_weather[0].tempC.astype(float).to_list()
h_list = df_weather[0].humidity.astype(float).to_list()
s_list = df_weather[0].totalSnow_cm.astype(float).to_list()

s_bin_list = [binarize(snow, 0.5) for snow in s_list]
cm = plt.cm.get_cmap('gray_r')
sc = plt.scatter(t_list, h_list, c=s_bin_list, cmap=cm, label='Snow')
plt.figure(dpi=150)
plt.colorbar(sc)
plt.legend()
plt.grid(True)
plt.title('Snow (T, H)')
plt.xlabel('Temperature C')
plt.ylabel('Humidity %')
plt.show()


