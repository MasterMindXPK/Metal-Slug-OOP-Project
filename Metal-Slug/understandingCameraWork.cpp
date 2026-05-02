
/*
World:
[0] -- - [320]--------[1600]--------[3200]-- - ...
↑Player

Screen shows world 0 - 1600 :
    ┌──────────────────────────────────────┐
    │   ↓ Player at screenX = 320            │
    │[P]                                 │
    └──────────────────────────────────────┘

    Calculation :
screenX = 320 - 0 = 320
Is 320 > 1181 ? NO
Is 320 < 100 ? NO
    → Camera doesn't move


    -----------------

    player_x = 1500;
cameraOffset = 0;  // Still hasn't moved yet


  Calculation:
screenX = 1500 - 0 = 1500
Is 1500 > maxScreenX (1181)? YES!
→ Camera must move!

New cameraOffset = playerWorldX - maxScreenX
                 = 1500 - 1181
                 = 319  
    
    World:
[0]-------[319]----[1500]---[1919]-----[3200]---...
                   ↑Player

Screen now shows world 319-1919:
          ┌──────────────────────────────────────┐
          │                  ↓ Player at x=1181  │
          │                 [P]                  │
          └──────────────────────────────────────┘

New calculation with cameraOffset=319:
screenX = 1500 - 319 = 1181
Player is now locked at screenX=1181


-----------------
playerX=2000

Old cameraOffset = 319

Calculation:
screenX = 2000 - 319 = 1681
Is 1681 > 1181? YES!

New cameraOffset = 2000 - 1181 = 819
    
    
    
    World:
[0]-------[819]--------[2000]---[2419]-----[3200]---...
                       ↑Player

Screen now shows world 819-2419:
                  ┌──────────────────────────────────────┐
                  │                  ↓ Player at x=1181  │
                  │                 [P]                  │
                  └──────────────────────────────────────┘

screenX = 2000 - 819 = 1181
Player stays locked at x=1181, world scrolls beneath them
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    */