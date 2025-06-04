# Spring103-final
reaction time game

discription:
  Display random led 0-9 in yellow or blue 
  Set a timer of 5 secs for the led to be on
  Left button corresponds to blue and right bottom corresponds to yellow
  
  Check the following
  If left button is touched within the 5 sec time frame and the led is blue, play celebration sequence 
  If right button is touched within the 5 sec time frame and the led is yellow, play celebration sequence
  If right button is touched within the 5 sec time frame and the led is blue, play fail sequence and 
  If left button is touched within the 5 sec time frame and the led is yellow, play fail sequence
  If no button is pressed within the 5 sec time frame, play fail sequence
  
  Have a interrupt switch that when flipped increases difficulty by repeat the previous code but with new timer of 3 seconds
  
  Then record the score is the serial monitor “your score: ”as a win repeat the whole setup again with the time frame being 3 secs
  
  Finishing sequences:
  Celebration sequence displays a smiley face led pattern in purple and two frequencies that get higher “you got it!”
  Fail sequence displays all leds in red and two frequencies that get lower “nope, too bad”
  
rules:
  when blue press left button 
  when yellow press right button
  if your fast enough, celebrate!
  too slow, too bad
  flip the switch it make it harder!
  
inputs:
  Right button pressed
  Left button pressed
  switch
  timer
  Random index

outputs:
  Celebration led
  Celebration tone
  Fail led
  Fail tone
  Serial output

