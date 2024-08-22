module Voice = SpeechSynthesis.Voice;

let getVoices = () => {
  SpeechSynthesis.getVoices()
  |> Promise.map(voices =>
       voices
       |> Array.toList
       |> List.filter(v =>
            v |> Voice.getLang |> Js.String.startsWith(~prefix="en-")
          )
     );
};

let speak = (voice, text) => {
  module Utterance = SpeechSynthesis.Utterance;
  let utterance = Utterance.make(text);
  utterance |> Utterance.setVoice(voice);
  SpeechSynthesis.speak(utterance);
};

module Inner = {
  [@react.component]
  let make = (~voice as initVoice: Voice.t, ~voices: list(Voice.t)) => {
    let (counter, setCounter) = React.useState(() => 30);
    let (voice, setVoice) = RR.useStateValue(initVoice);
    let (intervalId, setIntervalId) = RR.useStateValue(None);

    let speakAndDecrement = () => {
      setCounter(value => {
        let newValue = value - 1;
        speak(voice, string_of_int(newValue));
        newValue;
      });
    };

    React.useEffect1(
      () => {
        if (counter == 0) {
          intervalId
          |> Option.iter(intervalId => {
               Js.Global.clearInterval(intervalId);
               setIntervalId(None);
               speak(voice, "Time's up!");
             });
        };
        None;
      },
      [|counter|],
    );

    <div>
      <NumberInput
        disabled={intervalId |> Option.is_some}
        value=counter
        onChange={newCounter => setCounter(_ => newCounter)}
      />
      <select
        value={Voice.getName(voice)}
        onChange={evt => {
          let name = RR.getValueFromEvent(evt);
          voices
          |> List.find_opt(v => Voice.getName(v) == name)
          |> Option.iter(setVoice);
        }}>
        {voices
         |> List.map(v => {
              let name = v |> Voice.getName;
              <option key=name value=name> {name |> RR.s} </option>;
            })
         |> RR.list}
      </select>
      <button
        onClick={_evt =>
          switch (intervalId) {
          | Some(intervalId) =>
            Js.Global.clearInterval(intervalId);
            setIntervalId(None);
          | None =>
            Js.Global.setInterval(1000, ~f=speakAndDecrement)
            |> Option.some
            |> setIntervalId
          }
        }>
        {(intervalId == None ? "Start" : "Stop") |> RR.s}
      </button>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voices, setVoices) = RR.useStateValue([]);

  React.useEffect0(() => {
    getVoices() |> Promise.iter(setVoices);
    None;
  });

  switch (voices) {
  | [] => React.null
  | [voice, ..._] => <Inner voice voices />
  };
};
