# Consistent-Cups-Espresso-Accessories
Open Source DIY solution to create Bluetooth-compatible pressure sensor and scale to use with a manual espresso machine. 

_**Project Background and Introduction**_

The market for espresso machines is a vastly large and competitive one. Every individual has their own preferences and lifestyles which result in the need for different machines. There are so many methods for making espresso that vary significantly in user effort, time, and price. 

**Semi-Automatic Espresso Machines**

Automatic machines require little user effort and time for extracting an espresso shot. However, the large downside to these machines is that, generally speaking, they cost upwards of $300-400 depending on the additional features and components they have. Once the grinds are tamped in the portafilter, the machine automatically applies the pressure for the duration of time that can sometimes be specified by the user.  

 
![images](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/d9ebb77a-002f-4e52-8c84-2f716b7bec55)

Figure 1: Semi-Automatic Espresso Machine Example 

**Manual Espresso Machines**

Manual espresso machines require a significant amount of preparation, time, and effort. These machines are generally cheaper than their automatic counterparts, being found for upwards of $100. For these machines, once the grinds are tamped in the portafilter, the user must also heat their water to the desired temperature. The user then needs a scale underneath the cup to be able to see how fast they are extracting the espresso. After pulling the lever and applying pressure for a set amount of time, the extraction is finally complete. The downsides to this machine are the extensive clean-up time, and the loss of water temperature during the extraction. However, this machine allows the user to have complete control over the amount of pressure and time variables for making their own ideal cup of espresso, perfect for the connoisseurs. With the pressure transducer and scale, it would be even easier to repeat their results or tweak it to their preferences. An example of what this type of machine looks like is in the right image of Figure 3. 

 

Besides the bean and how it's roasted, a good cup of lever espresso relies on three main factors: Pressure, temperature, and speed of the pull.  The speed of pull is determined by the grind size of the bean and the pressure.  Pressure is controlled by pulling the machine's handle.  A monitor system lets you track and save the pressure and pull speed data to a template.  You can then follow the template when doing a future pull, enabling consistent tasting espresso. 

The downside to such systems is the cost.  An entry level scale like the Acaia Lunar Espresso Scale is $250, and the pressure gauge is $460 from naked-portafilter (both pictured below). 

![Capture](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/4032a096-29ca-4a02-a5ee-2f61479c5c94)

Figure 2: Naked Portafilter Smart Espresso Profiler 

![Capture1](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/d52db8cd-98b8-4e13-8d43-337a77dcf7b6)

Figure 3: Acaia Lunar Espresso Scale 


**Description of the solution -**

The market for espresso machines is a vastly large and competitive one. Every individual has their own preferences and lifestyles which result in the need for different machines, which vary significantly in user effort, time, and price.  Manual lever espresso machines offer an affordable entrance into brewing quality espresso at home.  A monitor system helps the novice get consistent pulls of espresso by tracking and logging variables like pressure and speed of pull. The existing market design for the pressure sensor is $400 and the scale is $200, a steep cost that ends up on the same price level as some higher-end automatic machines. The design group would need to make an open-source pressure sensor and scale system that could wirelessly connect to the SE Profiler or Coffee Flow phone app, create a GitHub repository with schematics, bill of materials, 3D printed case files, PCB files, and build instructions.  This all accomplishable with the materials posted below in the BOM, a soldering iron, a 3D printer, and a computer!

**Bill of Materials -**

[BOM.xlsx](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/files/14973055/BOM.xlsx)


**PCB files -**

[FAB-Senior Design-A.5.zip](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/files/14779862/FAB-Senior.Design-A.5.zip)

We recommend using the following site to order the PCB! Just select 'Order Now', then upload the Gerber files provided above, select base material as FR-4, and then 'Save to cart' and continue to checkout!

https://jlcpcb.com/?from=VGBNA&gad_source=1&gclid=CjwKCAjwh4-wBhB3EiwAeJsppDn5Pd4jk2OESbvpThskq3OpfqVmY4BNuV4RASAHVzgLXOW_oEA4fBoCTGIQAvD_BwE

