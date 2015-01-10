WiseUp
======

Wiseup is an intelligent home assistant open source framework that consists of three basic devices Gateway, Mediators and Sensors.

##Gateway
  Based on Raspberry Pi (Galileo/Edison in the future). All data is saved in this gateway that backs up the data on Cloud      service.
  All the important analytic scripts are running locally on the Gateway and analyzing sensor data every 3 seconds.
  
  Important modules
  - Wiseup service - this layer communicates with the sensors using Wiseup protocol.
  - Node.js services - this layer contains Web Service, connection to Mongo DB (local DB) and connection to Redis Service.
  - Redis service - Publish and Subscribe feature, also for fast data storage.
  - Mongo DB - local database.
  - Analytic services - based on node.js, executes analysis script on DB for detecting sensors features.
  - And more ...

##Mediator
  Main work frequency is 2.4Ghz with private protocol, but what if we want to use other sensors from EBay or other brands?
  For that we have the mediators that will convert from our protocol and frequency to other so we'll be able to consume
  other sensors.
  
##Sensors
  We have our own sensors running on batteries or directly from 5V power.
  
##Communication 
  nRF24l01 - 2.4Ghz very low cost and low power transceiver.
  
  RFComm - Private protocol for sensors/gateway communication. 

##Developers
- Yevgeniy Kiveisha - Linux and Embedded.
- Sharon Elihis - PCB Designer and JavaScript developer.
- Aviv Ron - Architect and senior software developer.
- Yuri Shapiro - SolidWorks designer and architect.
- Jennifer Kiveisha - UI Designer and web client developer.
- Roy Beiser - Architect
