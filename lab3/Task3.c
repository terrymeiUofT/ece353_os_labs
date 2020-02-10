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
    Players: array [8] of Thread = new array of Thread {8 of new Thread}
    -- Hint: Some variables are defined in "Task3.h".

  function gamingparlor()
    GP.Init()

    -- Remove the following line in your implementation
    -- This is only an example.
    -- GP.PrintExample()

    -- Add more code below
    Players[0].Init("Player A")
    Players[0].Fork(askFrontDesk, group_names[0], dice_per_group[0])
    Players[0].Init("Player B")
    Players[0].Fork(askFrontDesk, group_names[1], dice_per_group[1])
    Players[0].Init("Player C")
    Players[0].Fork(askFrontDesk, group_names[2], dice_per_group[2])
    Players[0].Init("Player D")
    Players[0].Fork(askFrontDesk, group_names[3], dice_per_group[3])
    Players[0].Init("Player E")
    Players[0].Fork(askFrontDesk, group_names[4], dice_per_group[4])
    Players[0].Init("Player F")
    Players[0].Fork(askFrontDesk, group_names[5], dice_per_group[5])
    Players[0].Init("Player G")
    Players[0].Fork(askFrontDesk, group_names[6], dice_per_group[6])
    Players[0].Init("Player H")
    Players[0].Fork(askFrontDesk, group_names[7], dice_per_group[7])

  endFunction

  function askFrontDesk(name: char, nr_of_dice: int)
    var
        i: int
      for i = 0 to 5
        GP.Request(name, nr_of_dice)
        currentThread.Yield()
        GP.Return(name, nr_of_dice)
      endFor
  endFunction

  behavior GamingParlor

    method Init()
      dice_available = total_dice
      monMutex = new Mutex
      monMutex.Init()
      monCon = new Condition
      monCon.Init()
    endMethod

    method Request(name: char, nr_of_dice: int)
      monMutex.Lock()
      self.Print(name, "requests", nr_of_dice)
      while dice_available < nr_of_dice
        monCon.Wait(&monMutex)
      endWhile
      dice_available = dice_available - nr_of_dice
      self.Print(name, "proceeds with", nr_of_dice)
      monMutex.Unlock()
    endMethod

    method Return(name: char, nr_of_dice: int)
      monMutex.Lock()
      dice_available = dice_available + nr_of_dice
      self.Print(name, "releases and adds back", nr_of_dice)
      monCon.Broadcast(&monMutex)
      monMutex.Unlock()
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
