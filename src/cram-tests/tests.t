Sandwich tests
  $ node ./output/src/cram-tests/SandwichTests.mjs | sed '/duration_ms/d'
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
