let getBurgerDiscountOriginal = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         fun
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith((a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  switch (burgers) {
  | [|Burger(_), Burger(cheaperBurger)|] =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};

let getBurgerDiscountCatchException = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         fun
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith((a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  switch (burgers[0], burgers[1]) {
  | exception _ => None
  | (Burger(_), Burger(cheaperBurger)) =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};

let arrayGet = (array, int) =>
  switch (array[int]) {
  | exception _ => None
  | element => Some(element)
  };

let getBurgerDiscount = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         fun
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith((a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  switch (arrayGet(burgers, 0), arrayGet(burgers, 1)) {
  | (Some(Burger(_)), Some(Burger(cheaperBurger))) =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};
