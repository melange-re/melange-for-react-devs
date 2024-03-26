// Only successfully match on two-burger array
let getFreeBurgerOriginal = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         ~f=
           fun
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith(~f=(a, b) => {
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

// Match on first and second elements of array, catch exception
let getFreeBurgerCatchException = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         ~f=
           fun
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith(~f=(a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  // switch (burgers[0], burgers[1]) {
  switch (Stdlib.Array.get(burgers, 0), Stdlib.Array.get(burgers, 1)) {
  | exception _ => None
  | (Burger(_), Burger(cheaperBurger)) =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};

// Buy 2 burgers, get the cheaper one free
let getFreeBurger = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         ~f=
           fun
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith(~f=(a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  switch (burgers[0], burgers[1]) {
  | (Some(Burger(_)), Some(Burger(cheaperBurger))) =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};

let getHalfOffFirst = (items: array(Item.t)) => {
  let meetsCondition =
    items
    |> Js.Array.some(
         ~f=
           fun
           | Item.Burger({
               lettuce: true,
               tomatoes: true,
               cheese: 1,
               bacon: 1,
               onions: 1,
             }) =>
             true
           | Burger(_)
           | Sandwich(_)
           | Hotdog => false,
       );

  switch (meetsCondition) {
  | false => None
  | true =>
    let sum =
      items
      |> Js.Array.reduce(~init=0.0, ~f=(sum, item) =>
           sum +. Item.toPrice(item)
         );
    Some(sum /. 2.0);
  };
};

let getHalfOff = (items: array(Item.t)) => {
  let meetsCondition =
    items
    |> Js.Array.some(
         ~f=
           fun
           | Item.Burger({
               lettuce: true,
               tomatoes: true,
               cheese,
               bacon,
               onions,
             })
               when cheese > 0 && bacon > 0 && onions > 0 =>
             true
           | Burger(_)
           | Sandwich(_)
           | Hotdog => false,
       );

  switch (meetsCondition) {
  | false => None
  | true =>
    let sum =
      items
      |> Js.Array.reduce(~init=0.0, ~f=(sum, item) =>
           sum +. Item.toPrice(item)
         );
    Some(sum /. 2.0);
  };
};
