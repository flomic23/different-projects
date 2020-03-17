import requests
import json
with open("input.txt") as f:
    for i_str in f:
        a = ("http://numbersapi.com/"+str(i_str[:-1])+"/math?json=true")
        res = requests.get(a)
        a_text = res.text
        #print(type(a_text), a_text)
        if '''"found": true''' in a_text:
            print("Interesting")
        else:
            print("Boring")

'''
еще варианты:
import requests as re

with open('dataset_24476_3.txt') as file:
    for num in file:
        response = re.get('http://numbersapi.com/{number}/math?json=true'.format( number=num.rstrip() )).json()﻿
        ﻿
        print('Interesting') if response['found'] else print('Boring')﻿
        
#####################  
        
        
import requests

api_url = 'http://numbersapi.com/{}/math?json=true'
for number in open('input.txt').read().splitlines():
    res = requests.get(api_url.format(number))
    print('Interesting') if res.json()['found'] else print('Boring')
'''