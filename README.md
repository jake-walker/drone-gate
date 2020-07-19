# Drone Gate

A drone gate that times laps using an Arduino.

> **This is a work in progress** and it's a bit of an experiment so far to see if something like this will work reliably with fast moving drones. At the moment I'm using an ultrasonic sensor which I'm thinking won't be very reliable with detecting a drone passing.

## Intro

I've recently gotten into drone racing as a hobby. I'm still only a couple of hours into flying but I thought that it would be fun to make an obstacle to fly though. So I made a small 'gate' out of PEX tube and some duct-taped on pegs.

The gate itself works quite well as an obstacle to fly though but I was then thinking that I could add some simple electronics to make it so that it can detect when I've flown through and begin a timer.

My first prototype is using an Arduino Nano, OLED screen and ultrasonic sensor as that's what I currently have on hand. I'm planning to upgrade the ultrasonic sensor to an RX5808 module that can detect the signal strength of a drone's video feed to trigger a lap. That should end up giving more reliable timings and there are many open source projects already such as [PIDflight Lap](https://www.pidflight.com/pidflight-lap/).

## Setup

You'll need to install [PlatformIO](https://platformio.org/) and plug in your Arduino Nano to program it.

The HC-SR04 ultrasonic sensor is wired so that trigger goes to pin 12 and echo to pin 11. The SSD1306 OLED screen is I2C.

## How it works

- Turn on the Arduino and it will immediately use the ultrasonic sensor to calibrate itself to a baseline reading.
- When a drone flies through, it should reduce the distance of the ultrasonic sensor to quite far below the baseline reading.
- This triggers a lap and the timer begins.
- When the drone flies through again, the timer stops and the time is shown on the screen.
