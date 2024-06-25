/**

// #region bad-discount-type
type discount = [>
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError(
      [>
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | `NoSubmittedCode
];
// #endregion bad-discount-type
*/

// #region delete-refinement
type discount = [
  | `CodeError(Discount.error)
  | `Discount(float)
  | `DiscountError(
      [
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | `NoSubmittedCode
];
// #endregion delete-refinement

module TypeVar = {
  // #region type-variable
  type discount('a) = [
    | `CodeError(Discount.error)
    | `Discount(float)
    | `DiscountError('a)
    | `NoSubmittedCode
  ];
  // #endregion type-variable
};

/**
// #region explicit-type-var
type discount =
  [>
    | `CodeError(Discount.error)
    | `Discount(float)
    | `DiscountError(
        [>
          | `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers
        ],
      )
    | `NoSubmittedCode
  ] as 'a;
// #endregion explicit-type-var
*/
module AddTypeArg = {
  // #region add-type-arg
  type discount('a) =
    [>
      | `CodeError(Discount.error)
      | `Discount(float)
      | `DiscountError(
          [>
            | `MissingSandwichTypes
            | `NeedMegaBurger
            | `NeedOneBurger
            | `NeedTwoBurgers
          ],
        )
      | `NoSubmittedCode
    ] as 'a;
  // #endregion add-type-arg
};
