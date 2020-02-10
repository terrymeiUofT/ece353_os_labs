code Main

  -- OS Class: Project 3
  --
  -- <Tianchang Mei>
  --

  -- This package contains the following:
  --     Gaming Parlor
-----------------------------  Main  ---------------------------------

  function main ()
      InitializeScheduler ()
      gamingparlor ()
      ThreadFinish ()
    endFunction

------------------------------- Gaming Parlor ----------------------------------
  var
    GP: GamingParlor = new GamingParlor
    gpMutex: Mutex
    gpCon: Condition
    Players: array [8] of Thread
    -- Hint: Some variables are defined in "Task3.h".

  function gamingparlor()
    GP.Init()

    -- Remove the following line in your implementation
    -- This is only an example.
    -- GP.PrintExample()

    -- Add more code below
    gpMutex = new Mutex
    gpMutex.Init()
    gpCon = new Condition
    gpCon.Init()

    Players = new array of Thread {8 of new Thread}

    Players[0].Init("Player A")
    Players[0].Fork(askFrontDesk, 0)
    Players[0].Init("Player B")
    Players[0].Fork(askFrontDesk, 1)
    Players[0].Init("Player C")
    Players[0].Fork(askFrontDesk, 2)
    Players[0].Init("Player D")
    Players[0].Fork(askFrontDesk, 3)
    Players[0].Init("Player E")
    Players[0].Fork(askFrontDesk, 4)
    Players[0].Init("Player F")
    Players[0].Fork(askFrontDesk, 5)
    Players[0].Init("Player G")
    Players[0].Fork(askFrontDesk, 6)
    Players[0].Init("Player H")
    Players[0].Fork(askFrontDesk, 7)
  endFunction

  function askFrontDesk(p: int)
    var
        i: int
        name: char
        nr_of_dice: int
      for i = 0 to 5
        name = group_names[p]
        nr_of_dice = dice_per_group[p]
        GP.Request(name, nr_of_dice)
        currentThread.Yield()
        GP.Return(name, nr_of_dice)
      endFor
  endFunction

  behavior GamingParlor

    method Init()
      dice_available = total_dice
    endMethod

    method Request(name: char, nr_of_dice: int)
      gpMutex.Lock()
      self.Print(name, "requests", nr_of_dice)
      while dice_available < nr_of_dice
        gpCon.Wait(&monMutex)
      endWhile
      dice_available = dice_available - nr_of_dice
      self.Print(name, "proceeds with", nr_of_dice)
      gpMutex.Unlock()
    endMethod

    method Return(name: char, nr_of_dice: int)
      gpMutex.Lock()
      dice_available = dice_available + nr_of_dice
      self.Print(name, "releases and adds back", nr_of_dice)
      gpCon.Broadcast(&monMutex)
      gpMutex.Unlock()
    endMethod

    method PrintExample()
      ----------------------------- Example -----------------------------
      -- This is an example of using print functions.
      -- You should not use this function in your implementation.

      -- A requests
      self.Request(group_names[0], 4)
      -- B requests
      self.Request(group_names[1], 4)
      -- B returns
      self.Return(group_names[1], 4)
      -- A returns
      self.Return(group_names[0], 4)
      -- C requests
      self.Request(group_names[2], 5)
      -- C returns
      self.Return(group_names[2], 5)

    endMethod

    method Print(name: char, str: String, nr_of_dice: int)
    --
    -- This method prints the current thread's name and the arguments.
    -- It also prints the current number of dice available.
    --
       printChar(name)
       print(" ")
       print(str)
       print(" ")
       printInt(nr_of_dice)
       nl()
       print("------------------------------Number of dice now avail = ")
       printInt(dice_available)
       nl()
    endMethod
  endBehavior

endCode
