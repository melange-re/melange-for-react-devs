// module Td = [%styled.td {|
//   padding: 0.5em;
// |}];
//
// include Td;

[@react.component]
let make = (~className="", ~children) =>
  <td className={className ++ " " ++ [%cx "padding: 0.5em"]}> children </td>;
