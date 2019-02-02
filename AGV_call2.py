#!/usr/bin/env python
import rospy
import pyrebase
import signal
import sys
import time

config = {
	"apiKey": "apiKey",
	"authDomain": "agv-jinpao.firebaseio.com",
	"databaseURL": "https://agv-jinpao.firebaseio.com",
	"storageBucket": "agv-jinpao.appspot.com"
}
firebase = pyrebase.initialize_app(config)
db = firebase.database()

if __name__ == '__main__':
		while True:
			next_position = db.child("position").child("next_position").get().val()
			now_position = db.child("position").child("now_position").get().val()

			if next_position != now_position:
				if now_position==1:
					if next_position==2:
						print "Station 1 to 2"
					if next_position==3:
						print "Station 1 to 3"
				if now_position==2:
					if next_position==1:
						print "Station 2 to 1"
					if next_position==3:
						print "Station 2 to 3"
				if now_position==3:
					if next_position==1:
						print "Station 3 to 1"
					if next_position==2:
						print "Station 3 to 2"
