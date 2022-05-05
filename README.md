## Your Project Answers

### Project Description

Weathering Station

We would like to be able to record weather measurements using peripheral add on devices. This can include wind measurements (speed and direction), precipitation measurements, etc. 

Counts rotations or revolutions to see how fast the wind is going. 

We will use the board display in order to show the weather measurements. 

We plan to code up using the specifics given with the peripherals and then code up a display for the board.

 
### Technical Approach

Weather device (precipitation machine) --> one of our members have

Adapters --> need to change adapters to add to the board

Wind direction and speed --> we have the physical tool for it 

Cup Anemometer: measures the speed of the wind

Direction (wind vane) : Each direction (for the wind) is associated with a certain voltage measurement 

Precipitation Container :  Ticks every few ounces of precipitation that gets drained out and measures how much precipitation we have total.

We plan to still use the current board or computer in order to display time, precipitation, and wind (speed + direction). 

We will continue to code in C and we will use the specific code language given by the peripheral device (we plan to read documentation for both our current board and the device as well in order to interconnect them). 
Weather device (precipitation machine) --> one of our members have

Adapters --> need to change adapters to add to the board

Wind direction and speed --> we have the physical tool for it 

Cup Anemometer: measures the speed of the wind

Direction (wind vane) : Each direction (for the wind) is associated with a certain voltage measurement 

Precipitation Container :  Ticks every few ounces of precipitation that gets drained out and measures how much precipitation we have total.

We plan to still use the current board or computer in order to display time, precipitation, and wind (speed + direction). 

We will continue to code in C and we will use the specific code language given by the peripheral device (we plan to read documentation for both our current board and the device as well in order to interconnect them). 

## Feedback.
This idea sounds cool! I think to give better feedback we would need to understand the weather device a little more. I am a little worried that the weather device will be doing most of the heavy lifing and the board will have a limited role. Are you planning to use a complete weather station and just display data using the LCD or is each instrument inerfaced to the baord separately? For the latter,  you can split up the testing by first working on the code that reads and analog voltage (weather vane) and then counts digital ticks (you can use a button to simulate). Try to focus on one or two of the instruments first instead of getting it all integrated in one go. 

Also, the board already has two relevant sensors, ambient light (could detect could cover) and a temperature sensore that is hooked up to the A/D converter. See the reference document for use. 

Good luck!

## Welcome team! Please edit me.
### You can use the 'main' branch for you code.
### The GitHub web-page should be in the 'gh-pages' branch
You can access the page adding a "pages" prefix to the url for the git repo e.g. if your browser currently shows https://github.coecis.cornell.edu/ece3140-sp22/lcd49-mib57-vb269 then the GitHub page is at https://pages.github.coecis.cornell.edu/ece3140-sp22/lcd49-mib57-vb269 . You can edit the source in the gh-pages branch or use the "automatic page generator" acessible via settings->options in the GitHub Pages Section toward the bottom. We recommend the latter approach since it is quick and easy. We made an initial wepage that you can edit and a description of the headings we expect to see.
