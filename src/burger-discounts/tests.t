Burger tests
  $ node ./output/src/burger-discounts/BurgerTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: Burger with no toppings doesn't have braces
  ok 1 - Burger with no toppings doesn't have braces
    ---
    ...
  # Subtest: Burger with 0 of onions, cheese, or bacon doesn't show those emoji
  ok 2 - Burger with 0 of onions, cheese, or bacon doesn't show those emoji
    ---
    ...
  # Subtest: Burger with 1 of onions, cheese, or bacon should show just the emoji without Ã
  ok 3 - Burger with 1 of onions, cheese, or bacon should show just the emoji without Ã
    ---
    ...
  # Subtest: Burger with 2 or more of onions, cheese, or bacon should show Ã
  ok 4 - Burger with 2 or more of onions, cheese, or bacon should show Ã
    ---
    ...
  # Subtest: Burger with more than 12 toppings should also show bowl
  ok 5 - Burger with more than 12 toppings should also show bowl
    ---
    ...
  1..5
  # tests 5
  # suites 0
  # pass 5
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0

Discount tests
  $ node ./output/src/burger-discounts/DiscountTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: 1 burger, no discount
  ok 1 - 1 burger, no discount
    ---
    ...
  # Subtest: 2 burgers of same price, discount
  ok 2 - 2 burgers of same price, discount
    ---
    ...
  # Subtest: 2 burgers of different price, discount of cheaper one
  ok 3 - 2 burgers of different price, discount of cheaper one
    ---
    ...
  # Subtest: input array isn't changed
  ok 4 - input array isn't changed
    ---
    ...
  # Subtest: 3 burgers of different price, discount of second one
  ok 5 - 3 burgers of different price, discount of second one
    ---
    ...
  1..5
  # tests 5
  # suites 0
  # pass 5
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0
