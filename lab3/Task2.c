code Main

  -- OS Class: Project 3
  --
  -- <Tianchang Mei>
  --

  -- This package contains the following:
  --     Sleeping Barber
-----------------------------  Main  ---------------------------------

  function main ()
      InitializeScheduler ()
      sleepingbarber ()
      ThreadFinish ()
    endFunction

-----------------------------  Sleeping Barber --------------------------------
  -- In the Producer/Consumer and Dining Philosophers problems, we tried to print the activity
  -- of each thread in a different column so that you could see what each thread was doing.
  -- The actual events in this problem can be reduced to a single letter, such as:
  --
  --         E = Enter
  --         S = Sit in waiting chair
  --         B = Begin haircut
  --         F = Finish haircut
  --         L = Leave
  --         start = Barber begins haircut
  --         end = Barber ends haircut
  --
  -- The status of the chairs should be printed every time you print a line. If there are 5
  -- chairs, of which 2 are occupied, print 2 X's and 3 -'s.  For example:
  --
  --         XX---
  --

  enum Start, End

  var
    sb: SleepingBarber
    bStatus: int = End
    cStatus: array[nrCustomers] of char = 
             new array of char { nrCustomers of 'L' }
    -- Hint: Some variables are defined in "Task2.h".

  function sleepingbarber()
    var
      i: int
      barberTh: Thread
      customersTh: array[nrCustomers] of Thread

    -- print initial line
    for i = 0 to nrChairs
        printChar(' ')
    endFor
    print("Barber\t")
    for i = 0 to nrCustomers - 1
        printInt(i+1)
        printChar('\t')
    endFor
    nl()

    sb = new SleepingBarber
    sb.Init(bStatus, &cStatus, nrChairs)

    -- Remove the following line in your implementation
    -- This is only an example.
    -- sb.printExample()

    -- Add more code below.
    customerSem = new Semaphore
    customerSem.Init(0)
    barberSem = new Semaphore
    barberSem.Init(0)
    barber_done = new Semaphore
    barber_done.Init(0)

    access_lock = new Mutex
    access_lock.Init()

    occupied_chairs = 0

    barberTh = new Thread
    barberTh.Init("Barber")
    barber.Fork(barber, nrChairs)
    customersTh = new array of Thread {nrCustomers of Thread}
    customers[0].Init("Customer 1")
    customers[0].Fork(customer, 0)
    customers[1].Init("Customer 2")
    customers[1].Fork(customer, 1)
    customers[2].Init("Customer 3")
    customers[2].Fork(customer, 2)
    customers[3].Init("Customer 4")
    customers[3].Fork(customer, 3)
    customers[4].Init("Customer 5")
    customers[4].Fork(customer, 4)
    customers[5].Init("Customer 6")
    customers[5].Fork(customer, 5)
    customers[6].Init("Customer 7")
    customers[6].Fork(customer, 6)
    customers[7].Init("Customer 8")
    customers[7].Fork(customer, 7)
    customers[8].Init("Customer 9")
    customers[8].Fork(customer, 8)
    customers[9].Init("Customer 10")
    customers[9].Fork(customer, 9)

  endFunction


  function barber(numChairs: int)
    while true
        customerSem.Down()
        access_lock.Lock()          -- critical section starts
        occupied_chairs = occupied_chairs - 1
        sb.availChairs = sb.availChairs + 1
        access_lock.Unlock()        -- critical section ends
        sb.barberStatus = Start
        sb.printBarberStatus()      -- print barber status before yield to a customer thread
        barberSem.Up()              -- the barber is now ready to start
        currentThread.Yield()       -- imitate the time for haircut
        barber_done.Up()            -- the barber is now done with the haircut
        sb.barberStatus = End
        sb.printBarberStatus()      -- print barberstatus after a customer thread is done
     endWhile
  endFunction


  function customer(p: int)
    access_lock.Lock()              -- critical section starts
    sb.customerStatus[p] = 'E'
    sb.printCustomerStatus(p)       -- customer p enters barber shop
    if occupied_chairs < numChairs
        occupied_chairs = occupied_chairs + 1
        sb.availChairs = sb.availChairs - 1
        access_lock.Unlock()        -- critical section ends

        sb.customerStatus[p] = 'S'
        sb.printCustomerStatus(p)   -- customer p sits down

        customerSem.Up()            -- one more customer has taken a chair
        barberSem.Down()            -- waiting for barber to get ready

        sb.customerStatus[p] = 'B'
        sb.printCustomerStatus(p)   -- customer p begins haircut
        currentThread.Yield()       -- imitate the time for hair cut,
                                    -- yield current thread to some other customers

        sb.customerStatus[p] = 'F'
        sb.printCustomerStatus(p)   -- customer p finishes haircut

        Barber_done.Down()          -- waiting for barber to be done
    else
        access_lock.Unlock()        -- if all chairs are occupied, exit critical section (leave barbershop)
    endIf
    sb.customerStatus[p] = 'L'
    sb.printCustomerStatus(p)       -- customer p leaves barber shop
  endFunction


  -- implementation of SleepingBarber class
  behavior SleepingBarber
    method Init(bStat: int, cStat: ptr to array of char, numOfChair: int)
      self.barberStatus = bStat
      self.customerStatus = cStat
      self.availChairs = numOfChair
    endMethod

    method printExample()
      ----------------------------- Example -----------------------------
      -- This is an example of using print functions.
      -- You should not use this function in your implementation.

      -- Customer 1 enter.
      self.customerStatus[0] = 'E'
      self.printCustomerStatus(0)

      -- Customer 1 sit in waiting chair.
      self.customerStatus[0] = 'S'
      self.availChairs = self.availChairs - 1
      self.printCustomerStatus(0)

      -- Customer 2 enter.
      self.customerStatus[1] = 'E'
      self.printCustomerStatus(1)

      -- Customer 2 sit in waiting chair.
      self.customerStatus[1] = 'S'
      self.availChairs = self.availChairs - 1
      self.printCustomerStatus(1)

      -- Barber begins haircut. Customer begins haircut and chair becomes available.
      self.barberStatus = Start
      self.availChairs = self.availChairs + 1
      self.printBarberStatus()

      -- Customer 1 begins haircut.
      self.customerStatus[0] = 'B'
      self.printCustomerStatus(0)

      -- Barber ends haircut.
      self.barberStatus = End
      self.printBarberStatus()

      -- Customer 1 finish haircut.
      self.customerStatus[0] = 'F'
      self.printCustomerStatus(0)

      -- Customer 1 leave.
      self.customerStatus[0] = 'L'
      self.printCustomerStatus(0)

      -- Barber begins haircut. Customer begins haircut and chair becomes available.
      self.barberStatus = Start
      self.availChairs = self.availChairs + 1
      self.printBarberStatus()

      -- Customer 2 begins haircut.
      self.customerStatus[1] = 'B'
      self.printCustomerStatus(1)

      -- Barber ends haircut.
      self.barberStatus = End
      self.printBarberStatus()

      -- Customer 2 finish haircut.
      self.customerStatus[1] = 'F'
      self.printCustomerStatus(1)

      -- Customer 2 leave.
      self.customerStatus[1] = 'L'
      self.printCustomerStatus(1)
      -----------------------------  End  -----------------------------
    endMethod

    method printBarberStatus()
      self.printChairs()
      if self.barberStatus == Start
         print("Start\n")
      else
         print("End\n")
      endIf
    endMethod

    method printCustomerStatus(customer: int)
      var i: int
      self.printChairs()
      print("      \t")
      for i = 0 to nrCustomers - 1
        if (i != customer)
          printChar(' ')
        else
          printChar(self.customerStatus[i])
        endIf
        printChar('\t')
      endFor
      nl()
    endMethod

    method printChairs()
      var i: int
      for i = self.availChairs + 1 to nrChairs
        printChar('X')
      endFor
      for i = 1 to self.availChairs
        printChar('-')
      endFor
      printChar(' ')
    endMethod

  endBehavior

endCode
