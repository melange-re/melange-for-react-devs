let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region type-annotate-argument
      |> Js.Array.filter(~f=(item: Item.t) =>
           switch (item) {
           | Burger(_) => true
           | Sandwich(_)
           | Hotdog => false
           }
         );
    // #endregion type-annotate-argument
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region full-name-constructors
      |> Js.Array.filter(~f=item =>
           switch (item) {
           | Item.Burger(_) => true
           | Item.Sandwich(_)
           | Item.Hotdog => false
           }
         );
    // #endregion full-name-constructors
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region full-name-constructor
      |> Js.Array.filter(~f=item =>
           switch (item) {
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false
           }
         );
    // #endregion full-name-constructor
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region full-name-fun
      |> Js.Array.filter(
           ~f=
             fun
             | Item.Burger(_) => true
             | Sandwich(_)
             | Hotdog => false,
         );
    // #endregion full-name-fun
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    // #region swap-function-order
    items
    |> Js.Array.filter(~f=item =>
         switch (item) {
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false
         }
       )
    |> Js.Array.sortInPlaceWith(~f=(item1, item2) =>
         - compare(Item.toPrice(item1), Item.toPrice(item2))
       )
    // #endregion swap-function-order
    |> ignore;
  };

let _ = {
  let burgers: array(Item.t) = [||];

  // #region match-on-tuple
  switch (burgers[0], burgers[1]) {
  | (Burger(_), Burger(cheaperBurger)) =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
  // #endregion match-on-tuple
};

let _ = {
  let burgers: array(Item.t) = [||];

  // #region check-array-length
  Js.Array.length(burgers) < 2
    ? None
    : (
      switch (burgers[0], burgers[1]) {
      | (Burger(_), Burger(cheaperBurger)) =>
        Some(Item.Burger.toPrice(cheaperBurger))
      | _ => None
      }
    );
  // #endregion check-array-length
};

let _ = {
  let burgers: array(Item.t) = [||];

  // #region catch-exception
  switch (burgers[0], burgers[1]) {
  | exception _ => None
  | (Burger(_), Burger(cheaperBurger)) =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
  // #endregion catch-exception
};

module Array = {
  // #region module-array
  // Safe array access function
  let get: (array('a), int) => option('a) =
    (array, index) =>
      switch (index) {
      | index when index < 0 || index >= Js.Array.length(array) => None
      | index => Some(Stdlib.Array.get(array, index))
      };
  // #endregion module-array
};

let _ = {
  let burgers: array(Item.t) = [||];

  // #region custom-array-get
  switch (burgers[0], burgers[1]) {
  | (Some(Burger(_)), Some(Burger(cheaperBurger))) =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
  // #endregion custom-array-get
};

// #region improved-get-free-burger
// Buy 2 burgers, get 1 free
let getFreeBurger = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(~f=item =>
         switch (item) {
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false
         }
       )
    |> Js.Array.map(~f=Item.toPrice)
    |> Js.Array.sortInPlaceWith(~f=(x, y) => - compare(x, y));

  switch (burgers[0], burgers[1]) {
  | (Some(_), Some(cheaperPrice)) => Some(cheaperPrice)
  | (None | Some(_), None | Some(_)) => None
  };
};
// #endregion improved-get-free-burger

// #region get-half-off-one
// Buy 1+ burger with 1 of every topping, get half off
let getHalfOff = (items: array(Item.t)) => {
  let meetsCondition =
    items
    |> Js.Array.some(
         ~f=
           fun
           | Item.Burger({
               lettuce: true,
               tomatoes: true,
               onions: 1,
               cheese: 1,
               bacon: 1,
             }) =>
             true
           | Burger(_)
           | Sandwich(_)
           | Hotdog => false,
       );

  switch (meetsCondition) {
  | false => None
  | true =>
    let total =
      items
      |> Js.Array.reduce(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Some(total /. 2.0);
  };
};
// #endregion get-half-off-one
ignore(getHalfOff);

// #region get-half-off
// Buy 1+ burger with 1+ of every topping, get half off
let getHalfOff = (items: array(Item.t)) => {
  let meetsCondition =
    items
    |> Js.Array.some(
         ~f=
           fun
           | Item.Burger({
               lettuce: true,
               tomatoes: true,
               onions,
               cheese,
               bacon,
             })
               when onions > 0 && cheese > 0 && bacon > 0 =>
             true
           | Burger(_)
           | Sandwich(_)
           | Hotdog => false,
       );

  switch (meetsCondition) {
  | false => None
  | true =>
    let total =
      items
      |> Js.Array.reduce(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Some(total /. 2.0);
  };
};
// #endregion get-half-off

let _ =
  name => {
    // #region return-variant-at-end
    let result =
      name
      |> String.split_on_char(' ')
      |> List.map(String.map(c => c |> Char.code |> (+)(1) |> Char.chr))
      |> String.concat(" ")
      |> String.cat("Hello, ");

    Some(result);
    // #endregion return-variant-at-end
  };
