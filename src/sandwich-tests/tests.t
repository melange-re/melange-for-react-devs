Sandwich tests
  $ node ./output/src/sandwich-tests/SandwichTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: Item.Sandwich.toEmoji
  ok 1 - Item.Sandwich.toEmoji
    ---
    ...
  # Subtest: Test.Sandwich.toPrice returns lower price for Turducken on Tuesdays
  ok 2 - Test.Sandwich.toPrice returns lower price for Turducken on Tuesdays
    ---
    ...
  1..2
  # tests 2
  # suites 0
  # pass 2
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0

Burger tests
  $ node ./output/src/sandwich-tests/BurgerTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: Burger with no toppings has no braces
  ok 1 - Burger with no toppings has no braces
    ---
    ...
  1..1
  # tests 1
  # suites 0
  # pass 1
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0
