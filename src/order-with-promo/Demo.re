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

[@react.component]
let make = () => {
  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    {let (show, setShow) = React.useState(() => true);
     <>
       <button onClick={_ => setShow(v => !v)}>
         {(show ? "Hide" : "Show") |> RR.s}
       </button>
       {show ? <TalkyTimer /> : React.null}
     </>}
    {datasets
     |> List.map(((label, items)) => <DateAndOrder key=label label items />)
     |> RR.list}
  </div>;
};
