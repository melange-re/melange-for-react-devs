// #region new-file
open Fest;

test("A fully-loaded burger", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         onions: 2,
         cheese: 3,
         tomatoes: true,
         bacon: 4,
       }),
       {js|🍔|js},
     )
);
// #endregion new-file

// #region first-test-fixed
test("A fully-loaded burger", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         onions: 2,
         cheese: 3,
         tomatoes: true,
         bacon: 4,
       }),
       {js|🍔{🥬,🍅,🧅×2,🧀×3,🥓×4}|js},
     )
);
// #endregion first-test-fixed

// #region three-tests-fixed
test("Burger with 0 of onions, cheese, or bacon doesn't show those emoji", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         tomatoes: true,
         onions: 0,
         cheese: 0,
         bacon: 0,
       }),
       {js|🍔{🥬,🍅}|js},
     )
);

test(
  "Burger with 1 of onions, cheese, or bacon should show just the emoji without ×",
  () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         tomatoes: true,
         onions: 1,
         cheese: 1,
         bacon: 1,
       }),
       {js|🍔{🥬,🍅,🧅,🧀,🥓}|js},
     )
);

test("Burger with 2 or more of onions, cheese, or bacon should show ×", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         tomatoes: true,
         onions: 2,
         cheese: 2,
         bacon: 2,
       }),
       {js|🍔{🥬,🍅,🧅×2,🧀×2,🥓×2}|js},
     )
);
// #endregion three-tests-fixed

// #region bowl-test
test("Burger with more than 12 toppings should also show bowl emoji", () => {
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
     );

  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: true,
         tomatoes: true,
         onions: 4,
         cheese: 2,
         bacon: 4,
       }),
       {js|🍔{🥬,🍅,🧅×4,🧀×2,🥓×4}|js},
     );
});
// #endregion bowl-test
