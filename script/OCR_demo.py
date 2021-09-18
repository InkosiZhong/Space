import base64
import requests
import sys
from urllib import parse

request_url = "https://aip.baidubce.com/rest/2.0/ocr/v1/accurate"

if __name__ == '__main__':
    access_token =  sys.argv[1]
    with open("/Users/inkosizhong/lab/space/test.png", 'rb') as f:
        img = base64.b64encode(f.read())
        print (img)
        params = {"image":img}
        request_url = f"{request_url}?access_token={access_token}"
        headers = {'content-type': 'application/x-www-form-urlencoded'}
        response = requests.post(request_url, data=params, headers=headers)
        if response:
            print (response.json())