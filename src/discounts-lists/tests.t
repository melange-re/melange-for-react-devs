Sandwich tests
  $ node ./output/src/discounts-lists/SandwichTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: Item.Sandwich.toEmoji
  ok 1 - Item.Sandwich.toEmoji
    ---
    ...
  # Subtest: Item.Sandwich.toPrice
  ok 2 - Item.Sandwich.toPrice
    ---
    ...
  # Subtest: Item.Sandwich.toPrice returns lower price for Turducken on Tuesdays
  ok 3 - Item.Sandwich.toPrice returns lower price for Turducken on Tuesdays
    ---
    ...
  1..3
  # tests 3
  # suites 0
  # pass 3
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0

Burger tests
  $ node ./output/src/discounts-lists/BurgerTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: A fully-loaded burger
  ok 1 - A fully-loaded burger
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
  # Subtest: Burger with more than 12 toppings should also show bowl emoji
  ok 5 - Burger with more than 12 toppings should also show bowl emoji
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
  $ node ./output/src/discounts-lists/DiscountTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: 0 burgers, no discount
  ok 1 - 0 burgers, no discount
    ---
    ...
  # Subtest: 1 burger, no discount
  ok 2 - 1 burger, no discount
    ---
    ...
  # Subtest: 2 burgers of same price, discount
  ok 3 - 2 burgers of same price, discount
    ---
    ...
  # Subtest: 2 burgers of different price, discount of cheaper one
  ok 4 - 2 burgers of different price, discount of cheaper one
    ---
    ...
  # Subtest: 3 burgers of different price, return Some(15.15)
  ok 5 - 3 burgers of different price, return Some(15.15)
    ---
    ...
  # Subtest: 7 burgers, return Some(46.75)
  ok 6 - 7 burgers, return Some(46.75)
    ---
    ...
  # Subtest: No burger has 1+ of every topping, return None
  ok 7 - No burger has 1+ of every topping, return None
    ---
    ...
  # Subtest: One burger has 1+ of every topping, return Some
  ok 8 - One burger has 1+ of every topping, return Some
    ---
    ...
  1..8
  # tests 8
  # suites 0
  # pass 8
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0
