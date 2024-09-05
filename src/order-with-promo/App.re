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

[@react.component]
let make = () => {
  let (date, setDate) =
    RR.useStateValue(Js.Date.fromString("2024-05-28T00:00"));

  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    <DateInput date onChange=setDate />
    {datasets
     |> List.map(((label, items)) => {
          <div key=label> <h3> {RR.s(label)} </h3> <Order items date /> </div>
        })
     |> RR.list}
  </div>;
};
