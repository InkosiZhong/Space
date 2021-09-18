# encoding:utf-8
import requests 
import sys

if __name__ == '__main__':
    app_key =  sys.argv[1]
    secret_key =  sys.argv[2]
    # client_id 为官网获取的AK， client_secret 为官网获取的SK
    host = f'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id={app_key}&client_secret={secret_key}'
    response = requests.get(host)
    if response:
        if 'access_token' in response.json():
            print(response.json()['access_token'])
        else:
            print(response.json())