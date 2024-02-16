open Fest;

test("Burger with no toppings has no braces", () =>
  expect
  |> equal(
       Item.Burger.toEmoji({
         lettuce: false,
         onions: 0,
         cheese: 0,
         tomatoes: false,
         bacon: 0,
       }),
       {js|🍔|js},
     )
);
