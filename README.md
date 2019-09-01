You will need:
-------------
* esp8266 (I'm using nodemcu v3)
* NPN transistor (almost any 30V tollerant will do beacuse most of gates have 250ma max power)
* soldering iron
* at least 20 IQ

## Basic Setup:

1.Download and edit the code to your needs.   
2.Get a NPN transistor (you can use other one if you know what you are doing).  
3.Solder transistor base to digital pin 6 and emitter to ground pin.  
4.Find place where you connect internal buttons in your garage gate opener.  
5.Check the polarity, connect positive to collector and negative to emitter.  
6.Connect esp to power and now your gate should open whenever you open this link http://yourip/garage.  

(optional)7.Make app to control the garage https://appinventor.mit.edu/, it's super easy so I recommend it.

### Connecting Google Assistant:
**First set up ip tunneling:** \
Opening port on your router would be a very bad idea we need to install ngrok to get a secure public link to our local ip so we can access it from IFTTT. I'm setting it up on raspberry pi beacuse it has to work 24/7 but you can use any other device (even a phone with app called userland).

Register and download ngrok from https://ngrok.com/ to your specific device, I'm downloading Linux (ARM) for raspberry pi if you are using other device download correct package. Also you will need authtoken, you can find it on ngrok Dashboard under Auth category.
```sh
$ wget https://bin.equinox.io/c/4VmDzA7iaHb/ngrok-stable-linux-arm.zip
$ sudo apt install unzip screen
$ unzip ngrok-stable-linux-arm.zip
$ ./ngrok authtoken yourauthtoken
$ screen ./ngrok http http://your_esp_ip 
```
And copy link which you see we will need it later.

**Second:**  
1.Visit https://ifttt.com/ and login with your google account. \
2.Create your own applet.  
3.Clcik on this and search for google assistant.  
4.Fill all spaces and next.  
5.Search for webhooks and paste there your tunneled link like this "http://asdasasd.ngrok.io/garage"  
6.Save and done. Now say your phrase to test it.