**Pressure sensor -**

![pressure sensor schematic](Images/Pressure/schem.jpg)

Figure 6: Wiring diagram


1. Casing files:
   
[pressure_base.zip](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/files/14780216/pressure_base.zip)

3. Components used:
   - Seeduino
   - Pressure Sensor
   - Battery
   - On/off switch
   - Casing
  
   Shown here:
  
![pressure sensor parts](Images/Pressure/parts.jpg)

How to put together: 

Step 1: Solder Power connections

- Using the battery, two short wires, and the seeduino (shown below in figure 4) follow the wiring diagram (shown below in figure 6) connecting the battery positive (red) wire to one of the pins on the switch. Then the battery negative (black) wire to the negative pin on the back of the seeduino. Next, solder a wire from the other switch pin to the positive pin on the back of the seeduino. 
- This final product attached to the casing cap is shown below in figure 5

![battery and xiao](Images/Pressure/bat.jpg)

Figure 4: Items needed

![battery soldered with switch](Images/Pressure/bat_soldered.jpg)

Figure 5: final product of this step


Step 2: Solder Pressure Sensor
- Using the two figures below, wire the pressure sensor with the Vcc to pin Vcc, the ground to ground, and the output to pin D2
- Make sure that the pressure sensor is wired thorugh the hole in the casing to the seeduino, this is to ensure that it can all be put together in the final step


![sensor pinout](Images/Pressure/pinout.jpg)

Figure 7: Sensor Wiring assignments

![cdd232f8-7777-4b1f-ac87-8251f978f37e](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/4a3bda79-6be8-4e8d-98c8-ada6fb880f56)

Figure 9: Wired pressure sensors with matching colors from figure 8

![1874f748-698e-4456-a6e0-650c7ba0616a](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/93d3a95b-ea01-45d5-9bd7-290a77a2d067)

Figure 10: Pressure sensor and seeduino attached and put into the casing


Step 3: Assemble
- To finalize this part of the pressure sensor, attach the antenna to the seeduino (as shown in figure 11)
- Next, fit all of the components into the casing, with seeduino on the shelf, the battery slotted into the main area of the casing, and the pressure sensor through the hole on the other side of the casing (all shown in figure 12)
- Final product shown in figure 13 and 14

![2c156bd9-9291-4784-b503-da6c319bb661](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/b1563120-563b-4975-b911-fdcc497daf99)

Figure 11: Attached antenna to the seedunio

![c7bbd2f2-a770-4a27-bed7-dbf7d90c0282](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/1a0d23a2-fa98-4957-b659-9a71fd4a8da2)

Figure 12: How to fit the components into the case

![88153bb2-ba61-4284-b39e-aa38bb277666](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/4722cc89-d3cd-4e37-b2ed-95cf308774fd)

![f19f561e-0c57-4f12-a13e-3d3df6846991](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/693ed929-bcd1-4e26-b73f-8e0c951ea7b0)

Figure 13/14: How the casing end should look when all is completed 





**Scale -**

1. Casing files:
   
[scale_bottom.zip](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/files/14780224/scale_bottom.zip)


3. Components used:
   - Seeduino
   - Load Cell
   - Battery
   - PCB
   - LCD
   - On/off switch
   - Casing
   - Header Pins
   
How to put together:

Step 1: Put headers on


Step 2: Solder PCB with components 
- 
- Note that you might have to file the short ends of the PCB down a little to make a snug press fit into the casing. This was done so that any sort of glue was not needed when assembling the system. 

![1cfc37c3-a4c4-43a3-8807-36e35830bb72](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/ad65405a-cd9c-4c88-8103-e562556b2658)

Figure 15: Diital PCB layout

![99710684-73d5-4d9d-af0a-ddaa33372d5e](https://github.com/rhit-coultabm/Consistent-Cups-Espresso-Accessories/assets/92759594/6913f29b-a96e-4a03-ad73-0f881fcd4cd5)

Figure 16: Wiring diagram 

Step 3: Add battery connections


Step 4: Assemble 


   
