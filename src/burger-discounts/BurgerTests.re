open Fest;

let burger: Item.Burger.t = {
  lettuce: false,
  onions: 0,
  cheese: 0,
  tomatoes: false,
  bacon: 0,
};

test("Burger with no toppings doesn't have braces", () =>
  expect |> equal(Item.Burger.toEmoji(burger), {js|🍔|js})
);

test("Burger with 0 of onions, cheese, or bacon doesn't show those emoji", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({...burger, lettuce: true, tomatoes: true}),
       {js|🍔{🥬,🍅}|js},
     )
);

test(
  "Burger with 1 of onions, cheese, or bacon should show just the emoji without ×",
  () =>
  expect
  |> equal(
       Item.Burger.toEmoji({...burger, onions: 1, cheese: 1, bacon: 1}),
       {js|🍔{🧅,🧀,🥓}|js},
     )
);

test("Burger with 2 or more of onions, cheese, or bacon should show ×", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({...burger, onions: 2, cheese: 3, bacon: 4}),
       {js|🍔{🧅×2,🧀×3,🥓×4}|js},
     )
);

test("Burger with more than 12 toppings should also show bowl", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         tomatoes: true,
         onions: 4,
         cheese: 2,
         bacon: 5,
       }),
       {js|🍔🥣{🥬,🍅,🧅×4,🧀×2,🥓×5}|js},
     )
);
