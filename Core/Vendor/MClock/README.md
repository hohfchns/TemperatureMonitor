# stm_mclock

Usage:
``` c
{
  // In init         
                     // v duration time in ms
  MCL_Init(&yourClock, 500);
}

{
  // In 1ms timer
  MCL_Update(&yourClock);
}

{
  // In update loop
  if (MCL_Pull(&yourClock) == MCL_STATE_TIMEOUT)
  {
    // Do something, clock will restart automatically
  }
}

```
