// #region initial
let items: Order.t = [
  Sandwich(Portabello),
  Sandwich(Unicorn),
  Sandwich(Ham),
  Sandwich(Turducken),
  Hotdog,
  Burger({lettuce: true, tomatoes: true, onions: 3, cheese: 2, bacon: 6}),
  Burger({lettuce: false, tomatoes: false, onions: 0, cheese: 0, bacon: 0}),
  Burger({lettuce: true, tomatoes: false, onions: 1, cheese: 1, bacon: 1}),
  Burger({lettuce: false, tomatoes: false, onions: 1, cheese: 0, bacon: 0}),
  Burger({lettuce: false, tomatoes: false, onions: 0, cheese: 1, bacon: 0}),
];

[@react.component]
let make = () => {
  let (date, setDate) =
    RR.useStateValue(Js.Date.fromString("2024-05-28T00:00"));

  <div>
    <h1> {RR.s("Order confirmation")} </h1>
    <DateInput date onChange=setDate />
    <h2> {RR.s("Order")} </h2>
    <Order items date />
  </div>;
};
// #endregion initial

// #region datasets
let burger =
  Item.Burger.{
    lettuce: false,
    tomatoes: false,
    onions: 0,
    cheese: 0,
    bacon: 0,
  };

let datasets = {
  [
    (
      "No burgers",
      Item.[
        Sandwich(Unicorn),
        Hotdog,
        Sandwich(Ham),
        Sandwich(Turducken),
        Hotdog,
      ],
    ),
    (
      "5 burgers",
      {
        [
          Burger({...burger, tomatoes: true}),
          Burger({...burger, lettuce: true}),
          Burger({...burger, bacon: 2}),
          Burger({...burger, cheese: 3, onions: 9, tomatoes: true}),
          Burger({...burger, onions: 2}),
        ];
      },
    ),
    (
      "1 burger with at least one of every topping",
      [
        Hotdog,
        Burger({
          lettuce: true,
          tomatoes: true,
          onions: 1,
          cheese: 2,
          bacon: 3,
        }),
        Sandwich(Turducken),
      ],
    ),
    (
      "All sandwiches",
      [
        Sandwich(Ham),
        Hotdog,
        Sandwich(Portabello),
        Sandwich(Unicorn),
        Hotdog,
        Sandwich(Turducken),
      ],
    ),
  ];
};
// #endregion datasets

/**
let datasets': list((string, list(Item.t))) = [
  // #region burger-expression
  {
    let burger =
      Item.Burger.{
        lettuce: false,
        tomatoes: false,
        onions: 0,
        cheese: 0,
        bacon: 0,
      };
    (
      "5 burgers",
      {
        [
          Burger({...burger, tomatoes: true}),
          Burger({...burger, lettuce: true}),
          Burger({...burger, bacon: 2}),
          Burger({...burger, cheese: 3, onions: 9, tomatoes: true}),
          Burger({...burger, onions: 2}),
        ];
      },
    );
  },
  // #endregion burger-expression
];
*/
ignore(make);

// #region refactor
[@react.component]
let make = () => {
  let (date, setDate) =
    RR.useStateValue(Js.Date.fromString("2024-05-28T00:00"));

  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    <DateInput date onChange=setDate />
    <h2> {RR.s("Order")} </h2>
    {datasets
     |> List.map(((label, items)) => {
          <div key=label> <h3> {RR.s(label)} </h3> <Order items date /> </div>
        })
     |> RR.list}
  </div>;
};
// #endregion refactor

ignore(make);

// #region date-and-order
module DateAndOrder = {
  [@react.component]
  let make = (~label: string, ~items: list(Item.t)) => {
    let (date, setDate) =
      RR.useStateValue(Js.Date.fromString("2024-05-28T00:00"));

    <div>
      <h2> {RR.s(label)} </h2>
      <DateInput date onChange=setDate />
      <Order items date />
    </div>;
  };
};
// #endregion date-and-order

// #region make
[@react.component]
let make = () => {
  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    {datasets
     |> List.map(((label, items)) => <DateAndOrder key=label label items />)
     |> RR.list}
  </div>;
};
// #endregion make
